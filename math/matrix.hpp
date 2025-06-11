/**
 * matrix.hpp - meSP2's linear algebra submodule for making matrix math.
 *
 * Copyright (C) 2025  metaq3

 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

#pragma once

#include <cmath>
#include <cstddef>
#include <initializer_list>
#include <ostream>

namespace mesp2 {

template <size_t w, size_t h = w, typename T = double> class matrix {
 private:
    T _elements[h * w];

 public:
    matrix(T initialValue) {
        std::fill(_elements, _elements + w * h, initialValue);
    }

    matrix(std::initializer_list<T> values) {
        std::copy(values.begin(), values.end(), _elements);
    }

    ~matrix() {}

    /**
     * @brief Fills elements with specified value
     *
     * @param initialValue - value to fill with
     */
    inline void fill(T initialValue) {
        std::fill(_elements, _elements + w * h, initialValue);
    }

    // ********************************************
    // *                OPERATORS                 *
    // ********************************************

    void operator+=(const matrix<w, h, T> &other) {
        for (size_t y = 0; y < h; y++) {
            for (size_t x = 0; x < w; x++) {
                this->_elements[y * w + x] += other._elements[y * w + x];
            }
        }
    }

    void operator-=(const matrix<w, h, T> &other) {
        for (size_t y = 0; y < h; y++) {
            for (size_t x = 0; x < w; x++) {
                this->_elements[y * w + x] -= other._elements[y * w + x];
            }
        }
    }

    // ********************************************
    // *             ACCESS FUNCTIONS             *
    // ********************************************

    inline T *operator[](size_t row) { return &this->_elements[row * w]; }
    inline const T *operator[](size_t row) const {
        return &this->_elements[row * w];
    }

    /**
     * @brief Creates identity *square* matrix
     */
    static matrix<w, w, T> identity();
};

// *************************************************************************
// *                                                                       *
// *                               OPERATORS                               *
// *                                                                       *
// *************************************************************************

template <size_t w, size_t h, typename T>
std::ostream &operator<<(std::ostream &os, const matrix<w, h, T> &matrix) {
    os << "<" << h << "x" << w << " matrix { ";

    for (size_t i = 0; i + 1 < w * h; i++) {
        os << matrix[i / w][i % w] << ", ";
    }

    os << matrix[h - 1][w - 1];

    return os << " }>";
}

template <typename T, size_t w, size_t h>
matrix<w, h, T> operator+(matrix<w, h, T> left, const matrix<w, h, T> &right) {
    left += right;

    return left;
}

template <typename T, size_t w, size_t h>
matrix<w, h, T> operator-(matrix<w, h, T> left, const matrix<w, h, T> &right) {
    left -= right;

    return left;
}

/**
 * @attention For frequent multiplication consider using `multiplyTo` with
 * buffer matrices as it does not create new matrix and therefore is more
 * memory-efficient ( see matrix `binpow` implementation for usage example )
 */
template <typename T, size_t w1, size_t h1, size_t w2>
matrix<w2, h1, T> operator*(const matrix<w1, h1, T> &left,
                            const matrix<w2, w1, T> &right) {
    matrix<w2, h1, T> out(T(0));

    for (size_t y = 0; y < h1; y++) {
        for (size_t x = 0; x < w1; x++) {
            const T el = left[y][x];

            for (size_t i = 0; i < w2; i++) {
                out[y][i] += el * right[x][i];
            }
        }
    }

    return out;
}

template <typename T, size_t w, size_t h>
bool operator==(const matrix<w, h, T> &left, const matrix<w, h, T> &right) {
    for (size_t y = 0; y < h; y++) {
        for (size_t x = 0; x < w; x++) {
            if (left[y][x] != right[y][x])
                return false;
        }
    }

    return true;
}

// *************************************************************************
// *                                                                       *
// *                                ALGEBRA                                *
// *                                                                       *
// *************************************************************************

/**
 * @brief Creates identity *square* matrix
 */
template <typename T, size_t w, size_t h> matrix<w, h, T> identity() {
    static_assert(w == h, "Identity matrix must be square.");

    matrix<w, w, T> out(T(0));

    for (size_t i = 0; i < w; i++) {
        out[i][i] = T(1);
    }

    return out;
}

/**
 * @brief Multiplies two matrices and saves the result into the third one.
 * @attention For frequent multiplication using this function is recommended as
 * it doesn't allocate memory and therefore is memory-efficient
 * @param out - where to write the result
 * @returns reference to out for convenient use ( BUT NOT a new matrix )
 */
template <typename T, size_t w1, size_t h1, size_t w2>
matrix<w2, h1, T> &multiplyTo(const matrix<w1, h1, T> &left,
                              const matrix<w2, w1, T> &right,
                              matrix<w2, h1, T> &out) {
    out.fill(T(0));

    for (size_t y = 0; y < h1; ++y) {
        for (size_t x = 0; x < w1; ++x) {
            const T el = left[y][x];

            for (size_t i = 0; i < w2; ++i) {
                out[y][i] += el * right[x][i];
            }
        }
    }

    return out;
}

/**
 * @brief Binary raises matrix to a power. Only square matrix can be raised to
 * power, as only square matrix can be multiplied by self.
 * @param x - matrix to raise to power to
 * @param power - power, should be some of integer type
 * @returns raised to power matrix
 */
template <typename T, typename U, size_t s>
matrix<s, s, T> binpow(matrix<s, s, T> x, U power) {
    if (power == 1) {
        return x;
    }

    matrix<s, s, T> out = identity<s, s, T>();
    matrix<s, s, T> buff;

    // Usual binpow that goes through binary represenation of power

    while (power > 0) {
        // For some reason, adding an "else" branch makes this code
        // faster, despite else branch is guaranteed to execute in the
        // next cycle ( so, logically, we should loose cycles rather than gain
        // them )
        if (power % 2) {
            multiplyTo(out, x, buff);
            std::swap(out, buff);
            power--;
        } else {
            multiplyTo(x, x, buff);
            std::swap(x, buff);
            power /= 2;
        }
    }

    return out;
};

// *************************************************************************
// *                                                                       *
// *                           MATRIX FUNCTIONS                            *
// *                                                                       *
// *************************************************************************

/**
 * @brief Generates rotation matrix for given angle in 2D space
 *
 * @tparam T
 * @param angle - in radians
 * @return matrix<2, 2, T>
 */
template <typename T> matrix<2, 2, T> rotationMatrix2d(T angle) {
    const T s = std::sin(angle), c = std::cos(angle);

    return matrix<2, 2, T>{c, -s, s, c};
}

/**
 * @brief Generates rotation matrix for given three angles along all 3
 * axes in 3d space
 *
 * @tparam T
 * @param angles
 * @return matrix<3, 3, T>
 */
template <typename T> matrix<3, 3, T> rotationMatrix3d(T angles[3]) {
    const T sa = std::sin(angles[0]), ca = std::cos(angles[0]),
            sb = std::sin(angles[1]), cb = std::cos(angles[1]),
            sg = std::sin(angles[2]), cg = std::cos(angles[2]);

    return matrix<3, 3, T>{cb * cg,
                           sa * sb * cg - ca * sg,
                           ca * sb * cg + sa * sg,
                           cb * sg,
                           sa * sb * sg + ca * cg,
                           ca * sb * sg - sa * sg,
                           -sb,
                           sa * cb,
                           ca * cb};
}

/**
 * @brief Generates rotation matrix within given rotation plane
 *
 * @tparam Dim
 * @tparam T
 * @param angle
 * @param dim1 - one of the axes to form a plane
 * @param dim2 - one of the axes to form a plane
 * @return matrix<Dim, Dim, T>
 */
template <size_t Dim, typename T>
matrix<Dim, Dim, T> rotationMatrixPlane(T angle, size_t dim1, size_t dim2) {
    const T s = std::sin(angle), c = std::cos(angle);

    matrix<Dim, Dim, T> result = identity<T, Dim>();

    result[dim1][dim1] = c;
    result[dim1][dim2] = -s;
    result[dim2][dim1] = s;
    result[dim2][dim2] = c;

    return result;
}

} // namespace mesp2

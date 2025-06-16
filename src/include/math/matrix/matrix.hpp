/**
 * matrix/matrix.hpp - meSP2's linear algebra submodule with matrix definition.
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

#include <algorithm>
#include <initializer_list>
#include <ostream>

namespace mesp2 {

template <size_t h, size_t w = h, typename T = float> class matrix {
 public:
    using value_type = T;

    matrix(T initialValue = T()) {
        std::fill(_elements, _elements + w * h, initialValue);
    }

    matrix(std::initializer_list<T> values) {
        std::copy(values.begin(), values.end(), _elements);
    }

    ~matrix() {}

    // *********************************************************************
    // *                                                                   *
    // *                            PUBLIC API                             *
    // *                                                                   *
    // *********************************************************************

    /**
     * @brief Fills elements with specified value
     *
     * @param initialValue - value to fill with
     */
    inline void fill(T initialValue) {
        std::fill(_elements, _elements + w * h, initialValue);
    }

    /**
     * @brief Calculates matrix's determinant
     *
     * @return T
     */
    T getDeterminant() const;

    /**
     * @brief Calculates matrix's trace
     *
     * @return T
     */
    T getTrace() const;

    /**
     * @brief Creates inverse matrix and returns it's copy
     *
     * @return matrix<w, w, T>
     */
    matrix<w, w, T> makeInverse() const;

    /**
     * @brief Creates transposed matrix and returns it's copy
     *
     * @return matrix<w, w, T>
     */
    matrix<w, h, T> makeTransposed() const;

    // *********************************************************************
    // *                                                                   *
    // *                             OPERATORS                             *
    // *                                                                   *
    // *********************************************************************

    template <size_t w2> void operator*=(const matrix<w2, w, T> &other) {
        // You could make this operator more efficient, but you will need to
        // take into account case, when matrix is multiplied by itself.
        // This is the reason, why I do it this way.
        (*this) = (*this) * other;
    }

    template <typename U> void operator*=(const U &coefficient) {
        for (size_t y = 0; y < h; y++) {
            for (size_t x = 0; x < w; x++) {
                this->_elements[y * w + x] *= coefficient;
            }
        }
    }

    template <typename U> void operator/=(const U &coefficient) {
        for (size_t y = 0; y < h; y++) {
            for (size_t x = 0; x < w; x++) {
                this->_elements[y * w + x] /= coefficient;
            }
        }
    }

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

    // *********************************************************************
    // *                                                                   *
    // *                         ACCESS FUNCTIONS                          *
    // *                                                                   *
    // *********************************************************************

    inline T *operator[](size_t row) { return &this->_elements[row * w]; }
    inline const T *operator[](size_t row) const {
        return &this->_elements[row * w];
    }

    // *********************************************************************
    // *                                                                   *
    // *                         STATIC FUNCTIONS                          *
    // *                                                                   *
    // *********************************************************************

    /**
     * @brief Creates identity *square* matrix
     */
    static constexpr matrix<w, w, T> identity();

 private:
    T _elements[h * w];
};

template <size_t h, size_t w, typename T>
constexpr matrix<w, w, T> matrix<h, w, T>::identity() {
    static_assert(w == h, "Identity matrix must be square.");

    matrix<w, w, T> out(T(0));

    for (size_t i = 0; i < w; i++) {
        out[i][i] = T(1);
    }

    return out;
}

template <size_t h, size_t w, typename T>
matrix<w, h, T> matrix<h, w, T>::makeTransposed() const {
    matrix<w, h, T> out = {};

    for (size_t i = 0; i < h; i++) {
        for (size_t j = 0; j < w; ++j) {
            out[j][i] = (*this)[i][j];
        }
    }

    return out;
}

// *************************************************************************
// *                                                                       *
// *                               OPERATORS                               *
// *                                                                       *
// *************************************************************************

template <size_t h, size_t w, typename T>
std::ostream &operator<<(std::ostream &os, const matrix<h, w, T> &matrix) {
    os << "<" << h << "x" << w << " matrix { ";

    for (size_t i = 0; i + 1 < w * h; i++) {
        os << matrix[i / w][i % w] << ", ";
    }

    os << matrix[h - 1][w - 1];

    return os << " }>";
}

template <typename T, size_t h, size_t w>
matrix<h, w, T> operator+(matrix<h, w, T> left, const matrix<h, w, T> &right) {
    left += right;

    return left;
}

template <typename T, size_t h, size_t w>
matrix<h, w, T> operator-(matrix<h, w, T> left, const matrix<h, w, T> &right) {
    left -= right;

    return left;
}

template <typename T, typename U, size_t h, size_t w>
matrix<h, w, T> operator*(matrix<h, w, T> left, const U &right) {
    left *= right;

    return left;
}

template <typename T, typename U, size_t h, size_t w>
matrix<h, w, T> operator/(matrix<h, w, T> left, const U &right) {
    left /= right;

    return left;
}

/**
 * @attention For frequent multiplication consider using `multiplyTo` with
 * buffer matrices as it does not create new matrix and therefore is more
 * memory-efficient ( see matrix `binpow` implementation for usage example )
 */
template <typename T, size_t w1, size_t h1, size_t w2>
matrix<w2, h1, T> operator*(const matrix<h1, w1, T> &left,
                            const matrix<w1, w2, T> &right) {
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

template <typename T, size_t h, size_t w>
bool operator==(const matrix<h, w, T> &left, const matrix<h, w, T> &right) {
    for (size_t y = 0; y < h; y++) {
        for (size_t x = 0; x < w; x++) {
            if (left[y][x] != right[y][x])
                return false;
        }
    }

    return true;
}

}; // namespace mesp2
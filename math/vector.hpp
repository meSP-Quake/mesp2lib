/**
 * vector.hpp - meSP2's linear algebra submodule for making vector math
 *              as special case of matrix.
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
#include <cmath>
#include <cstddef>
#include <initializer_list>
#include <math/matrix.hpp>
#include <ostream>

namespace mesp2 {

// *********************************************

template <size_t Dimensions, typename T> struct coordinates_t {
 protected:
    T _c[Dimensions];
};

template <typename T> struct coordinates_t<1, T> {
    union {
        T x;
        T _c[1];
    };
};

template <typename T> struct coordinates_t<2, T> {
    union {
        struct {
            T x, y;
        };
        T _c[2];
    };
};

template <typename T> struct coordinates_t<3, T> {
    union {
        struct {
            T x, y, z;
        };
        T _c[3];
    };
};

template <typename T> struct coordinates_t<4, T> {
    union {
        struct {
            T x, y, z, w;
        };
        T _c[4];
    };
};

template <size_t Dimensions, typename T = float>
struct vector : public coordinates_t<Dimensions, T> {
 public:
    using value_type = T;

    [[gnu::nothrow]] vector() {
        std::fill(this->_c, this->_c + Dimensions, T());
    }

    [[gnu::nothrow]] vector(T initial_value) : vector() {
        std::fill(this->_c, this->_c + Dimensions, initial_value);
    }

    [[gnu::nothrow]] vector(vector<Dimensions, T> &&other) : vector() {
        std::copy(other._c, other._c + Dimensions, this->_c);
    }

    [[gnu::nothrow]] vector(const vector<Dimensions, T> &other) : vector() {
        std::copy(other._c, other._c + Dimensions, this->_c);
    }

    template <typename U>
    [[gnu::nothrow]] vector(const vector<Dimensions, U> &other) : vector() {
        for (size_t i = 0; i < Dimensions; ++i) {
            this->_c[i] = other._c[i];
        }
    }

    [[gnu::nothrow]] vector(std::initializer_list<T> values) : vector() {
        std::copy(values.begin(), values.end(), this->_c);
    }

    [[gnu::nothrow]] void operator+=(const vector<Dimensions, T> &other) {
        for (size_t i = 0; i < Dimensions; i++) {
            this->_c[i] += other[i];
        }
    }

    [[gnu::nothrow]] void operator-=(const vector<Dimensions, T> &other) {
        for (size_t i = 0; i < Dimensions; i++) {
            this->_c[i] -= other[i];
        }
    }

    [[gnu::nothrow]] void operator*=(const vector<Dimensions, T> &other) {
        for (size_t i = 0; i < Dimensions; i++) {
            this->_c[i] *= other[i];
        }
    }

    [[gnu::nothrow]] void operator/=(const vector<Dimensions, T> &other) {
        for (size_t i = 0; i < Dimensions; i++) {
            this->_c[i] /= other[i];
        }
    }

    template <typename U> [[gnu::nothrow]] void operator+=(const U &other) {
        for (size_t i = 0; i < Dimensions; i++) {
            this->_c[i] += other;
        }
    }

    template <typename U> [[gnu::nothrow]] void operator-=(const U &other) {
        for (size_t i = 0; i < Dimensions; i++) {
            this->_c[i] -= other;
        }
    }

    template <typename U> [[gnu::nothrow]] void operator*=(const U &other) {
        for (size_t i = 0; i < Dimensions; i++) {
            this->_c[i] *= other;
        }
    }

    template <typename U> [[gnu::nothrow]] void operator/=(const U &other) {
        for (size_t i = 0; i < Dimensions; i++) {
            this->_c[i] /= other;
        }
    }

    [[gnu::nothrow]] void operator=(const vector<Dimensions, T> &other) {
        std::copy(other._c, other._c + Dimensions, this->_c);
    }

    T &operator[](size_t index) { return this->_c[index]; }
    const T &operator[](size_t index) const { return this->_c[index]; }
};

template <size_t Dimensions, typename T>
std::ostream &operator<<(std::ostream &os,
                         const vector<Dimensions, T> &vector) {
    os << "<" << Dimensions << "d vector { ";

    for (size_t i = 0; i + 1 < Dimensions; i++) {
        os << vector[i] << ", ";
    }

    os << vector[Dimensions - 1];

    return os << " }>";
}

template <size_t Dimensions, typename T>
[[gnu::nothrow]] vector<Dimensions, T>
operator+(vector<Dimensions, T> left, const vector<Dimensions, T> &right) {
    left += right;

    return left;
}

template <size_t Dimensions, typename T>
[[gnu::nothrow]] vector<Dimensions, T>
operator-(vector<Dimensions, T> left, const vector<Dimensions, T> &right) {
    left -= right;

    return left;
}

template <size_t Dimensions, typename T>
[[gnu::nothrow]] vector<Dimensions, T>
operator*(vector<Dimensions, T> left, const vector<Dimensions, T> &right) {
    left *= right;

    return left;
}

template <size_t Dimensions, typename T>
[[gnu::nothrow]] vector<Dimensions, T>
operator/(vector<Dimensions, T> left, const vector<Dimensions, T> &right) {
    left /= right;

    return left;
}

template <size_t Dimensions, typename T, typename U>
[[gnu::nothrow]] vector<Dimensions, T> operator+(vector<Dimensions, T> left,
                                                 const U &right) {
    left += right;

    return left;
}

template <size_t Dimensions, typename T, typename U>
[[gnu::nothrow]] vector<Dimensions, T> operator-(vector<Dimensions, T> left,
                                                 const U &right) {
    left -= right;

    return left;
}

template <size_t Dimensions, typename T, typename U>
[[gnu::nothrow]] vector<Dimensions, T> operator*(vector<Dimensions, T> left,
                                                 const U &right) {
    left *= right;

    return left;
}

template <size_t Dimensions, typename T, typename U>
[[gnu::nothrow]] vector<Dimensions, T> operator/(vector<Dimensions, T> left,
                                                 const U &right) {
    left /= right;

    return left;
}

// *************************************************************************
// *                                                                       *
// *                           VECTOR FUNCTIONS                            *
// *                                                                       *
// *************************************************************************

/**
 * @brief The dot product function for two vectors in cartesian CS
 *
 * @tparam Dimensions
 * @tparam T
 * @param a
 * @param b
 * @return T scalar dot product
 */
template <size_t Dimensions, typename T>
[[gnu::nothrow]] T dot(const vector<Dimensions, T> &a,
                       const vector<Dimensions, T> &b) {
    T result = T(0);

    for (size_t i = 0; i < Dimensions; i++) {
        result += a[i] * b[i];
    }

    return result;
}

/**
 * @brief Cross product for two 3-dimensional vectors in cartesian CS
 *
 * @tparam T
 * @param a
 * @param b
 * @return vector<3, T>
 */
template <typename T>
[[gnu::nothrow]] vector<3, T> cross(const vector<3, T> &a,
                                    const vector<3, T> &b) {
    return vector<3, T>{a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z,
                        a.x * b.y - a.y * b.x};
}

template <size_t Dimensions, typename T>
[[gnu::nothrow]] inline T length(const vector<Dimensions, T> &vector) {
    return std::sqrt(dot(vector, vector));
}

template <size_t Dimensions, typename T>
[[gnu::nothrow]] inline T distance(const vector<Dimensions, T> &a,
                                   const vector<Dimensions, T> &b) {
    return length(b - a);
}

template <size_t Dimensions, typename T>
[[gnu::nothrow]] inline vector<Dimensions, T>
normalized(const vector<Dimensions, T> &vector) {
    return vector / length(vector);
}

template <size_t Dimensions, typename T>
[[gnu::nothrow]] vector<Dimensions, T>
operator*(const matrix<Dimensions, Dimensions, T> &matrix,
          const vector<Dimensions, T> &vector) {
    mesp2::vector<Dimensions, T> result;

    for (size_t i = 0; i < Dimensions; i++) {
        T sum = T(0);

        for (size_t dimension = 0; dimension < Dimensions; dimension++) {
            sum += vector[dimension] * matrix[dimension][i];
        }

        result[i] = sum;
    }

    return result;
}

template <size_t Dimensions, typename T>
[[gnu::nothrow]] vector<Dimensions, T>
operator*(const vector<Dimensions, T> &vector,
          const matrix<Dimensions, Dimensions, T> &matrix) {
    mesp2::vector<Dimensions, T> result;

    for (size_t i = 0; i < Dimensions; i++) {
        T sum = T(0);

        for (size_t dimension = 0; dimension < Dimensions; dimension++) {
            sum += vector[dimension] * matrix[i][dimension];
        }

        result[i] = sum;
    }

    return result;
}

} // namespace mesp2
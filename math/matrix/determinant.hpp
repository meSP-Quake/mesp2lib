/**
 * matrix/determinant.hpp - meSP2's linear algebra submodule for calculating
 *                          matrix determinant.
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

/**
 *~************************************************************************
 *~*                                                                      *
 *~*                                BRIEF                                 *
 *~*                                                                      *
 *~************************************************************************
 *
 * ~ metaq3, 12 Jun 2025
 *
 *   I've created this file with purpose of fixing a problem when compiling
 * matrix's determinant calculation function - matrix::getDeterminant().
 *   As naive solution, I'm using recursive approach here, so getDeterminant
 * function calls itself for lower dimensional matrices, meaning I need to
 * setup recursive base, i.e. make non-recursive implementation for 1- and
 * 2-dimensional matrices.
 *   The problem is that I cannot use constexpr if statement here, as
 * compiler anyways checks else branch and gets stuck in infinite loop,
 * spoiling at me errors with dumb face about matrices with negative
 * dimensions.
 *   So... I've made additional functions with two extra overloads to setup
 * recursion base and it worked. Since I'm not keen on creating big files
 * with over 1k lines of spaghetti code, I've separated this into two files.
 *
 */

#include <math/matrix/matrix.hpp>

namespace mesp2 {

template <size_t n, typename T>
constexpr T getMatrixDeterminant(const matrix<n, n, T> &matrix) {
    static_assert(n > 0);

    T det = T(0);

    // Used for calculating matrix's minors
    mesp2::matrix<n - 1, n - 1, T> copy = {};

    // Copy matrix contents

    for (size_t i = 0; i < n - 1; ++i) {
        for (size_t j = 0; j < n - 1; ++j) {
            copy[i][j] = matrix[i][j];
        }
    }

    // Calculate the determinant

    for (size_t i = 0; i < n - 1; ++i) {
        // Swap rows

        for (size_t j = 0; j < n - 1; ++j) {
            copy[j][i] = matrix[j][n - 1];
        }

        // Sign is not present because of row swap in the code above.
        // Determinant of the copy matrix ( minor ) has opposite sign,
        // so they're cancelling out into -1.
        det -= matrix[n - 1][i] * getMatrixDeterminant(copy);

        // Rollback matrix

        for (size_t j = 0; j < n - 1; ++j) {
            copy[j][i] = matrix[j][i];
        }
    }

    det += matrix[n - 1][n - 1] * getMatrixDeterminant(copy);

    return det;
}

template <typename T>
constexpr T getMatrixDeterminant(const matrix<2, 2, T> &matrix) {
    return matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
}

template <typename T>
constexpr T getMatrixDeterminant(const matrix<1, 1, T> &matrix) {
    return matrix[0][0];
}

template <size_t w, size_t h, typename T>
T matrix<w, h, T>::getDeterminant() const {
    static_assert(w == h,
                  "Matrix must be square to calculate it's determinant.");

    return getMatrixDeterminant(*this);
}

}; // namespace mesp2
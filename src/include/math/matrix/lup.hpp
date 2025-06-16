/**
 * matrix/lup.hpp - meSP2's linear algebra submodule with LUP matrix
 *                  decomposition.
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

#include <math/matrix/matrix.hpp>

namespace mesp2 {

/**
 * @brief Performs LUP decomposition for given square matrix.
 *
 * @tparam n
 * @tparam T
 * @param matrix
 * @return std::tuple<matrix<n, n, T>, matrix<n, n, T>, bool> - triple of matrix
 * C, equal to L + U ( you can restore L and U knowing they're triangular ),
 * permutation matrix and determinant sign.
 */
template <size_t n, typename T>
std::tuple<matrix<n, n, T>, matrix<n, n, T>, bool>
LUPDecompose(mesp2::matrix<n, n, T> matrix) {
    mesp2::matrix<n, n, T> permutation = mesp2::matrix<n, n, T>::identity();

    bool detSign = false;

    for (size_t i = 0; i < n; ++i) {
        size_t pivot = i;

        for (size_t row = i + 1; row < n; ++row) {
            if (std::abs(matrix[row][i]) > std::abs(matrix[pivot][i])) {
                pivot = row;
            }
        }

        if (pivot != i) {
            // Swapping rows changes matrix determinant sign
            for (size_t j = 0; j < n; ++j) {
                std::swap(matrix[i][j], matrix[pivot][j]);
                std::swap(permutation[i][j], permutation[pivot][j]);
            }

            detSign = !detSign;
        }

        if (matrix[i][i] == T(0)) {
            return {mesp2::matrix<n, n, T>(T(0)), permutation, detSign};
        }

        // SIMD-friendly code

        for (size_t j = i + 1; j < n; ++j) {
            matrix[j][i] /= matrix[i][i];
        }

        for (size_t j = i + 1; j < n; ++j) {
            const T coef = matrix[j][i];

            for (size_t k = i + 1; k < n; ++k) {
                matrix[j][k] -= coef * matrix[i][k];
            }
        }
    }

    return {matrix, permutation, detSign};
}

} // namespace mesp2
/**
 * matrix/inverse.hpp - meSP2's linear algebra submodule for calculating
 *                      inverse matrix.
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
 * @brief Calculates matrix algebraic complement at given row and column.
 *
 * @tparam n - square matrix size
 * @tparam T
 * @param matrix
 * @param row - minor's row
 * @param column - minor's column
 * @return T
 *
 * Basically, algebraic complement is just matrix minor, multiplied by it's
 * sign, which is M * (-1) ** (row + column), where M - minor.
 */
template <size_t n, typename T>
constexpr T calculateMatrixAlgebraicComplement(const matrix<n, n, T> &matrix,
                                               size_t row, size_t column) {
    mesp2::matrix<n, n, T> copy = matrix;

    // No need to copy matrix's content into minor matrix with
    // ommitting corresponding row and column. We can just zero
    // out them and set element at [row][column] to one, so
    // matrix's determinant is actually it's minor there.
    //
    // It's not that hard to show that:
    // you can see it if you choose "zeroed out" row/column and
    // calculate the determinant. All the coefficients, except
    // the only one, are zero, so other minors are zeroed out.
    // For more information visit theese links:
    // https://www.mathsisfun.com/algebra/matrix-determinant.html
    // https://en.wikipedia.org/wiki/Minor_(linear_algebra)
    //
    // I haven't benchmarked this solution against common
    // implementation, but maybe in the future I will...

    for (size_t i = 0; i < n; ++i) {
        copy[row][i] = copy[i][column] = T(0);
    }

    copy[row][column] = T(1);

    return copy.getDeterminant();
}

template <size_t h, size_t w, typename T>
matrix<w, w, T> matrix<h, w, T>::makeInverse() const {
    static_assert(w == h, "Matrix must be square to make inverse copy of it.");

    auto [matrix, permutation, _] = LUPDecompose(*this);

    mesp2::matrix<w, w, T> &L = matrix;
    mesp2::matrix<w, w, T> U = mesp2::matrix<w, w, T>::identity();

    for (size_t i = 0; i < w; ++i) {
        for (size_t j = i; j < w; ++j) {
            std::swap(matrix[i][j], U[i][j]);
        }
    }

    for (size_t i = 0; i < w; ++i) {
        U[i][i] = T(1) / U[i][i];

        for (size_t j = i + 1; j < w; ++j) {
            T newValue = T(0);

            for (size_t k = 0; k < j; ++k) {
                newValue -= U[i][k] * U[k][j];
            }

            U[i][j] = newValue / U[j][j];
        }

        for (size_t j = 0; j < i; ++j) {
            T newValue = T(0);

            for (size_t k = 0; k < i; ++k) {
                newValue -= L[k][j] * L[i][k];
            }

            L[i][j] = newValue / L[i][i];
        }
    }

    return U * L * permutation;
}

}; // namespace mesp2
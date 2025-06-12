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

    // You won't speed function asymptotically if you inline this function
    // and optimize it in such a way, that matrix copies only once and
    // restores each time it's used. You'll wave O(n^2) operations, but
    // matrix.getDeterminant() function is O(n^3).
    return copy.getDeterminant();
}

template <size_t w, size_t h, typename T>
matrix<w, w, T> matrix<w, h, T>::makeInverse() const {
    static_assert(w == h, "Matrix must be square to make inverse copy of it.");

    matrix<w, w, T> out = {};

    // Assuming determinant is not zero
    const T det = this->getDeterminant();

    for (size_t row = 0; row < w; ++row) {
        for (size_t column = 0; column < w; ++column) {
            // Values are transposed automatically, so swapped row and column in
            // a function call are not a mistake.
            out[row][column] =
                calculateMatrixAlgebraicComplement(*this, column, row);
        }
    }

    // You could speed it up by calculating inverse determinant and doing
    // multiplication instead of division, which is faster in floats, but
    // keep in mind, that this could produce incorrect results with integer
    // matrices ( e.g. mesp2::matrix<n, n, int64_t> ).
    return out / det;
}

}; // namespace mesp2
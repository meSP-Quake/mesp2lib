/**
 * matrix/algebra.hpp - meSP2's linear algebra submodule for matrix
 *                      algebra.
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

// *************************************************************************
// *                                                                       *
// *                                ALGEBRA                                *
// *                                                                       *
// *************************************************************************

template <size_t w, size_t h, typename T> T matrix<w, h, T>::getTrace() const {
    static_assert(w == h, "Matrix must be square to calculate it's trace.");

    T out = T(0);

    for (size_t i = 0; i < w; ++i) {
        out += (*this)[i][i];
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

    matrix<s, s, T> out = matrix<s, s, T>::identity();
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

}; // namespace mesp2
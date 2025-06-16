/**
 * interpolate.hpp - meSP2's linear algebra submodule for making interpolations.
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

namespace mesp2 {

/**
 * @brief Linear interpolation
 *
 * @param left - from
 * @param right - to
 * @param x - interpolation coefficient
 * @return T - interpolated value
 */
template <typename T, typename CoefType>
constexpr inline T lerp(const T &left, const T &right, const CoefType &x) {
    return left + (right - left) * x;
}

/**
 * @brief Linear interpolation between y0 and y1, where linear coefficient i
 * satisfies lerp(x0, x1, i) = x
 *
 * @param x0, x1, x - some values to get linear coefficient from
 * @param y0, y1 - values to interpolate between using linear coefficient
 * @return T - interpolated value
 */
template <typename T, typename U>
constexpr inline U lerp(const T &x0, const T &x1, const T &x, const U &y0,
                        const U &y1) {
    return y0 + (y1 - y0) * (x - x0) / (x1 - x0);
}

}; // namespace mesp2
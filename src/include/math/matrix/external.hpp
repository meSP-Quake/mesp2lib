/**
 * matrix/external.hpp - meSP2's linear algebra submodule for external
 *                       matrix functions.
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
#include <math/matrix/matrix.hpp>

namespace mesp2 {

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

    mesp2::matrix<Dim, Dim, T> result = mesp2::matrix<Dim, Dim, T>::identity();

    result[dim1][dim1] = c;
    result[dim1][dim2] = -s;
    result[dim2][dim1] = s;
    result[dim2][dim2] = c;

    return result;
}

} // namespace mesp2

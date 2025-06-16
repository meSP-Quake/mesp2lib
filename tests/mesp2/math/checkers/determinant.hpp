#include <math/matrix.hpp>

namespace mesp2 {
namespace test {

// **************************************************************
// *                                                            *
// *            MATRIX DETERMINANT CORRECT ALGORITHM            *
// *                                                            *
// **************************************************************
// This is what I think 100% correct algorithm, suitable for checking
// other algorithms in stress tests.

template <typename T>
constexpr T getMatrixDeterminant(const matrix<3, 3, T> &matrix) {
    return matrix[0][0] * matrix[1][1] * matrix[2][2] -
           matrix[0][0] * matrix[1][2] * matrix[2][1] -
           matrix[0][1] * matrix[1][0] * matrix[2][2] +
           matrix[0][1] * matrix[1][2] * matrix[2][0] +
           matrix[0][2] * matrix[1][0] * matrix[2][1] -
           matrix[0][2] * matrix[1][1] * matrix[2][0];
}

template <typename T>
constexpr T getMatrixDeterminant(const matrix<2, 2, T> &matrix) {
    return matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
}

template <typename T>
constexpr T getMatrixDeterminant(const matrix<1, 1, T> &matrix) {
    return matrix[0][0];
}

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
        det -= matrix[n - 1][i] * mesp2::test::getMatrixDeterminant(copy);

        // Rollback matrix

        for (size_t j = 0; j < n - 1; ++j) {
            copy[j][i] = matrix[j][i];
        }
    }

    det += matrix[n - 1][n - 1] * mesp2::test::getMatrixDeterminant(copy);

    return det;
}

} // namespace test
} // namespace mesp2
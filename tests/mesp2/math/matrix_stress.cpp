#include "checkers/determinant.hpp"
#include <gtest/gtest.h>
#include <math/matrix.hpp>
#include <math/vector.hpp>
#include <random>

// **************************************************************
// *                                                            *
// *          MATRIX PERFOMANCE AND CORRECTNESS TESTS           *
// *                                                            *
// **************************************************************

TEST(MatrixStressTest, DeterminantWithFloats) {
    static constexpr size_t matrixSize = 8;

    std::mt19937_64 random(199999);

    mesp2::matrix<matrixSize, matrixSize, double> m;

    for (uint32_t test = 0; test < 1000; ++test) {
        // Fillup matrix
        for (uint32_t i = 0; i < matrixSize; ++i) {
            for (uint32_t j = 0; j < matrixSize; ++j) {
                m[i][j] =
                    (double(random()) / double(random.max()) - 0.5) * 2.0 * 4.0;
            }
        }

        // Calculating determinant with absolute correctness is slow,
        // usually taking exactly 2s on my PC...
        double correctDeterminant = mesp2::test::getMatrixDeterminant(m);

        // ...as well as optimized algorithm takes only 4ms in total...
        double calculatedDeterminant = m.getDeterminant();

        // ...so it's fine for this test case to run 1-4 seconds in total.

        // Please, do not even consider changing constant 1e-9 to something more
        // relaxed, like 1e-8 or 1e-7. This would involve significant
        // calculation error when doing a lot of operations.
        EXPECT_NEAR(correctDeterminant, calculatedDeterminant, 1e-9);
    }
}

TEST(MatrixStressTest, InverseWithFloats) {
    static constexpr size_t matrixSize = 4;

    std::mt19937_64 random(199999);

    mesp2::matrix<matrixSize, matrixSize, double> m(0.0);

    const mesp2::matrix identity =
        mesp2::matrix<matrixSize, matrixSize, double>::identity();

    for (uint32_t test = 0; test < 1000; ++test) {
        // Fillup matrix

        while (std::abs(m.getDeterminant()) < 1e-4) {
            for (uint32_t i = 0; i < matrixSize; ++i) {
                for (uint32_t j = 0; j < matrixSize; ++j) {
                    m[i][j] = (double(random()) / double(random.max()) - 0.5) *
                              2.0 * 4.0;
                }
            }
        }

        mesp2::matrix<matrixSize, matrixSize, double> inverse = m.makeInverse();

        // If B is an inverse matrix of A, then AB = BA = E, where E is identity
        // matrix.

        mesp2::matrix<matrixSize, matrixSize, double> shouldBeIdentity =
            inverse * m;

        // So check A * B is in fact identity matrix
        for (uint32_t i = 0; i < matrixSize; ++i) {
            for (uint32_t j = 0; j < matrixSize; ++j) {
                EXPECT_NEAR(shouldBeIdentity[i][j], identity[i][j], 1e-9);
            }
        }
    }
}
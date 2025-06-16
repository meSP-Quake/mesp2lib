#include <gtest/gtest.h>
#include <math/matrix.hpp>
#include <math/vector.hpp>

TEST(MatrixTest, DefaultConstructible) {
    mesp2::matrix<3, 6, double> m;

    for (size_t row = 0; row < 3; ++row) {
        for (size_t column = 0; column < 6; ++column) {
            EXPECT_EQ(m[row][column], double(0));
        }
    }
}

TEST(MatrixTest, ScalarConstructible) {
    mesp2::matrix<3, 6, double> m(1.0);

    for (size_t row = 0; row < 3; ++row) {
        for (size_t column = 0; column < 6; ++column) {
            EXPECT_EQ(m[row][column], 1.0);
        }
    }
}

TEST(MatrixTest, InitializerListConstructible) {
    mesp2::matrix<2, 3, double> m = {1, 2, 3, 4, 5, 6};

    for (size_t i = 0; i < 2; ++i) {
        for (size_t j = 0; j < 3; ++j) {
            EXPECT_EQ(m[i][j], i * 3 + j + 1);
        }
    }
}

TEST(MatrixTest, Arithmetic) {
    {
        mesp2::matrix<3, 3, double> a = {4, 7, 1, 8, 0, -4, 6, 1, 0};
        mesp2::matrix<3, 3, double> b = {1, 9, 0, 6, -5, 3, 3, 2, 1};

        mesp2::matrix<3, 3, double> result = a + b;

        for (size_t i = 0; i < 3; ++i) {
            for (size_t j = 0; j < 3; ++j) {
                EXPECT_EQ(result[i][j], a[i][j] + b[i][j]);
            }
        }
    }

    {
        mesp2::matrix<3, 4, double> a = {4, 7, 1, 5, 8, 0, -4, 2, 6, 1, 0, 5};
        mesp2::matrix<4, 3, double> b = {8, 9, 9, 3, 0, -2, -1, -4, 3, 7, 1, 0};

        a *= 0.5;
        b *= 2.0;

        mesp2::matrix<3, 3, double> abSample = {87, 37, 25, 82, 90,
                                                60, 86, 59, 52};
        mesp2::matrix<3, 3, double> ab = a * b;

        for (size_t i = 0; i < 3; ++i) {
            for (size_t j = 0; j < 3; ++j) {
                EXPECT_EQ(abSample[i][j], ab[i][j]);
            }
        }

        mesp2::matrix<4, 4, double> baSample = {
            158, 65, -28, 103, 0, 19, 3, 5, -18, -4, 15, 2, 36, 49, 3, 37};
        mesp2::matrix<4, 4, double> ba = b * a;

        for (size_t i = 0; i < 4; ++i) {
            for (size_t j = 0; j < 4; ++j) {
                EXPECT_EQ(baSample[i][j], ba[i][j]);
            }
        }
    }
}

TEST(MatrixTest, EqualityComparable) {
    mesp2::matrix<2, 3, double> m1 = {1, 2, 3, 4, 5, 6};
    mesp2::matrix<2, 3, double> m2 = {1, 2, 3, 4, 5, 6};
    mesp2::matrix<2, 3, double> m3 = {1, 2, 3.1, 4, 5, 6};

    EXPECT_TRUE(m1 == m2);
    EXPECT_FALSE(m1 != m2);
    EXPECT_FALSE(m1 == m3);
    EXPECT_TRUE(m1 != m3);
}

template <typename T> void checkDeterminant() {
    std::vector<std::pair<mesp2::matrix<4, 4, T>, int64_t>> matrices4x4 = {
        {{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16}, 0},
        {{4, 7, 1, -3, 8, 0, -4, 2, 6, 1, 0, 3, 2, 2, 1, 2}, -248},
        {{2, 1, 3, 1, 0, 5, 1, 4, 0, 0, 3, 5, 0, 0, 0, 4}, 120},
        {{8, 2, 7, 1, 3, 5, 2, 4, 4, 1, 3, 5, 16, 4, 14, 2}, 0},
        // LU-fail test group
        {{1, 2, 3, 0, 2, 4, 5, 0, 1, 3, 4, 0, 0, 0, 0, 1}, 1},
        {{1, 2, 6, 7, 1, 5, -6, -4, 2, 4, 12, 4, -2, -3, 2, 8}, 540}};

    std::vector<std::pair<mesp2::matrix<3, 3, T>, int64_t>> matrices3x3 = {
        // 3x3 version of LU-fail test
        {{1, 2, 3, 2, 4, 5, 1, 3, 4}, 1}};

    for (auto [mat, det] : matrices4x4) {
        EXPECT_NEAR(mat.getDeterminant(), det, 1e-10);
    }

    for (auto [mat, det] : matrices3x3) {
        EXPECT_NEAR(mat.getDeterminant(), det, 1e-10);
    }
}

TEST(MatrixTest, Determinant) {
    checkDeterminant<int64_t>();
    checkDeterminant<float>();
    checkDeterminant<double>();
}

TEST(MatrixTest, Trace) {
    std::vector<std::pair<mesp2::matrix<4, 4, double>, int64_t>> matrices4x4 = {
        {{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16}, 34},
        {{4, 7, 1, -3, 8, 0, -4, 2, 6, 1, 0, 3, 2, 2, 1, 2}, 6},
        {{-1, 0, 0, 0, 0, -1, 0, 0, 0, 0, -1, 0, 0, 0, 0, -1}, -4}};

    for (auto [mat, trace] : matrices4x4) {
        EXPECT_DOUBLE_EQ(mat.getTrace(), trace);
    }
}

TEST(MatrixTest, Transposition) {
    std::vector<mesp2::matrix<4, 4, double>> matrices4x4 = {
        {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16},
        {4, 7, 1, -3, 8, 0, -4, 2, 6, 1, 0, 3, 2, 2, 1, 2},
        {-1, 0, 0, 0, 0, -1, 0, 0, 0, 0, -1, 0, 0, 0, 0, -1}};

    for (auto &mat : matrices4x4) {
        mesp2::matrix<4, 4, double> transposed = mat.makeTransposed();

        for (size_t i = 0; i < 4; ++i) {
            for (size_t j = 0; j < 4; ++j) {
                EXPECT_EQ(transposed[i][j], mat[j][i]);
            }
        }
    }
}
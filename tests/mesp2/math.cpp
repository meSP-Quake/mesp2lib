#include <gtest/gtest.h>
#include <math/matrix.hpp>
#include <math/vector.hpp>

TEST(MathTests, VectorMatrixMultiplication) {
    mesp2::vector<2, float> v = {9, 4};
    mesp2::matrix<2, 2, float> m = {7, -2, 5, 3};

    mesp2::vector<2, float> vm = {55, 57};
    mesp2::vector<2, float> mv = {83, -6};

    EXPECT_TRUE(v * m == vm);
    EXPECT_TRUE(m * v == mv);
}
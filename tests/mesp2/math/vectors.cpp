#include <gtest/gtest.h>
#include <math/matrix.hpp>
#include <math/vector.hpp>

TEST(VectorTests, DefaultConstructible) {
    mesp2::vector<4, float> vec;

    EXPECT_EQ(vec[0], 0.f);
    EXPECT_EQ(vec[1], 0.f);
    EXPECT_EQ(vec[2], 0.f);
    EXPECT_EQ(vec[3], 0.f);
}

TEST(VectorTests, ScalarConstructible) {
    mesp2::vector<4, float> vec(1.0f);

    EXPECT_EQ(vec[0], 1.f);
    EXPECT_EQ(vec[1], 1.f);
    EXPECT_EQ(vec[2], 1.f);
    EXPECT_EQ(vec[3], 1.f);
}

TEST(VectorTests, InitializerListConstructible) {
    mesp2::vector<4, int64_t> vec = {1, 2, 3, 4};

    EXPECT_EQ(vec[0], 1);
    EXPECT_EQ(vec[1], 2);
    EXPECT_EQ(vec[2], 3);
    EXPECT_EQ(vec[3], 4);
}

TEST(VectorTests, OtherTypeCopyConstructible) {
    mesp2::vector<4, int64_t> vec = {1, 2, 0, 4};
    mesp2::vector<4, float> vec2(vec);

    EXPECT_EQ(vec2[0], 1);
    EXPECT_EQ(vec2[1], 2);
    EXPECT_EQ(vec2[2], 0);
    EXPECT_EQ(vec2[3], 4);
}

TEST(VectorTests, EqualityComparable) {
    mesp2::vector<7, int64_t> vec = {8, 9, 0, 4, 6, 5, 2};
    mesp2::vector<7, int64_t> vec2 = {8, 9, 0, 4, 6, 5, 2};
    mesp2::vector<7, int64_t> vec3 = {8, 9, 1, 4, 6, 5, 2};
    mesp2::vector<7, int64_t> vec4 = {8, 9, 0, 4, 6, 5, -1};

    EXPECT_TRUE(vec == vec2);
    EXPECT_TRUE(vec == vec2);
    EXPECT_FALSE(vec == vec3);
    EXPECT_FALSE(vec == vec4);

    EXPECT_FALSE(vec != vec2);
    EXPECT_FALSE(vec != vec2);
    EXPECT_TRUE(vec != vec3);
    EXPECT_TRUE(vec != vec4);
}

TEST(VectorTests, CoordinateAccessable) {
    mesp2::vector<3, int64_t> vec3 = {1, 2, 3};
    mesp2::vector<4, int64_t> vec4 = {1, 2, 3, 4};

    EXPECT_EQ(vec3.x, 1);
    EXPECT_EQ(vec3.y, 2);
    EXPECT_EQ(vec3.z, 3);
    EXPECT_EQ(vec4.x, 1);
    EXPECT_EQ(vec4.y, 2);
    EXPECT_EQ(vec4.z, 3);
    EXPECT_EQ(vec4.w, 4);
}

template <typename T> void CheckVectorArithmetic() {
    const std::vector<std::pair<mesp2::vector<3, T>, mesp2::vector<3, T>>>
        sampleVectors = {{{5, 2, 7}, {1, 2, 3}},
                         {{4, 8, 2}, {3, 3, 6}},
                         {{9, 8, 1}, {0, 2, 0}}};

    {
        // vector with vector operations

        for (auto [v1, v2] : sampleVectors) {
            // addition result
            mesp2::vector<3, T> v_ar = {T(v1[0] + v2[0]), T(v1[1] + v2[1]),
                                        T(v1[2] + v2[2])};

            // subtraction result
            mesp2::vector<3, T> v_sr = {T(v1[0] - v2[0]), T(v1[1] - v2[1]),
                                        T(v1[2] - v2[2])};

            // multiplication result
            mesp2::vector<3, T> v_mr = {T(v1[0] * v2[0]), T(v1[1] * v2[1]),
                                        T(v1[2] * v2[2])};

            EXPECT_TRUE(v1 * v2 == v2 * v1);
            EXPECT_TRUE(v1 * v2 == v_mr);
            EXPECT_TRUE(v1 + v2 == v2 + v1);
            EXPECT_TRUE(v1 + v2 == v_ar);
            EXPECT_TRUE(v1 - v2 == v_sr);
        }
    }

    {
        // vector with scalar operations

        for (auto [v1, _] : sampleVectors) {
            // addition result
            mesp2::vector<3, T> v_ar = {T(v1[0] + 3), T(v1[1] + 3),
                                        T(v1[2] + 3)};

            // subtraction result
            mesp2::vector<3, T> v_sr = {T(v1[0] - 3), T(v1[1] - 3),
                                        T(v1[2] - 3)};

            // multiplication result
            mesp2::vector<3, T> v_mr = {T(v1[0] * 3), T(v1[1] * 3),
                                        T(v1[2] * 3)};

            // division result
            mesp2::vector<3, T> v_dr = {T(v1[0] / 3), T(v1[1] / 3),
                                        T(v1[2] / 3)};

            // vector is multiplied component-wise
            EXPECT_TRUE(v1 * T(3) == v_mr);
            EXPECT_TRUE(v1 / T(3) == v_dr);

            // scalar is added component-wise
            EXPECT_TRUE(v1 + T(3) == v_ar);
            EXPECT_TRUE(v1 - T(3) == v_sr);
        }
    }
}

TEST(VectorTests, CorrectArithmeticWithAllTypes) {
    // Check for basic ring axioms

    CheckVectorArithmetic<float>();
    CheckVectorArithmetic<double>();
    CheckVectorArithmetic<int32_t>();
    CheckVectorArithmetic<int64_t>();
    CheckVectorArithmetic<uint64_t>();
    CheckVectorArithmetic<uint32_t>();
}

TEST(VectorTests, DotProduct) {
    /*
    Dot product can be implemented many different times. As meSP2 reserves
    functionality for abstract multidimensional spaces and linear algebra,
    we need to check basic dot product axioms.
    */

    const std::vector<
        std::tuple<mesp2::vector<3, double>, mesp2::vector<3, double>,
                   mesp2::vector<3, double>>>
        sampleVectors = {{{5, 2, 7}, {1, 2, 3}, {4, 0, 0}},
                         {{4, 8, 2}, {3, 3, 6}, {0, 5, 6}},
                         {{9, 8, 1}, {0, 2, 0}, {1, 0, 3}}};

    // Dot product is a symmetrical operator
    {
        for (auto [v1, v2, _] : sampleVectors) {
            EXPECT_DOUBLE_EQ(mesp2::dot(v1, v2), mesp2::dot(v2, v1));
        }
    }

    // Dot product is a linear operator

    // (1) dot(a + b, c) = dot(a, c) + dot(b, c)
    {
        for (auto [v1, v2, v3] : sampleVectors) {
            EXPECT_DOUBLE_EQ(mesp2::dot(v1 + v2, v3),
                             mesp2::dot(v1, v3) + mesp2::dot(v2, v3));
            EXPECT_DOUBLE_EQ(mesp2::dot(v1, v3 + v2),
                             mesp2::dot(v1, v3) + mesp2::dot(v1, v2));
        }
    }

    // (2) dot(a * v1, v2) = a * dot(v1, v2)
    {
        for (auto [v1, v2, _] : sampleVectors) {
            constexpr double alpha = 5.0;

            EXPECT_DOUBLE_EQ(mesp2::dot(v1 * alpha, v2),
                             alpha * mesp2::dot(v1, v2));
            EXPECT_DOUBLE_EQ(mesp2::dot(v1, v2 * alpha),
                             alpha * mesp2::dot(v1, v2));
        }
    }

    // Dot product is always less than lengths of vectors:
    // |dot(a, b)| <= |a| * |b|

    {
        for (auto [v1, v2, _] : sampleVectors) {
            EXPECT_LT(std::abs(mesp2::dot(v1, v2)),
                      mesp2::length(v1) * mesp2::length(v2));
        }
    }
}

TEST(VectorTests, CrossProduct) {
    const std::vector<
        std::pair<mesp2::vector<3, double>, mesp2::vector<3, double>>>
        sampleVectors = {{{5, 2, 7}, {1, 2, 3}},
                         {{4, 8, 2}, {3, 3, 6}},
                         {{9, 8, 1}, {0, 2, 0}}};

    // Cross product is perpendicular to other two. This could be shown
    // with dot(a, cross(a, b)) == dot(b, cross(a, b)) == 0
    {
        for (auto [v1, v2] : sampleVectors) {
            EXPECT_DOUBLE_EQ(mesp2::dot(v1, mesp2::cross(v1, v2)), 0);
            EXPECT_DOUBLE_EQ(mesp2::dot(v2, mesp2::cross(v1, v2)), 0);
        }
    }
}

TEST(VectorTests, Normalize) {
    const std::vector<mesp2::vector<3, double>> sampleVectors = {
        {5, 2, 7}, {1, 2, 3}, {4, 8, 2}, {3, 3, 6}, {9, 8, 1}, {0, 2, 0}};

    // Cross product is perpendicular to other two. This could be shown
    // with dot(a, cross(a, b)) == dot(b, cross(a, b)) == 0
    {
        for (auto v : sampleVectors) {
            EXPECT_DOUBLE_EQ(mesp2::length(mesp2::normalized(v)), 1);
        }
    }
}

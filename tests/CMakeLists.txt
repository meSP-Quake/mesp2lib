cmake_minimum_required(VERSION 3.22)
project(meSP2 LANGUAGES CXX)

option(MESP2_SOURCE_DIRECTORY "meSP2 location" "${PROJECT_SOURCE_DIR}/../src")

SET(CMAKE_INSTALL_RPATH "${PROJECT_SOURCE_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_C_COMPILER clang)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)
set(CMAKE_BUILD_TYPE Debug)

add_compile_options(-Wextra -fsanitize=address -fsanitize=undefined -O0 -g)
add_link_options(-Wextra -fsanitize=address -fsanitize=undefined -O0 -g)

find_package(GTest REQUIRED)
include_directories(.)
include_directories(${MESP2_SOURCE_DIRECTORY}/include)
include_directories(${GTEST_INCLUDE_DIRS})

enable_testing()

add_executable(mesp2_test
        mesp2/math/vectors.cpp
        mesp2/math/matrix.cpp
        mesp2/math/matrix_stress.cpp
        mesp2/math.cpp)

add_test(mesp2_test ${PROJECT_SOURCE_DIR}/bin)

target_link_libraries(mesp2_test GTest::gtest_main)
target_link_libraries(mesp2_test Threads::Threads ${GTEST_LIBRARIES} ${GMOCK_BOTH_LIBRARIES})

install (
        TARGETS mesp2_test
        RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/bin
)

include(GoogleTest)
gtest_discover_tests(mesp2_test)
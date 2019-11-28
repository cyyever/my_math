/*!
 * \file rational_test.cpp
 *
 * \brief 测试rational类
 */
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include "../src/rational.hpp"
using namespace std;
using namespace cyy::math;

TEST_CASE("from_string") {
  for (auto const &str : {"1/2", "-1/2", "5"}) {
    auto a = rational(str);
    CHECK_EQ(str, a.to_string());
  }
}
TEST_CASE("simplify") {
  CHECK_EQ(rational(-3, 7).simplify(), rational(-3, 7));
  CHECK_EQ(rational(-14, 77).simplify(), rational(-2, 11));
  CHECK_EQ(rational(-5, 5).simplify(), rational(-1, 1));
}
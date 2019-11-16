/*
 *	程序名：integer_test.cpp
 *	作者：陈源源
 *	日期：2016-03-28
 *	功能：测试integer类
 */
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include "../src/integer.hpp"
using namespace std;
using namespace cyy::math;

TEST_CASE("from_string") {
  SUBCASE("valid_string") {
    std::vector<std::string> strs{"1", "-1", "0", to_string(UINT64_MAX),
                                  to_string(INT64_MIN)};
    for (auto const &str : strs) {
      auto a = integer(str);
      CHECK_EQ(str, a.to_string());
    }
  }

  SUBCASE("invalid_string") {
    for (auto const &str : {"aaa", "0123"}) {
      CHECK_THROWS(auto a = integer(str));
    }
  }
}
TEST_CASE("+") {
  SUBCASE("signed") {
    for (auto a = static_cast<int64_t>(INT32_MIN) - 1;
         a < static_cast<int64_t>(INT32_MIN) + 1; a++) {
      for (auto b = static_cast<int64_t>(INT32_MIN) - 1;
           b < static_cast<int64_t>(INT32_MIN) + 1; b++) {
        CHECK_EQ(integer(a) + integer(b), (a + b));
      }
    }
  }
  SUBCASE("unsigned") {
    for (auto a = static_cast<uint64_t>(UINT32_MAX) - 1;
         a < static_cast<uint64_t>(UINT32_MAX) + 1; a++) {
      for (auto b = static_cast<uint64_t>(UINT32_MAX) - 1;
           b < static_cast<uint64_t>(UINT32_MAX) + 1; b++) {
        CHECK_EQ(integer(a) + integer(b), (a + b));
      }
    }
  }
}

TEST_CASE("-") {
  SUBCASE("signed") {
    for (auto a = static_cast<int64_t>(INT32_MIN) - 1;
         a < static_cast<int64_t>(INT32_MIN) + 1; a++) {
      for (auto b = static_cast<int64_t>(INT32_MIN) - 1;
           b < static_cast<int64_t>(INT32_MIN) + 1; b++) {
        CHECK_EQ(integer(a) - integer(b), (a - b));
      }
    }
  }
  SUBCASE("unsigned") {
    for (auto a = static_cast<int64_t>(UINT32_MAX) - 1;
         a < static_cast<int64_t>(UINT32_MAX) + 1; a++) {
      for (auto b = static_cast<int64_t>(UINT32_MAX) - 1;
           b < static_cast<int64_t>(UINT32_MAX) + 1; b++) {
        CHECK_EQ(integer(a) - integer(b), (a - b));
      }
    }
  }
}

TEST_CASE("*") {
  SUBCASE("signed") {
    for (auto a = static_cast<int64_t>(INT32_MIN) - 1;
         a < static_cast<int64_t>(INT32_MIN) + 1; a++) {
      for (auto b = static_cast<int64_t>(INT32_MIN) - 1;
           b < static_cast<int64_t>(INT32_MIN) + 1; b++) {
        CHECK_EQ(integer(a) * integer(b), (a * b));
      }
    }
  }
  SUBCASE("unsigned") {
    for (auto a = static_cast<uint64_t>(UINT32_MAX) - 1;
         a < static_cast<uint64_t>(UINT32_MAX) + 1; a++) {
      auto b = static_cast<uint64_t>(UINT32_MAX) - 1;
      CHECK_EQ(integer(a) * integer(b), (a * b));
    }
  }
}

TEST_CASE("/") {
  SUBCASE("signed") {
    for (auto a = static_cast<int64_t>(INT32_MIN) - 1;
         a < static_cast<int64_t>(INT32_MIN) + 1; a++) {
      CHECK_EQ(integer(a) /= static_cast<uint32_t>(10),
               a / static_cast<uint32_t>(10));
    }
  }
  SUBCASE("unsigned") {
    for (auto a = static_cast<int64_t>(UINT32_MAX) - 1;
         a < static_cast<int64_t>(UINT32_MAX) + 1; a++) {
      CHECK_EQ(integer(a) /= static_cast<uint32_t>(10),
               a / static_cast<uint32_t>(10));
    }
  }
}

TEST_CASE("%") {
  SUBCASE("signed") {
    for (auto a = static_cast<int64_t>(INT32_MIN) - 1;
         a < static_cast<int64_t>(INT32_MIN) + 1; a++) {
      CHECK_EQ(integer(a) % static_cast<uint32_t>(10),
               a % static_cast<uint32_t>(10));
    }
  }
  SUBCASE("unsigned") {
    for (auto a = static_cast<int64_t>(UINT32_MAX) - 1;
         a < static_cast<int64_t>(UINT32_MAX) + 1; a++) {
      CHECK_EQ(integer(a) % static_cast<uint32_t>(10),
               a % static_cast<uint32_t>(10));
    }
  }
}

TEST_CASE("++") {
  integer a = 0;
  CHECK_EQ(a++, 0);
  CHECK_EQ(a, 1);
  CHECK_EQ(++a, 2);
  CHECK_EQ(a, 2);
}

TEST_CASE("--") {
  integer a = 0;
  CHECK_EQ(a--, 0);
  CHECK_EQ(a, -1);
  CHECK_EQ(--a, -2);
  CHECK_EQ(a, -2);
}

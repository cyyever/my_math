/*!
 * \file arithmetic.hpp
 *
 * \brief 算术相关的函数
 */
#pragma once

#include <type_traits>

#include "exception.hpp"
#include "integer.hpp"
#include "rational.hpp"
namespace cyy::math {

  template <
      typename BASE_T, typename POWER_T,
      typename = std::enable_if_t<
          (std::is_same_v<BASE_T, integer> ||
           std::is_same_v<BASE_T, rational>)&&std::is_integral_v<POWER_T> &&
          std::is_integral_v<POWER_T>>>
  BASE_T exponent(BASE_T base, POWER_T power) {
    if constexpr (std::is_unsigned<POWER_T>::value) {
      BASE_T res = 1;

      while (power != 0) {
        if (power % 2 == 1) {
          res *= base;
        }
        power /= 2;
        base *= base;
      }
      return res;
    } else {
      bool add_one = false;
      if (power < 0) {
        if constexpr (!std::is_same_v<BASE_T, rational>) {
          throw exception::invalid_type("not rational base");
        } else {
          base = 1 / base;
          if (power == std::numeric_limits<POWER_T>::min()) {
            add_one = true;
            power += 1;
          }
          power = -power;
        }
      }
      auto res = exponent(base, std::make_unsigned_t<POWER_T>(power));
      if (add_one) {
        res *= base;
      }

      return res;
    }
  }

  template <typename T, typename = std::enable_if_t<std::is_integral_v<T> &&
                                                    std::is_unsigned_v<T>>>
  integer factorial(T n) {
    if (n <= 1) {
      return 1;
    }
    integer res = 2;
    T i = 3;
    while (i <= n) {
      res *= integer(i);
      i++;
    }
    return res;
  }

  rational ln(const rational &a);
  /* rational log(const integer &n, uint64_t base, size_t digit_num); */
} // namespace cyy::math

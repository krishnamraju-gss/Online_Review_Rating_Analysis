/* Try operation macros
(C) 2017-2019 Niall Douglas <http://www.nedproductions.biz/> (20 commits)
File Created: July 2017


Boost Software License - Version 1.0 - August 17th, 2003

Permission is hereby granted, free of charge, to any person or organization
obtaining a copy of the software and accompanying documentation covered by
this license (the "Software") to use, reproduce, display, distribute,
execute, and transmit the Software, and to prepare derivative works of the
Software, and to permit third-parties to whom the Software is furnished to
do so, all subject to the following:

The copyright notices in the Software and this entire statement, including
the above license grant, this restriction and the following disclaimer,
must be included in all copies of the Software, in whole or in part, and
all derivative works of the Software, unless such copies or derivative
works are solely in the form of machine-executable object code generated by
a source language processor.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
*/

#ifndef BOOST_OUTCOME_TRY_HPP
#define BOOST_OUTCOME_TRY_HPP

#include "success_failure.hpp"

namespace std  // NOLINT
{
  namespace experimental
  {
    template <class T, class E> class expected;
    template <class E> class unexpected;
  }  // namespace experimental
}  // namespace std

BOOST_OUTCOME_V2_NAMESPACE_BEGIN

/*! AWAITING HUGO JSON CONVERSION TOOL 
SIGNATURE NOT RECOGNISED
*/
BOOST_OUTCOME_TEMPLATE(class T)
BOOST_OUTCOME_TREQUIRES(BOOST_OUTCOME_TEXPR(std::declval<T>().as_failure()))
inline decltype(auto) try_operation_return_as(T &&v)
{
  return static_cast<T &&>(v).as_failure();
}
/*! AWAITING HUGO JSON CONVERSION TOOL 
SIGNATURE NOT RECOGNISED
*/
template <class T, class E> inline auto try_operation_return_as(const std::experimental::expected<T, E> &v)
{
  return std::experimental::unexpected<E>(v.error());
}
/*! AWAITING HUGO JSON CONVERSION TOOL 
SIGNATURE NOT RECOGNISED
*/
template <class T, class E> inline auto try_operation_return_as(std::experimental::expected<T, E> &&v)
{
  return std::experimental::unexpected<E>(static_cast<std::experimental::expected<T, E> &&>(v).error());
}

namespace detail
{
  BOOST_OUTCOME_TEMPLATE(class T)
  BOOST_OUTCOME_TREQUIRES(BOOST_OUTCOME_TEXPR(std::declval<T>().assume_value()))
  inline decltype(auto) try_extract_value(T &&v) { return static_cast<T &&>(v).assume_value(); }

  template <class T, class... Args> inline decltype(auto) try_extract_value(T &&v, Args &&... /*unused*/) { return static_cast<T &&>(v).value(); }
}  // namespace detail

BOOST_OUTCOME_V2_NAMESPACE_END

#define BOOST_OUTCOME_TRY_GLUE2(x, y) x##y
#define BOOST_OUTCOME_TRY_GLUE(x, y) BOOST_OUTCOME_TRY_GLUE2(x, y)
#define BOOST_OUTCOME_TRY_UNIQUE_NAME BOOST_OUTCOME_TRY_GLUE(_outcome_try_unique_name_temporary, __COUNTER__)

#define BOOST_OUTCOME_TRY_RETURN_ARG_COUNT(_1_, _2_, _3_, _4_, _5_, _6_, _7_, _8_, count, ...) count
#define BOOST_OUTCOME_TRY_EXPAND_ARGS(args) BOOST_OUTCOME_TRY_RETURN_ARG_COUNT args
#define BOOST_OUTCOME_TRY_COUNT_ARGS_MAX8(...) BOOST_OUTCOME_TRY_EXPAND_ARGS((__VA_ARGS__, 8, 7, 6, 5, 4, 3, 2, 1, 0))
#define BOOST_OUTCOME_TRY_OVERLOAD_MACRO2(name, count) name##count
#define BOOST_OUTCOME_TRY_OVERLOAD_MACRO1(name, count) BOOST_OUTCOME_TRY_OVERLOAD_MACRO2(name, count)
#define BOOST_OUTCOME_TRY_OVERLOAD_MACRO(name, count) BOOST_OUTCOME_TRY_OVERLOAD_MACRO1(name, count)
#define BOOST_OUTCOME_TRY_OVERLOAD_GLUE(x, y) x y
#define BOOST_OUTCOME_TRY_CALL_OVERLOAD(name, ...) BOOST_OUTCOME_TRY_OVERLOAD_GLUE(BOOST_OUTCOME_TRY_OVERLOAD_MACRO(name, BOOST_OUTCOME_TRY_COUNT_ARGS_MAX8(__VA_ARGS__)), (__VA_ARGS__))

#if !defined(__clang__) && defined(__GNUC__) && __GNUC__ >= 8
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wparentheses"
#endif

#define BOOST_OUTCOME_TRYV2(unique, ...)                                                                                                                                                                                                                                                                                             \
  auto && (unique) = (__VA_ARGS__);                                                                                                                                                                                                                                                                                            \
  if(!(unique).has_value())                                                                                                                                                                                                                                                                                                    \
  return BOOST_OUTCOME_V2_NAMESPACE::try_operation_return_as(static_cast<decltype(unique) &&>(unique))
#define BOOST_OUTCOME_TRY2(unique, v, ...)                                                                                                                                                                                                                                                                                           \
  BOOST_OUTCOME_TRYV2(unique, __VA_ARGS__);                                                                                                                                                                                                                                                                                          \
  auto && (v) = BOOST_OUTCOME_V2_NAMESPACE::detail::try_extract_value(static_cast<decltype(unique) &&>(unique))

#if !defined(__clang__) && defined(__GNUC__) && __GNUC__ >= 8
#pragma GCC diagnostic pop
#endif

/*! AWAITING HUGO JSON CONVERSION TOOL 
SIGNATURE NOT RECOGNISED
*/
#define BOOST_OUTCOME_TRYV(...) BOOST_OUTCOME_TRYV2(BOOST_OUTCOME_TRY_UNIQUE_NAME, __VA_ARGS__)

#if defined(__GNUC__) || defined(__clang__)

/*! AWAITING HUGO JSON CONVERSION TOOL 
SIGNATURE NOT RECOGNISED
*/
#define BOOST_OUTCOME_TRYX(...)                                                                                                                                                                                                                                                                                                      \
  ({                                                                                                                                                                                                                                                                                                                           \
    auto &&res = (__VA_ARGS__);                                                                                                                                                                                                                                                                                                \
    if(!res.has_value())                                                                                                                                                                                                                                                                                                       \
      return BOOST_OUTCOME_V2_NAMESPACE::try_operation_return_as(static_cast<decltype(res) &&>(res));                                                                                                                                                                                                                                \
    BOOST_OUTCOME_V2_NAMESPACE::detail::try_extract_value(static_cast<decltype(res) &&>(res));                                                                                                                                                                                                                                       \
  \
})
#endif

/*! AWAITING HUGO JSON CONVERSION TOOL 
SIGNATURE NOT RECOGNISED
*/
#define BOOST_OUTCOME_TRYA(v, ...) BOOST_OUTCOME_TRY2(BOOST_OUTCOME_TRY_UNIQUE_NAME, v, __VA_ARGS__)

#define BOOST_OUTCOME_TRY_INVOKE_TRY8(a, b, c, d, e, f, g, h) BOOST_OUTCOME_TRYA(a, b, c, d, e, f, g, h)
#define BOOST_OUTCOME_TRY_INVOKE_TRY7(a, b, c, d, e, f, g) BOOST_OUTCOME_TRYA(a, b, c, d, e, f, g)
#define BOOST_OUTCOME_TRY_INVOKE_TRY6(a, b, c, d, e, f) BOOST_OUTCOME_TRYA(a, b, c, d, e, f)
#define BOOST_OUTCOME_TRY_INVOKE_TRY5(a, b, c, d, e) BOOST_OUTCOME_TRYA(a, b, c, d, e)
#define BOOST_OUTCOME_TRY_INVOKE_TRY4(a, b, c, d) BOOST_OUTCOME_TRYA(a, b, c, d)
#define BOOST_OUTCOME_TRY_INVOKE_TRY3(a, b, c) BOOST_OUTCOME_TRYA(a, b, c)
#define BOOST_OUTCOME_TRY_INVOKE_TRY2(a, b) BOOST_OUTCOME_TRYA(a, b)
#define BOOST_OUTCOME_TRY_INVOKE_TRY1(a) BOOST_OUTCOME_TRYV(a)
/*! AWAITING HUGO JSON CONVERSION TOOL 
SIGNATURE NOT RECOGNISED
*/
#define BOOST_OUTCOME_TRY(...) BOOST_OUTCOME_TRY_CALL_OVERLOAD(BOOST_OUTCOME_TRY_INVOKE_TRY, __VA_ARGS__)

#endif

#pragma once

#include <type_traits>
#include <utility>

#include "utility.h"

template <typename T, typename... Args>
struct LibCppIsConstructible;

template <typename Derived, typename Base>
struct IsInvalidBaseToDerivedCast {
    using derived = uncvref_t<Derived>;
    using base = uncvref_t<Base>;
    static std::integral_constant<bool, std::is_base_of_v<base, derived> &&
                                            !std::is_same_v<base, derived> &&
                                            !LibCppIsConstructible<derived, Base>::type::value>
    value;
};

template <typename To, typename From>
struct IsInvalidLvalueToRvalueCast : std::false_type {};

template <typename RefTo, typename RefFrom>
struct IsInvalidLvalueToRvalueCast<RefTo&&, RefFrom&> {
    using to = uncvref_t<RefTo>;
    using from = uncvref_t<RefFrom>;
    static std::integral_constant<bool,
                                  std::is_base_of_v<to, from> &&
                                  std::is_same_v<from, to>>
    value;
};

struct IsConstructibleHelper {
    template <typename To>
    static void ImplicitCast(To);

    template <typename, typename>
    static std::false_type CheckIfCastable(...);

    template <typename To, typename From, typename = decltype(ImplicitCast<To>(Declval<From>()))>
    static std::true_type CheckIfCastable(int);

    template <typename To, typename From, typename = decltype(static_cast<To>(Declval<From>()))>
    static std::integral_constant<bool, !IsInvalidLvalueToRvalueCast<To, From>::value &&
                                            !IsInvalidBaseToDerivedCast<To, From>::value>
    CheckIfCastable(int64_t);

    template <typename, typename>
    static std::false_type CheckIfConstructible(...);

    template <typename T, typename... Args, typename = decltype(T(Declval<Args>()...))>
    static std::true_type CheckIfConstructible(int);
};

template <typename T, typename Arg>
struct LibCppIsConstructible<T, Arg> {
    using type = decltype(IsConstructibleHelper::CheckIfConstructible<T, Arg>(0));
};

template <typename T, typename... Args>
struct LibCppIsConstructible {
    using type = decltype(IsConstructibleHelper::CheckIfConstructible<T, Args...>(0));
};

template <typename T, typename Arg>
struct LibCppIsConstructible<T&, Arg> {
    using type = decltype(IsConstructibleHelper::CheckIfCastable<T&, Arg>(0));
};

template <typename T, typename Arg>
struct LibCppIsConstructible<T&&, Arg> {
    using type = decltype(IsConstructibleHelper::CheckIfCastable<T&&, Arg>(0));
};

template <typename T, typename... Args>
struct IsConstructible : LibCppIsConstructible<T, Args...>::type {};

template <typename T>
struct IsCopyConstructible : LibCppIsConstructible<T, const T&>::type {};
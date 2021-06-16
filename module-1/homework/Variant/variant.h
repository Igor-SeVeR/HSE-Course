#include <cstdlib>
#include <utility>

#pragma once

namespace task {

template <size_t Index, typename... Types>
union UnionHolder;

template <size_t Index>
union UnionHolder<Index> {};

template <size_t Index, typename T, typename... Types>
union UnionHolder<Index, T, Types...> {
public:
    UnionHolder() {
        if (!std::is_trivially_constructible_v<T>) {
            head = T();
        }
    }

    ~UnionHolder() {
        if (!!std::is_trivially_constructible_v<T>) {
            head.~T();
        }
    }

    friend struct UnionOperations;

private:
    T head;
    UnionHolder<Index + 1, Types...> tail;
};

struct UnionOperations {
    template <typename T>
    static constexpr auto&& Get(T&& val, std::in_place_index_t<0>) {
        return std::forward<T>(val).head;
    }

    template <typename T, size_t Index>
    static constexpr auto&& Get(T&& val, std::in_place_index_t<Index>) {
        return Get(std::forward<T>(val).tail, std::in_place_index<Index - 1>);
    }

    template <std::size_t Index, typename T, std::size_t UnionIndex, typename Head,
              typename... Tail>
    static void Set(T&& val, std::in_place_index_t<0>,
                    UnionHolder<UnionIndex, Head, Tail...>& curUnion) {
        if (std::is_same_v<T, Head> || std::is_convertible_v<T, Head>) {
            curUnion.head = val;
        }
    }

    template <std::size_t Index, typename T, std::size_t UnionIndex, typename Head,
              typename... Tail>
    static void Set(T&& val, std::in_place_index_t<Index>,
                    UnionHolder<UnionIndex, Head, Tail...>& curUnion) {
        Set<Index - 1>(std::forward<T>(val), std::in_place_index<Index - 1>, curUnion.tail);
    }
};

template <typename Head, typename... Tail>
struct TypeList {
    typedef Head head;
    typedef TypeList<Tail...> tail;
};

template <typename TList, unsigned int Index>
struct TypeAt;

template <typename Head, typename... Tail>
struct TypeAt<TypeList<Head, Tail...>, 0> {
    typedef Head TargetType;
};

template <typename Head, typename... Tail, unsigned int Index>
struct TypeAt<TypeList<Head, Tail...>, Index> {
    typedef typename TypeAt<TypeList<Tail...>, Index - 1>::TargetType TargetType;
};

template <std::size_t Size>
constexpr std::size_t FindPosition(std::size_t cur_pos, const bool (&check)[Size]) {
    if (cur_pos == Size) {
        return -1;
    }
    if (check[cur_pos]) {
        return cur_pos;
    }
    return FindPosition(cur_pos + 1, check);
}

template <typename TargetType, typename... Types>
struct FindType {
    constexpr static bool check[sizeof...(Types)] = {std::is_same<TargetType, Types>::value...};
    constexpr static bool convertible[sizeof...(Types)] = {
        std::is_convertible<TargetType, Types>::value...};
    constexpr static std::size_t foundIndex = FindPosition(0, check);
    constexpr static std::size_t convertibleIndex = 
        foundIndex == -1 ? FindPosition(0, convertible) : foundIndex;
};

template <typename... Types>
class Variant;

template <size_t Idx, typename T>
struct VariantAlternative;

template <size_t Idx, typename T>
using variant_alternative_t = typename VariantAlternative<Idx, T>::type;

template <size_t Idx, typename... Types>
struct VariantAlternative<Idx, Variant<Types...>> {
  using type = typename TypeAt<TypeList<Types...>, Idx>::TargetType;
};

template <typename TargetType, typename... Types>
auto&& GetTypeInVariant(Variant<Types...>& val) {
    const std::size_t foundIndex = FindType<TargetType, Types...>::foundIndex;
    return UnionOperations::Get(std::forward<Variant<Types...>>(val).variantHolder,
                                std::in_place_index<foundIndex>);
}

template <typename... Types>
class Variant {
public:
    constexpr Variant() noexcept;

    template <typename T>
    Variant& operator=(T&& t) noexcept;

    template <typename T, typename... AllTypes>
    friend auto&& GetTypeInVariant(Variant<AllTypes...>& val);

private:
    UnionHolder<0, Types...> variantHolder;
};

template <typename... Types>
constexpr Variant<Types...>::Variant() noexcept {};

template <typename... Types>
template <typename T>
Variant<Types...>& Variant<Types...>::operator=(T&& t) noexcept {
    const std::size_t foundIndex = FindType<T, Types...>::convertibleIndex;
    assert(foundIndex != -1);
    UnionOperations::Set<foundIndex>(std::forward<T>(t), std::in_place_index<foundIndex>,
                                     this->variantHolder);
    return *this;
}

template <std::size_t I, typename... Types>
constexpr const variant_alternative_t<I, Variant<Types...>>& Get(Variant<Types...>& v) {
    return GetTypeInVariant<typename TypeAt<TypeList<Types...>, I>::TargetType>(
        std::forward<decltype(v)>(v));
}

template <std::size_t I, typename... Types>
constexpr variant_alternative_t<I, Variant<Types...>>&& Get(Variant<Types...>&& v) {
    return GetTypeInVariant<typename TypeAt<TypeList<Types...>, I>::TargetType>(std::move(v));
}

template <typename T, typename... Types>
constexpr const T& Get(Variant<Types...>& v) {
    return GetTypeInVariant<T>(v);
}

template <typename T, typename... Types>
constexpr T&& Get(Variant<Types...>&& v) {
    return GetTypeInVariant<T>(std::move(v));
}

};
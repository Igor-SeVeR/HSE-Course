#include <cstdlib>
#include <type_traits>

#pragma once

namespace task {

struct NullOpt {
    explicit constexpr NullOpt(int) {}
};

constexpr NullOpt kNullOpt = NullOpt{0};

struct InPlace {
    explicit InPlace() = default;
};

constexpr InPlace kInPlace = InPlace{};

template<typename T, bool>
class BaseOptionalDestruct {
public:
    constexpr BaseOptionalDestruct() : isObtained(false) {}

    constexpr explicit BaseOptionalDestruct(NullOpt) : isObtained(false) {}

    template <typename... Args>
    constexpr explicit BaseOptionalDestruct(InPlace, Args&&... args) : value(std::forward<Args>(args)...), 
                                                                       isObtained(true) {}

    template<typename U = T>
    constexpr explicit BaseOptionalDestruct(U&& value) : value(std::forward<U>(value)), 
                                                         isObtained(true) {}

protected:
    bool isObtained;
    T value;

    void reset() {
        isObtained = false;
    }

    template<typename U = T>
    void set(U&& value) {
        this->value = std::forward<U>(value);
        isObtained = true;
    } 
};

template<typename T>
class BaseOptionalDestruct<T, false> {
public:
    constexpr BaseOptionalDestruct() : isObtained(false) {}

    constexpr explicit BaseOptionalDestruct(NullOpt) : isObtained(false) {}

    template <typename... Args>
    constexpr explicit BaseOptionalDestruct(InPlace, Args&&... args) : value(std::forward<Args>(args)...), 
                                                                       isObtained(true) {}

    template<typename U = T>
    constexpr explicit BaseOptionalDestruct(U&& value) : value(std::forward<U>(value)), 
                                                         isObtained(true) {}

    ~BaseOptionalDestruct() {
        if (this->isObtained)
            value.~T();
    }

protected:
    bool isObtained;
    T value;

    void reset() {
        if (this->isObtained)
            value.~T();
        isObtained = false;
    }

    template<typename U = T>
    void set(U&& value) {
        if (this->isObtained)
            value.~T();
        this->value = std::forward<U>(value);
        isObtained = true;
    } 
};

template <typename T>
class Optional : public // Your code goes here; {
public:
    using value_type =  // Your code goes here;

    constexpr Optional() noexcept;

    template <typename U = value_type>
    constexpr explicit Optional(U&& value);

    constexpr explicit Optional(NullOpt) noexcept;

    template <typename... Args>
    constexpr explicit Optional(InPlace, Args&&... args);

    Optional& operator=(NullOpt) noexcept;

    template <typename U = T>
    Optional& operator=(U&& value);

    void Reset() noexcept;

    template <typename U>
    constexpr T ValueOr(U&& default_value) const&;

    template <typename U>
    constexpr T ValueOr(U&& default_value) &&;

    constexpr bool HasValue() const noexcept;

    constexpr explicit operator bool() const noexcept;

    constexpr std::add_pointer_t<const value_type> operator->() const;

    constexpr std::add_pointer_t<value_type> operator->();

    constexpr const value_type& operator*() const&;

    constexpr value_type& operator*() &;

    constexpr const value_type&& operator*() const&&;

    constexpr value_type&& operator*() &&;
};
}  // namespace task
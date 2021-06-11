#include <cstdlib>
#include <type_traits>

#pragma once

namespace task {

struct NullOpt {
    explicit constexpr NullOpt(int) {
    }
};

constexpr NullOpt kNullOpt = NullOpt{0};

struct InPlace {
    explicit InPlace() = default;
};

constexpr InPlace kInPlace = InPlace{};

template <typename T, bool>
class BaseOptionalDestruct {
public:
    constexpr BaseOptionalDestruct() : isObtained(false) {
    }

    constexpr explicit BaseOptionalDestruct(NullOpt) : isObtained(false) {
    }

    template <typename... Args>
    constexpr explicit BaseOptionalDestruct(InPlace, Args&&... args)
        : value(std::forward<Args>(args)...), isObtained(true) {
    }

    template <typename U = T>
    constexpr explicit BaseOptionalDestruct(U&& value)
        : value(std::forward<U>(value)), isObtained(true) {
    }

protected:
    T value;
    bool isObtained;

    void ResetBase() {
        isObtained = false;
    }

    template <typename U = T>
    void Set(U&& value) {
        this->value = std::forward<U>(value);
        isObtained = true;
    }
};

template <typename T>
class BaseOptionalDestruct<T, false> {
public:
    constexpr BaseOptionalDestruct() : isObtained(false) {
    }

    constexpr explicit BaseOptionalDestruct(NullOpt) : isObtained(false) {
    }

    template <typename... Args>
    constexpr explicit BaseOptionalDestruct(InPlace, Args&&... args)
        : value(std::forward<Args>(args)...), isObtained(true) {
    }

    template <typename U = T>
    constexpr explicit BaseOptionalDestruct(U&& value)
        : value(std::forward<U>(value)), isObtained(true) {
    }

    ~BaseOptionalDestruct() {
        if (this->isObtained) {
            value.~T();
        }
    }

protected:
    T value;
    bool isObtained;

    void ResetBase() {
        if (this->isObtained) {
            value.~T();
        }
        isObtained = false;
    }

    template <typename U = T>
    void Set(U&& value) {
        if (this->isObtained) {
            value.~T();
        }
        this->value = std::forward<U>(value);
        isObtained = true;
    }
};

template <typename T>
class Optional : public BaseOptionalDestruct<T, std::is_trivially_destructible_v<T>> {
private:
    using base = BaseOptionalDestruct<T, std::is_trivially_destructible<T>::value>;

public:
    using value_type = T;

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

template <typename T>
constexpr Optional<T>::Optional() noexcept : base() {
}

template <typename T>
template <typename U>
constexpr Optional<T>::Optional(U&& value) : base(std::forward<U>(value)) {
}

template <typename T>
constexpr Optional<T>::Optional(NullOpt) noexcept : base(kNullOpt) {
}

template <typename T>
template <typename... Args>
constexpr Optional<T>::Optional(InPlace, Args&&... args) : base(kInPlace, args...) {
}

template <typename T>
Optional<T>& Optional<T>::operator=(NullOpt) noexcept {
    this->ResetBase();
    return *this;
}

template <typename T>
template <typename U>
Optional<T>& Optional<T>::operator=(U&& value) {
    this->Set(std::forward<U>(value));
    return *this;
}

template <typename T>
void Optional<T>::Reset() noexcept {
    this->ResetBase();
}

template <typename T>
template <typename U>
constexpr T Optional<T>::ValueOr(U&& default_value) const& {
    if (this->HasValue()) {
        return this->value;
    }
    return default_value;
}

template <typename T>
template <typename U>
constexpr T Optional<T>::ValueOr(U&& default_value) && {
    if (this->HasValue()) {
        return this->value;
    }
    return default_value;
}

template <typename T>
constexpr bool Optional<T>::HasValue() const noexcept {
    return this->isObtained;
}

template <typename T>
constexpr Optional<T>::operator bool() const noexcept {
    return this->HasValue();
}

template <typename T>
constexpr std::add_pointer_t<const typename Optional<T>::value_type> Optional<T>::operator->()
    const {
    return &(this->value);
}

template <typename T>
constexpr std::add_pointer_t<typename Optional<T>::value_type> Optional<T>::operator->() {
    return &(this->value);
}

template <typename T>
constexpr const typename Optional<T>::value_type& Optional<T>::operator*() const& {
    return this->value;
}

template <typename T>
constexpr typename Optional<T>::value_type& Optional<T>::operator*() & {
    return this->value;
}

template <typename T>
constexpr const typename Optional<T>::value_type&& Optional<T>::operator*() const&& {
    return this->value;
}

template <typename T>
constexpr typename Optional<T>::value_type&& Optional<T>::operator*() && {
    return this->value;
}

}  // namespace task
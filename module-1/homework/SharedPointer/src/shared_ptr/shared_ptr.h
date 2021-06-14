#pragma once

#include "../control/control.h"

template <typename T>
struct RemoveArrayType {
    using type = T;
};

template <typename T>
struct RemoveArrayType<T[]> {
    using type = T;
};

template <typename T, std::size_t N>
struct RemoveArrayType<T[N]> {
    using type = T;
};

// SharedPtr
template <typename T>
class WeakPtr;

template <typename T>
class SharedPtr {
public:
    using element_type = typename RemoveArrayType<T>::type;

    constexpr SharedPtr() noexcept = default;
    ~SharedPtr();

    template <typename Y>
    explicit SharedPtr(Y* p);

    template <typename Y, typename Deleter>
    SharedPtr(Y* p, Deleter deleter) noexcept;

    SharedPtr(const SharedPtr& other) noexcept;
    SharedPtr(SharedPtr&& other) noexcept;

    SharedPtr& operator=(const SharedPtr& r) noexcept;

    template <typename Y>
    SharedPtr& operator=(const SharedPtr<Y>& r) noexcept;

    SharedPtr& operator=(SharedPtr&& r) noexcept;

    template <typename Y>
    SharedPtr& operator=(SharedPtr<Y>&& r) noexcept;

    // Modifiers
    void Reset() noexcept;

    template <typename Y>
    void Reset(Y* p) noexcept;

    template <typename Y, typename Deleter>
    void Reset(Y* p, Deleter deleter) noexcept;

    void Swap(SharedPtr& other) noexcept;

    // Observers
    T* Get() const noexcept;
    int64_t UseCount() const noexcept;
    T& operator*() const noexcept;
    T* operator->() const noexcept;
    element_type& operator[](std::ptrdiff_t idx) const;
    explicit operator bool() const noexcept;

    template <typename U>
    friend class WeakPtr;

private:
    element_type* valuePointer = nullptr;
    SharedWeakCount* controlPointer = nullptr;

};


template <typename T, typename... Args>
SharedPtr<T> MakeShared(Args&&... args) {
    return SharedPtr<T>(new T(std::forward<Args>(args)...));
}

template <typename T>
SharedPtr<T>::~SharedPtr() {
    if (controlPointer != nullptr) {
        controlPointer->RemoveRef();
    }
    valuePointer = nullptr;
    controlPointer = nullptr;
}

template <typename T>
template <typename Y>
SharedPtr<T>::SharedPtr(Y* p)
    : valuePointer(p), controlPointer(new ControlBlock<element_type*>(p)) {
    controlPointer->AddRef();
}

template <typename T>
template <typename Y, typename Deleter>
SharedPtr<T>::SharedPtr(Y* p, Deleter deleter) noexcept
    : valuePointer(p), controlPointer(new ControlBlock<T*, Deleter>(p, deleter)) {
    controlPointer->AddRef();
}

template <typename T>
SharedPtr<T>::SharedPtr(const SharedPtr& other) noexcept
    : valuePointer(other.valuePointer), 
    controlPointer(other.controlPointer) {
    controlPointer->AddRef();
}

template <typename T>
SharedPtr<T>::SharedPtr(SharedPtr&& other) noexcept
    : valuePointer(other.valuePointer), 
    controlPointer(other.controlPointer) {
    other.valuePointer = nullptr;
    other.controlPointer = nullptr;
}

template <typename T>
SharedPtr<T>& SharedPtr<T>::operator=(const SharedPtr& r) noexcept {
    SharedPtr<T>(r).Swap(*this);
    return *this;
}

template <typename T>
SharedPtr<T>& SharedPtr<T>::operator=(SharedPtr&& r) noexcept {
    SharedPtr<T>(std::forward<SharedPtr>(r)).Swap(*this);
    return *this;
}

template <typename T>
void SharedPtr<T>::Reset() noexcept {
    SharedPtr<T>().Swap(*this);
}

template <typename T>
template <typename Y>
void SharedPtr<T>::Reset(Y* p) noexcept {
    SharedPtr<T>(p).Swap(*this);
}

template <typename T>
template <typename Y, typename Deleter>
void SharedPtr<T>::Reset(Y* p, Deleter deleter) noexcept {
    SharedPtr<T>(p, deleter).Swap(*this);
}

template <typename T>
void SharedPtr<T>::Swap(SharedPtr& other) noexcept {
    std::swap(valuePointer, other.valuePointer);
    std::swap(controlPointer, other.controlPointer);
}

template <typename T>
T* SharedPtr<T>::Get() const noexcept {
    return valuePointer;
}

template <typename T>
int64_t SharedPtr<T>::UseCount() const noexcept {
    if (controlPointer != nullptr) {
        return controlPointer->UseCount();
    }
    return 0;
}

template <typename T>
T& SharedPtr<T>::operator*() const noexcept {
    return *valuePointer;
}

template <typename T>
T* SharedPtr<T>::operator->() const noexcept {
    return valuePointer;
}

template <typename T>
typename SharedPtr<T>::element_type& SharedPtr<T>::operator[](std::ptrdiff_t idx) const {
    return *(valuePointer + idx);
}

template <typename T>
SharedPtr<T>::operator bool() const noexcept {
    return (valuePointer != nullptr);
}

// WeakPtr
template <typename T>
class WeakPtr {

public:
    using element_type = typename RemoveArrayType<T>::type;

    // Special-member functions
    constexpr WeakPtr() noexcept = default;
    template <typename Y>
    explicit WeakPtr(const SharedPtr<Y>& other);
    WeakPtr(const WeakPtr& other) noexcept;
    WeakPtr(WeakPtr&& other) noexcept;
    template <typename Y>
    WeakPtr& operator=(const SharedPtr<Y>& other);
    WeakPtr& operator=(const WeakPtr& other) noexcept;
    WeakPtr& operator=(WeakPtr&& other) noexcept;

    ~WeakPtr();

    // Modifiers
    void Reset() noexcept;
    void Swap(WeakPtr<T>& other) noexcept;

    // Observers
    bool Expired() noexcept;
    SharedPtr<T> Lock() const noexcept;

    template <typename U>
    friend class SharedPtr;

public:
    element_type* valuePointer = nullptr;
    SharedWeakCount* controlPointer = nullptr;

};

template <typename T>
template <typename Y>
WeakPtr<T>::WeakPtr(const SharedPtr<Y>& other)
    : valuePointer(other.valuePointer), controlPointer(other.controlPointer) {
}

template <typename T>
WeakPtr<T>::WeakPtr(const WeakPtr& other) noexcept
    : valuePointer(other.valuePointer), controlPointer(other.controlPointer) {
}

template <typename T>
WeakPtr<T>::WeakPtr(WeakPtr&& other) noexcept
    : valuePointer(other.valuePointer), 
    controlPointer(std::move(other.controlPointer)) {
    other.valuePointer = nullptr;
    other.controlPointer = nullptr;
}

template <typename T>
template <typename Y>
WeakPtr<T>& WeakPtr<T>::operator=(const SharedPtr<Y>& other) {
    valuePointer = other.valuePointer;
    controlPointer = other.controlPointer;
    return *this;
}

template <typename T>
WeakPtr<T>& WeakPtr<T>::operator=(const WeakPtr& other) noexcept {
    if (this == other) {
        return *this;
    }
    valuePointer = other.valuePointer;
    controlPointer = other.controlPointer;
    return *this;
}

template <typename T>
WeakPtr<T>& WeakPtr<T>::operator=(WeakPtr&& other) noexcept {
    valuePointer = other.valuePointer;
    controlPointer = other.controlPointer;
    other.valuePointer = nullptr;
    other.controlPointer = nullptr;
    return *this;
}

template <typename T>
WeakPtr<T>::~WeakPtr() {
    Reset();
}

template <typename T>
void WeakPtr<T>::Reset() noexcept {
    valuePointer = nullptr;
    controlPointer = nullptr;
}

template <typename T>
void WeakPtr<T>::Swap(WeakPtr<T>& other) noexcept {
    std::swap(valuePointer, other.valuePointer);
    std::swap(controlPointer, other.controlPointer);
}

template <typename T>
bool WeakPtr<T>::Expired() noexcept {
    return (controlPointer == nullptr || controlPointer->UseCount() == 0);
}

template <typename T>
SharedPtr<T> WeakPtr<T>::Lock() const noexcept {
    if (controlPointer->UseCount() == 0) {
        SharedPtr<T> answer;
        answer.controlPointer = nullptr;
        return answer;
    }
    SharedPtr<T> answer(valuePointer);
    answer.controlPointer = controlPointer;
    return answer;
}
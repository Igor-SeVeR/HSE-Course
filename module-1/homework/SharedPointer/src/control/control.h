#pragma once

#include <atomic>
#include <iostream>

class SharedCount {
public:
    explicit SharedCount(int64_t count = 0) : refCount(count) {
    }

    int64_t UseCount() const noexcept {
        return refCount;
    }

    void AddRef() noexcept {
        refCount.fetch_add(1);
    }

    void RemoveRef() noexcept {
        refCount.fetch_sub(1);
        if (refCount == 0) {
            ObjectDeleter();
        }
    }

    virtual void ObjectDeleter() noexcept = 0;

protected:
    std::atomic<int64_t> refCount;

    virtual ~SharedCount() = default;
};

class SharedWeakCount : public SharedCount {
public:
    int64_t UseCount() const noexcept {
        return SharedCount::UseCount();
    }

    virtual void ObjectDeleter() noexcept = 0;

protected:
    virtual ~SharedWeakCount() = default;
};

template <typename T>
struct PointerDeleter {
    void operator()(T pointer) noexcept {
        delete pointer;
    }
};

template <typename T, typename Deleter = PointerDeleter<T>>
class ControlBlock : public SharedWeakCount {
public:
    explicit ControlBlock(T pointer) : pointer(pointer), deleter(PointerDeleter<T>()) {
    }

    ControlBlock(T pointer, Deleter deleter) : pointer(pointer), deleter(deleter) {
    }

    void ObjectDeleter() noexcept override {
        deleter(pointer);
    }

private:
    T pointer;
    Deleter deleter;
};
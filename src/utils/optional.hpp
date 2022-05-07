#ifndef OPTIONAL_HPP
#define OPTIONAL_HPP

#include "functional"
#include "stdexcept"

template<typename T>
class Optional {

    T *ptr;

    public:
    explicit Optional(T *t1) {
        ptr = t1;
    }

    public:
    explicit Optional(T &t2) {
        ptr = &t2;
    }

    [[nodiscard]] bool isPresent() const noexcept(true) {
        return ptr != nullptr;
    }

    [[nodiscard]] bool isEmpty() const noexcept(true) {
        return ptr == nullptr;
    }

    T *value() noexcept(false) {
        if (ptr == nullptr) {
            throw std::invalid_argument("Optional is empty!");
        }
        return ptr;
    }
};

template<typename T>
Optional<T> nullopt() {
    return Optional<T>(nullptr);
}

#endif//OPTIONAL_HPP
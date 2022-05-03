#ifndef OPTIONAL_HPP
#define OPTIONAL_HPP

#include "functional"
#include "stdexcept"

template<typename T>
class optional {

    T *ptr;

    public:
    explicit optional(T *t) {
        ptr = t;
    }

    [[nodiscard]] bool is_present() const noexcept(true) {
        return ptr != nullptr;
    }

    [[nodiscard]] bool is_empty() const noexcept(true) {
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
optional<T> nullopt() {
    return optional<T>(nullptr);
}

#endif//OPTIONAL_HPP
#ifndef OPTIONAL_H
#define OPTIONAL_H

#include "functional"
#include "stdexcept"

/**
 * A wrapper for values which may exist
 * @tparam T The type of values
 */
template<typename T>
class Optional {

    // The pointer to the object
    T *ptr;

    public:
    explicit Optional(T *t1) {
        ptr = t1;
    }

    public:
    explicit Optional(T &t2) {
        ptr = &t2;
    }

    /**
     * Check whether this optional has a value
     * @return <code>true</code> If the wrapped vale is not a nullptr
     */
    [[nodiscard]] bool isPresent() const noexcept(true) {
        return ptr != nullptr;
    }

    /**
     * Check whether this optional does not have a value
     * @return <code>true</code> If the wrapped value is a nullptr
     */
    [[nodiscard]] bool isEmpty() const noexcept(true) {
        return ptr == nullptr;
    }

    /**
     * The pointer to the value stored in this optional
     * Callers of this function <strong>must</strong> check
     * <code>#isPresent()</code> or <code>#isEmpty()</code>
     * to ensure that there is a value
     * @return Return the pointer to the value wrapped by this optional
     * @throws std::invalid_argument if the value stored is a <code>nullptr</code>
     */
    T *value() noexcept(false) {
        if (ptr == nullptr) {
            throw std::invalid_argument("Optional is empty!");
        }
        return ptr;
    }
};

/**
 * Shorthand for creating an empty <code>Optional</code>
 * @tparam T The type of element
 * @return Returns an empty <code>Optional</code>
 */
template<typename T>
Optional<T> nullopt() {
    return Optional<T>(nullptr);
}

#endif//OPTIONAL_H
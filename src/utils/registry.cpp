#include "registry.h"

/**
 * Implementation of a Registry
 * @tparam K The type of keys
 * @tparam V The type of values
 */
template<typename K, typename V>
class SimpleRegistry : public Registry<K, V> {

    std::map<K, V> data;

    public:
    SimpleRegistry() = default;

    explicit SimpleRegistry(const std::map<K, V> &values) {
        data = std::map<K, V>(values);
    }

    ~SimpleRegistry() override = default;

    bool isRegistered(const K &key) const noexcept(true) override {
        return data.find(key) != data.end();
    }

    bool add(const K &key, const V &value) override {
        if (!isRegistered(key)) {
            data[key] = value;
            return true;
        }
        return false;
    }

    void remove(const K &key) override {
        auto iter = data.find(key);
        if (iter != data.end()) {
            data.erase(iter);
        }
    }

    Optional<V> get(const K &key) override {
        auto iter = data.find(key);
        if (iter == data.end()) {
            // Key doesn't exist so we return an empty optional
            return nullopt<V>();
        }
        return Optional<V>(&iter->second);
    }

    std::map<K, V> toMap() const noexcept(true) override {
        return std::map<K, V>{data};
    }
};

template<typename K, typename V>
Registry<K, V> *newRegistry() {
    return new SimpleRegistry<K, V>;
}

template<typename K, typename V>
Registry<K, V> *newRegistry(const std::map<K, V> &values) {
    return new SimpleRegistry<K, V>{values};
}
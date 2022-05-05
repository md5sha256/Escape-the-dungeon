#include "registry.hpp"

template<typename K, typename V>
class SimpleRegistry : public Registry<K, V> {

    std::map<K, V> data;

    public:
    SimpleRegistry() = default;

    explicit SimpleRegistry(const std::map<K, V> &values) {
        data = std::map<K, V>(values);
    }

    ~SimpleRegistry() override = default;

    bool isRegistered(K &key) const noexcept(true) override {
        return data.find(key) != data.end();
    }

    bool add(K &key, V &value) override {
        if (!isRegistered(key)) {
            data[key] = value;
            return true;
        }
        return false;
    }

    void remove(K &key) override {
        auto iter = data.find(key);
        if (iter != data.end()) {
            data.erase(iter);
        }
    }

    optional<V> get(K &key) override {
        auto iter = data.find(key);
        if (iter == data.end()) {
            return nullopt<V>();
        }
        return optional<V>(&iter->second);
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
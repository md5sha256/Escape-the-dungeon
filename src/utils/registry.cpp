#include "registry.hpp"

template<typename K, typename V>
class SimpleRegistry : public Registry<K, V> {

    std::map<K *, V *> map;

    public:
    SimpleRegistry() = default;

    explicit SimpleRegistry(const std::map<K*, V*> &values) {
        map = std::map<K*, V*>(values);
    }

    ~SimpleRegistry() {
        delete map;
    }

    bool isRegistered(K *key) const noexcept(true) override{
        return map.find(key) != map.end();
    }

    bool add(const K *key, const V *value) override {
        if (!isRegistered(key)) {
            map[key] = value;
            return true;
        }
        return false;
    }

    void remove(const K *key) override {
        auto iter = map.find(key);
        if (iter != map.end()) {
            map.erase(iter);
        }
    }

    optional<V> get(const K &key) override {
        auto iter = map.find(key);
        if (iter == map.end()) {
            return nullopt<V>();
        }
    }

    std::map<K*, V*> toMap() const noexcept(true) override {
        return std::map<K*, V*>{map};
    }

};

template<typename K, typename V>
Registry<K, V>* newRegistry() {
    return new SimpleRegistry<K, V>;
}

template<typename K, typename V>
Registry<K, V>* newRegistry(const std::map<K*, V*> &values) {
    return new SimpleRegistry<K, V>;
}
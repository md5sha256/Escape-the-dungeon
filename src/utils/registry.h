#ifndef REGISTRY_HPP
#define REGISTRY_HPP

#include "map"
#include "optional.h"

template<typename K, typename V>
class Registry {

    protected:
    Registry() = default;

    public:
    virtual ~Registry() = default;

    virtual bool isRegistered(const K &key) const noexcept(true) = 0;

    virtual bool add(const K &key, const V &value) = 0;

    virtual void remove(const K &key) = 0;

    virtual Optional<V> get(const K &key) = 0;

    virtual std::map<K, V> toMap() const noexcept(true) = 0;
};

template<typename K, typename V>
Registry<K, V> *newRegistry();
template<typename K, typename V>
Registry<K, V> *newRegistry(const std::map<K, V> &values);

#endif//REGISTRY_HPP

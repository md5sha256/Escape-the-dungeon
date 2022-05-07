#ifndef REGISTRY_H
#define REGISTRY_H

#include "map"
#include "optional.h"

/**
 * A simple registry which maps a key to a value
 * @tparam K The type of keys
 * @tparam V The type of values
 */
template<typename K, typename V>
class Registry {

    protected:
    Registry() = default;

    public:
    virtual ~Registry() = default;

    /**
     * Check if a value exists for the given key
     * @param key The key to check
     * @return <code>true</code> is a value exists for the key <code>false</code> otherwise
     */
    virtual bool isRegistered(const K &key) const noexcept(true) = 0;

    /**
     * Attempt to register a key-value pair
     * This function will only succeed if a value has not already
     * been registered for the given key
     * @param key The key
     * @param value The value
     * @return <code>true</code> if the value was successfully added <code>false</code> otherwise
     */
    virtual bool add(const K &key, const V &value) = 0;

    /**
     * Deregister a given key
     * @param key The key to deregister
     */
    virtual void remove(const K &key) = 0;

    /**
     * Get the value registered to the given key
     * @param key The key to lookup
     * @return Returns an <code>Optional</code> wrapping the value
     */
    virtual Optional<V> get(const K &key) = 0;

    /**
     * Convert this registry to a <code>std::map</code>
     * @return Returns a map representing the key-value pairs in this registry
     */
    virtual std::map<K, V> toMap() const noexcept(true) = 0;
};

///Create a new new registry on the heap
template<typename K, typename V>
Registry<K, V> *newRegistry();

///Create a new registry on the heap with some preregistered keys and values
template<typename K, typename V>
Registry<K, V> *newRegistry(const std::map<K, V> &values);

#endif//REGISTRY_H

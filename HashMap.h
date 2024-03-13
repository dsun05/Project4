#ifndef HashMap_h
#define HashMap_h

#include "geopoint.h"
#include <list>
#include <string>
#include <functional>
#include <iostream>
#include <fstream>

template <typename T>
class HashMap {
public:
    // Constructor initializes the hashmap. The default number of buckets is 10.
    HashMap(double max_load = 0.75) : m_list(), m_nBuckets(10), m_items(0), m_maxLoad(max_load)
    {
        m_list = new std::list<KeyValuePair>[m_nBuckets];
    };

    ~HashMap() {
        // Clear all buckets
        delete[] m_list;
    }

    int size() const { return m_items; }

    void insert(const std::string& key, const T& value);

    T& operator[](const std::string& key);

    const T* find(const std::string& key) const;

    T* find(const std::string& key) {
        const auto& hm = *this;
        return const_cast<T*>(hm.find(key));
    }

private:    
    void rehash();    
    bool contains(const std::string& key) const;
    HashMap(const HashMap&) = delete;
    HashMap& operator=(const HashMap&) = delete;
    int hash(const std::string& key) const {
        return std::hash<std::string>{}(key) % m_nBuckets;
    }

    struct KeyValuePair {
        std::string key;
        T value;
    };

    int m_nBuckets;
    int m_items;
    double m_maxLoad;
    std::list<KeyValuePair>* m_list;

};

template <typename T>
void HashMap<T>::insert(const std::string& key, const T& value) {

    int index = hash(key);

    if (!contains(key)) {
        if ((m_items + 1) / static_cast<double>(m_nBuckets) > m_maxLoad) {
            rehash();
        }
        index = hash(key);
        KeyValuePair kv = { key, value };
        m_list[index].push_back(kv);
        m_items++;
    }
    else {
        for (auto it = m_list[index].begin(); it != m_list[index].end(); it++) {
            if ((*it).key == key) {
                (*it).value = value; // Update existing value
                return;
            }
        }
    }
}

template <typename T>
T& HashMap<T>::operator[](const std::string& key) {
    int index = hash(key);

    if (!contains(key)) {
        insert(key, T()); // Insert with default value
    }

    // Search for the key and return its value
    for (auto it = m_list[index].begin(); it != m_list[index].end(); it++) {
        if ((*it).key == key) {
            return (*it).value;
        }
    }
}

template <typename T>
const T* HashMap<T>::find(const std::string& key) const {

    int index = hash(key);

    // Search for the key and return its value
    for (auto it = m_list[index].begin(); it != m_list[index].end(); it++) {
        if ((*it).key == key) {
            return &(*it).value;
        }
    }
    
    return nullptr; // Key not found
}

template <typename T>
bool HashMap<T>::contains(const std::string& key) const {

    if (m_items == 0) {
		return false;
	}

    int index = hash(key);

    // Search for the key in the bucket
    for (auto it = m_list[index].begin(); it != m_list[index].end(); it++) {
        if ((*it).key == key) {
            return true;
        }
    }

    return false; // Key not found
}

template <typename T>
void HashMap<T>::rehash() {
    int old_nBuckets = m_nBuckets;
    m_nBuckets *= 2;

    // Create a new array of linked lists with twice the capacity
    std::list<KeyValuePair>* new_list = new std::list<KeyValuePair>[m_nBuckets];

    // Reinsert existing elements into the new hash table
    for (int i = 0; i < old_nBuckets; i++) {
        for (auto it = m_list[i].begin(); it != m_list[i].end();) {
            int index = hash((*it).key);
			new_list[index].push_back(*it);
            it = m_list[i].erase(it);
        }
    }

    delete[] m_list;
    m_list = new_list;
}

#endif /* HashMap_h */
#pragma once
#include "cstddef"
#include "functional"
#include <iterator>
// implementation following Tessil/robin-map

template <typename Key, typename T, typename Hash = std::hash<Key>,
          typename Alloc = std::allocator<std::pair<Key, T>>>
class rmap {
public:
    using KVpair = std::pair<Key, T>;
    using const_iterator = std::iterator_traits<KVpair>;
    void insert(const Key &item);
    T find(const Key &item);
    void erase(const Key &item);
    Key *operator[](Key key);

private:
    template <typename KVpair>
    struct bucket {
        std::size_t distance_from_ideal_;
        bool is_last_bucket_;
        unsigned char kvpair_[sizeof(KVpair)];
    };
    constexpr static std::size_t SIZE = 100;
    bucket<KVpair> buckets_[SIZE];
    std::size_t load_factor_;
    const_iterator find(const Key &key, std::size_t hash) {};
    void rehash();
    void hash(T key);
};

#pragma once
#include "cstddef"
#include "functional"
#include "mystd/allocator.h"
#include <cstddef>
#include <iterator>
#include <memory>
// implementation following Tessil/robin-map

template <typename Key, typename T, typename Hash = std::hash<Key>,
          typename Alloc = mystd::PoolAllocator<std::pair<Key, T>>>
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
    class bucket {
    public:
        bucket() : distance_from_ideal_{-1}, is_last_bucket_{false} {}
        bucket(std::ptrdiff_t distance_from_ideal)
            : distance_from_ideal_{distance_from_ideal},
              is_last_bucket_{false} {}
        bool empty() const { return distance_from_ideal_ == -1; }
        bool is_last_bucket() const { return is_last_bucket_; }
        void set_last_bucket() { is_last_bucket_ = true; }
        KVpair &value() {
            return *std::launder(reinterpret_cast<KVpair *>(kvpair_));
        }
        const KVpair &value() const {
            return *std::launder(reinterpret_cast<KVpair *>(kvpair_));
        }
        void clear() {
            if (!empty()) {
                destroy();
                distance_from_ideal_ = -1;
            }
        };

    private:
        void destroy() { value()->~KVpair(); };
        std::ptrdiff_t distance_from_ideal_;
        bool is_last_bucket_;
        alignas(KVpair) std::byte kvpair_[sizeof(KVpair)];
    };
    struct key_select {
        Key &operator()(const KVpair &pair) { return pair.first; };
    };
    struct value_select {
        Key &operator()(const KVpair &pair) { return pair.second; };
    };
    constexpr static std::size_t SIZE = 100;
    bucket<KVpair> buckets_[SIZE];
    std::size_t load_factor_;
    const_iterator find_help(const Key &key, std::size_t hash) {
        auto ibucket = bucket_from_hash(hash);
        std::size_t distance = 0;
        while (distance <= ibucket.distance_from_ideal_) {
        }
    };
    bucket<KVpair> &bucket_from_hash(std::size_t hash) {
        return buckets_[hash % SIZE];
    };
    void erase_from_bucket(const_iterator iter) {};
    void rehash() {};
    void hash(T key);

    template <bool is_const>
    class robin_iterator {
        friend class rmap;

    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type =
            std::conditional_t<is_const, const typename rmap::KVpair,
                               typename rmap::KVpair>;
        using pointer = typename rmap::KVpair *;
        using reference = typename rmap::KVpair &;
        using difference_type = std::ptrdiff_t;
        robin_iterator() = default;
        reference operator*() { return bucket_->kvpair_; };
        pointer operator->() { return std::addressof(bucket_->kvpair_); };
        robin_iterator &operator++() {
            while (true) {
                if (bucket_->is_last_bucket_) {
                    ++bucket_;
                    return *this;
                }
                ++bucket_;
                if (!bucket_->empty())
                    return *this;
            }
        };
        robin_iterator &operator++(int) {
            auto &tmp = *this;
            ++bucket_;
            return tmp;
        };

    private:
        robin_iterator(rmap::bucket<KVpair> *bucket_entry)
            : bucket_{bucket_entry} {};
        using bucket_ptr =
            std::conditional_t<is_const, const typename rmap::bucket<KVpair> *,
                               typename rmap::bucket<KVpair> *>;
        bucket_ptr bucket_;
    };
};

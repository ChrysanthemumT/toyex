#pragma once
#include "BST.h"
#include "mystd/map.h"
#include "mystd/memory.h"
#include "protocol.h"
#include <memory_resource>

// maintain BSTs of buy and sell limits,
// as well as a map that allows for O(1) deletion,
// and addition of already inserted limits
struct Order;
struct Limit;

struct Limit {
    uint64_t price;
    uint64_t size;
    uint64_t total_volume;
    Limit *parent;
    Limit *left;
    Limit *right;
    mystd::unique_ptr<Order> head;
    mystd::unique_ptr<Order> tail;
};

struct Order {
    AddOrder order;
    uint64_t entry_time;
    uint64_t exit_time;
    mystd::unique_ptr<Order> next;
    Order *prev;
    Limit *parent;
};
class OrderBook {
public:
    inline void clear();
    inline void limit_sell(uint64_t order_id, uint64_t price, uint64_t amount);
    inline void limit_buy(uint64_t order_id, uint64_t price, uint64_t amount);
    Order &get(uint64_t order_id) const;
    void market_sell();
    void market_buy();
    uint64_t best_sell();
    uint64_t best_buy();

private:
    alignas(std::max_align_t) std::byte sellbuffer_[4096];
    alignas(std::max_align_t) std::byte buybuffer_[4096];
    std::pmr::monotonic_buffer_resource pool_sell{sellbuffer_,
                                                  sizeof(sellbuffer_)};
    std::pmr::monotonic_buffer_resource pool_buy{buybuffer_,
                                                 sizeof(buybuffer_)};
    BST<Limit> sell_;
    BST<Limit> buy_;
    Limit low_sell_;
    Limit highest_buy_;
    std::unordered_map<uint64_t, Limit *> order_map_;
};

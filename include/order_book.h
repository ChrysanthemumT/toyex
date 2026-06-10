#pragma once
#include "BST.h"
#include "mystd/allocator.h"
#include "mystd/map.h"
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
    Limit *parent = nullptr;
    Limit *left = nullptr;
    Limit *right = nullptr;
    Order *head = nullptr;
    Order *tail = nullptr;
};

struct Order {
    AddOrder order;
    Order *next = nullptr;
    Order *prev = nullptr;
    Limit *parent = nullptr;
};
class OrderBook {
public:
    void clear();
    void process(AddOrder &);
    void process(CancelOrder &);
    void process(Trade &);
    void limit_sell(AddOrder &processed_sell_order);
    void limit_buy(AddOrder &processed_buy_order);
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
    mystd::PoolAllocator<Order, 4096> order_alloc_;
    mystd::PoolAllocator<Limit, 4096> limit_alloc_;
    BST<Limit> sell_;
    BST<Limit> buy_;
    Limit *low_sell_;
    Limit *highest_buy_;
    std::unordered_map<uint64_t, Order *> order_map_;
};

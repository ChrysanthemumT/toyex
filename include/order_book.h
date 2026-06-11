#pragma once
#include "BST.h"
#include "mystd/allocator.h"
#include "mystd/map.h"
#include "protocol.h"
#include <memory_resource>

// maintain BSTs of buy and sell limits,
// as well as a map that allows for O(1) deletion,
// and addition of already inserted limits

template <typename Derived>
struct PoolAllocated {
    template <std::size_t N, typename... Args>
    static Derived *create(mystd::PoolAllocator<Derived, N> &alloc,
                           Args &&...args) {
        auto mem = alloc.allocate();
        return new (mem) Derived{std::forward<Args>(args)...};
    };
};
struct Order;
struct Limit;

struct Limit : PoolAllocated<Limit> {
    uint64_t price;
    uint64_t size;
    uint64_t total_volume;
    Limit *parent = nullptr;
    Limit *left = nullptr;
    Limit *right = nullptr;
    Order *head = nullptr;
    Order *tail = nullptr;
    Limit(uint64_t pprice, uint64_t psize, uint64_t ptotal_volume)
        : price{pprice}, size{psize}, total_volume{ptotal_volume} {}
};

struct Order : PoolAllocated<Order> {
    AddOrder order;
    Order *next = nullptr;
    Order *prev = nullptr;
    Limit *parent = nullptr;
    Order(AddOrder porder, Order *pnext, Order *pprev, Limit *pparent)
        : order{porder}, next{pnext}, prev{pprev}, parent{pparent} {}
};

class OrderBook {
private:
    static constexpr std::size_t SIZE = 4096;
    template <typename T>
    using PoolAlloc = mystd::PoolAllocator<T, SIZE>;

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
    PoolAlloc<Order> order_alloc_;
    PoolAlloc<Limit> limit_alloc_;
    BST<Limit> sell_;
    BST<Limit> buy_;
    Limit *low_sell_ = nullptr;
    Limit *highest_buy_ = nullptr;
    std::unordered_map<uint64_t, Order *> order_map_;
};

#pragma once
#include "BST.h"
#include "mystd/map.h"
#include "mystd/memory.h"
#include "protocol.h"

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
    mystd::unique_ptr<BST<Limit>> sell_;
    mystd::unique_ptr<BST<Limit>> buy_;
    mystd::unique_ptr<Limit> low_sell_;
    mystd::unique_ptr<Limit> highest_buy_;
};

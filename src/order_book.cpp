#include "order_book.h"
#include "protocol.h"

void OrderBook::process(AddOrder &order) {
    if (order.buyorsell == 'B') {
        OrderBook::limit_buy(order);
    } else {
        OrderBook::limit_sell(order);
    }
};
void OrderBook::process(CancelOrder &order) {
    auto order_struct = order_map_[order.order_id];
    bool is_head = order_struct->prev == nullptr;
    if (order_struct->next == nullptr)
        order_struct->parent->tail = order_struct->prev;
    if (order_struct->prev != nullptr)
        order_struct->prev->next = order_struct->next;
    if (order_struct->next != nullptr)
        order_struct->next->prev = order_struct->prev;
    if (is_head) // dealloc
        limit_alloc_.deallocate(order_struct->parent);
    order_alloc_.destroy(order_struct);
};
void OrderBook::process(Trade &) {};

void OrderBook::clear() {};
void OrderBook::limit_sell(AddOrder &processed_sell_order) {
    Limit tmp{static_cast<uint64_t>(processed_sell_order.price), 0, 0};
    if (auto *limit_node = sell_.search(&tmp); limit_node != nullptr) {
        // void *mem = pool_sell.allocate(sizeof(Order), alignof(Order));
        auto mem = order_alloc_.allocate();
        limit_node->tail->next = new (mem)
            Order{processed_sell_order, {}, limit_node->tail, limit_node};
        limit_node->tail = limit_node->tail->next;
        limit_node->size++;
        limit_node->total_volume += processed_sell_order.amount;
        order_map_[processed_sell_order.order_id] = limit_node->tail;
    } else {
        // void *mem = pool_sell.allocate(sizeof(Limit), alignof(Limit));
        // void *mem2 = pool_sell.allocate(sizeof(Order), alignof(Order));
        auto mem = limit_alloc_.allocate();
        auto mem2 = order_alloc_.allocate();
        auto new_limit =
            new (mem) Limit{static_cast<uint64_t>(processed_sell_order.price),
                            1, processed_sell_order.amount};
        new_limit->head =
            new (mem2) Order{processed_sell_order, {}, {}, new_limit};
        new_limit->tail = new_limit->head;
        sell_.insert(new_limit);
        order_map_[processed_sell_order.order_id] = new_limit->head;
    }
};
void OrderBook::limit_buy(AddOrder &processed_buy_order) {
    Limit tmp{static_cast<uint64_t>(processed_buy_order.price), 0, 0};
    if (auto *limit_node = buy_.search(&tmp); limit_node != nullptr) {
        // void *mem = pool_buy.allocate(sizeof(Order), alignof(Order));
        auto mem = order_alloc_.allocate();
        limit_node->tail->next = new (mem)
            Order{processed_buy_order, {}, limit_node->tail, limit_node};
        limit_node->tail = limit_node->tail->next;
        limit_node->size++;
        limit_node->total_volume += processed_buy_order.amount;
        order_map_[processed_buy_order.order_id] = limit_node->tail;
    } else {
        // void *mem = pool_buy.allocate(sizeof(Limit), alignof(Limit));
        // void *mem2 = pool_buy.allocate(sizeof(Order), alignof(Order));
        auto mem = limit_alloc_.allocate();
        auto mem2 = order_alloc_.allocate();
        auto new_limit =
            new (mem) Limit{static_cast<uint64_t>(processed_buy_order.price), 1,
                            processed_buy_order.amount};
        new_limit->head =
            new (mem2) Order{processed_buy_order, {}, {}, new_limit};
        new_limit->tail = new_limit->head;
        buy_.insert(new_limit);
        order_map_[processed_buy_order.order_id] = new_limit->tail;
    }
}
Order &OrderBook::get(uint64_t order_id) const {};
void OrderBook::market_sell() {};
void OrderBook::market_buy() {};
uint64_t OrderBook::best_sell() {};
uint64_t OrderBook::best_buy() {};

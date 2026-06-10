#include "order_book.h"

inline void OrderBook::clear() {};
inline void OrderBook::limit_sell(uint64_t order_id, uint64_t price,
                                  uint64_t amount) {};
inline void OrderBook::limit_buy(uint64_t order_id, uint64_t price,
                                 uint64_t amount) {};
Order &OrderBook::get(uint64_t order_id) const {};
void OrderBook::market_sell() {};
void OrderBook::market_buy() {};
uint64_t OrderBook::best_sell() {};
uint64_t OrderBook::best_buy() {};

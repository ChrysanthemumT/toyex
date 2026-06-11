#pragma once
#include <algorithm>
#include <cstdint>

struct Header {
    uint64_t sequence_num;
    uint64_t timestamp;
    uint32_t instrument_id;
    uint16_t type;
    uint16_t msg_len;
};

enum MessageT : uint16_t {
    ADD = 1,
    CANCEL = 2,
    TRADE = 3,
};

struct AddOrder {
    struct Header header;
    uint64_t order_id;
    double price;
    uint32_t amount;
    char buyorsell;
};
struct CancelOrder {
    struct Header header;
    uint64_t order_id;
};
struct Trade {
    struct Header header;
    uint32_t price;
    uint32_t amount;
};

constexpr const std::size_t MAX_SIZE =
    std::max({sizeof(struct AddOrder), sizeof(struct CancelOrder),
              sizeof(struct Trade)});

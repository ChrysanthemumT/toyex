#pragma once
#include "mystd/variant.h"
#include "protocol.h"

using variant_t = mystd::variant<AddOrder, CancelOrder, Trade>;
class Parser {
public:
    void set(void *buf) { buf_ = buf; }
    variant_t parse();

private:
    void *buf_;
};

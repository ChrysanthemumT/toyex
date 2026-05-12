#include "parser.h"
#include "protocol.h"

auto Parser::parse() -> variant_t {
    struct Header *header = reinterpret_cast<struct Header *>(buf_);
    switch (header->type) {
    case ADD: {
        return *reinterpret_cast<struct AddOrder *>(buf_);
    }
    case CANCEL: {
        return *reinterpret_cast<struct CancelOrder *>(buf_);
    }
    case TRADE: {
        return *reinterpret_cast<struct Trade *>(buf_);
    }
    default:
        throw std::runtime_error("unknown message type");
    }
}

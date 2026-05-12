#include "mystd/lmax_disruptor.h"
#include "parser.h"
#include "protocol.h"
#include "udp_socket.h"
#include <arm_neon.h>
#include <cstdio>
#include <iostream>

struct Packet {
    char data[MAX_SIZE];
};

int main() {
    auto sock = UDPSocket::instance();
    mystd::lmax_disruptor<struct Packet, 20, 1> lmax;
    Parser psr;
    while (1) {
        sock.recv(lmax.get_slot(), sizeof(struct Packet));
        lmax.publish();
        Packet packet{};
        if (lmax.try_pop(packet, 0)) {
            psr.set(&packet);
            variant_t var = psr.parse();
            var.visit(
                [](auto &item) { std::cout << item.header.type << std::endl; });
        }
    }
    return 0;
}

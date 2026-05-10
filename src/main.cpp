#include "mystd/lmax_disruptor.h"
#include <arm_neon.h>
#include <cstdio>
#include <iostream>
#include <udp_socket.h>

struct message {
    uint32_t data;
};
int main() {
    auto sock = UDPSocket::instance();
    mystd::lmax_disruptor<struct message, 20, 1> lmax;
    while (1) {
        sock.recv(lmax.get_slot(), sizeof(message));
        lmax.publish();
        struct message m{};
        if (lmax.try_pop(m, 0))
            std::cout << m.data << std::endl;
    }
    return 0;
}

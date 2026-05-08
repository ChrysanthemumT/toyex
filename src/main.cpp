#include <arm_neon.h>
#include <cstdio>
#include <iostream>
#include <udp_socket.h>

int main() {
    int fd = UDPSocket::instance().get_fd();
    return 0;
}

#include "protocol.h"
#include <arpa/inet.h>
#include <cstdint>
#include <iostream>
#include <netdb.h>      // addrinfo, getaddrinfo
#include <netinet/in.h> // sockaddr_in
#include <sys/socket.h>
#include <unistd.h>

int main() {
    const char *PORT = "8081";
    int rv, fd_;
    struct addrinfo hints, *res, *p;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE;
    if ((rv = getaddrinfo(NULL, PORT, &hints, &res)) != 0) {
        fprintf(stderr, "getaddrinfo failed:%s\n", gai_strerror(rv));
        return 1;
    }
    for (p = res; p != NULL; p = p->ai_next) {
        if ((fd_ = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) ==
            -1) {
            perror("socket failed\n");
            continue;
        }
        break;
    }
    if (p == NULL) {
        fprintf(stderr, "creating socket failed");
        return 1;
    }
    freeaddrinfo(res);
    // fake broadcast, im just gonna send to toyex;
    struct sockaddr_in broadcast;
    broadcast.sin_family = AF_INET;
    broadcast.sin_port = htons(8080);
    inet_pton(broadcast.sin_family, "127.0.0.1", &broadcast.sin_addr);
    for (;;) {
        uint64_t sequence_num;
        uint64_t timestamp;
        uint32_t instrument_id;
        uint16_t type;
        uint16_t msg_len;
        Trade message{
            .header = {1, 1, 1, 3, sizeof(Trade)},
            .amount = 1,
            .price = 1,
        };
        sendto(fd_, &message, sizeof(message), 0,
               reinterpret_cast<struct sockaddr *>(&broadcast),
               sizeof(broadcast));
        sleep(4);
    }
}

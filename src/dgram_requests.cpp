#include "protocol.h"
#include <arpa/inet.h>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <netdb.h>      // addrinfo, getaddrinfo
#include <netinet/in.h> // sockaddr_in
#include <sys/socket.h>
#include <unistd.h>

int main() {
    srand(time(0));
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
    uint64_t seq = 0;
    for (;;) {
        uint16_t type = (rand() % 3) + 1;
        switch (type) {
        case ADD: {
            AddOrder message{};
            message.header = {seq++, __builtin_readcyclecounter(), 1, ADD,
                              sizeof(AddOrder)};
            message.order_id = rand() % 10000;
            message.price = 100.0 + (rand() % 1000) * 0.01;
            message.amount = 100 + rand() % 900;
            message.buyorsell = (rand() % 2) ? 'B' : 'S';
            sendto(fd_, &message, sizeof(message), 0,
                   reinterpret_cast<sockaddr *>(&broadcast), sizeof(broadcast));
            break;
        }
        case CANCEL: {
            CancelOrder message{};
            message.header = {seq++, __builtin_readcyclecounter(), 1, CANCEL,
                              sizeof(CancelOrder)};
            message.order_id = rand() % 10000;
            sendto(fd_, &message, sizeof(message), 0,
                   reinterpret_cast<sockaddr *>(&broadcast), sizeof(broadcast));
            break;
        }
        case TRADE: {
            Trade message{};
            message.header = {seq++, __builtin_readcyclecounter(), 1, TRADE,
                              sizeof(Trade)};
            message.price = 100.0 + (rand() % 1000) * 0.01;
            message.amount = 100 + rand() % 900;
            sendto(fd_, &message, sizeof(message), 0,
                   reinterpret_cast<sockaddr *>(&broadcast), sizeof(broadcast));
            break;
        }
        default:
            break;
        }
        sleep(4);
    }
}

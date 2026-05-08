#include "udp_socket.h"
#include <netdb.h>      // addrinfo, getaddrinfo
#include <netinet/in.h> // sockaddr_in
#include <stdint.h>
#include <sys/socket.h> // socket(), bind() ...
#include <sys/types.h>

UDPSocket &UDPSocket::instance() {
    static UDPSocket sock{};
    return sock;
}

int UDPSocket::get_fd() const { return fd_; }

UDPSocket::UDPSocket() {
    int rv, sockfd;
    struct addrinfo ai, hints, *res, *p;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE;
    if ((rv = getaddrinfo(NULL, PORT, &hints, &res)) != 0) {
        fprintf(stderr, "getaddrinfo failed:%s\n", gai_strerror(rv));
    }
    for (p = res; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) ==
            -1) {
            perror("socket failed:");
            continue;
        }
        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            perror("bind failed:");
            continue;
        }
    }
    if (p == NULL) {
        fprintf(stderr, "failed to bind socket\n");
        return;
    }
    freeaddrinfo(res);
    fd_ = sockfd;
}

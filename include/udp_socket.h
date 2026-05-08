#pragma once
#include <iostream>

class UDPSocket {
    // parser?
public:
    static UDPSocket &instance();
    int get_fd() const;

private:
    static constexpr const char *PORT = "8080";
    UDPSocket();
    int fd_;
};

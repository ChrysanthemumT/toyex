cmake_minimum_required(VERSION 3.20)
project(toyex)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64")
    add_compile_options(-mavx2 -mfma)
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "arm64|aarch64")
    add_compile_options(-march=native)
endif()

add_library(mystd INTERFACE)
target_include_directories(mystd INTERFACE ../mystd/include)


add_executable(toyex
    src/main.cpp
    src/parser.cpp
    src/order_book.cpp
    src/udp_socket.cpp
)
target_include_directories(toyex PRIVATE include)
target_link_libraries(toyex PRIVATE mystd)







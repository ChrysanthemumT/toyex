#pragma once
#include "mystd/memory.h"
#include <concepts>

template <typename T>
concept BSTable = requires(T t) {
    { t.left } -> std::convertible_to<T *>;
    { t.right } -> std::convertible_to<T *>;
    { t.parent } -> std::convertible_to<T *>;
    { t.price } -> std::totally_ordered;
};

template <BSTable T>
class BST {
public:
    BST(T root) : root_{mystd::make_unique(root)}, count_{1}, volume_{0} {};
    T *insert(T *value);
    void remove(T *value);
    T *search(T *value);

private:
    void min_value(T *curr, T *&mini);
    void max_value(T *curr, T *&maxi);
    T *insert(T *value, T *curr);
    T *search(T *value, T *curr);
    T *root_;
    uint64_t count_;
    uint64_t volume_;
};

#include "BST.h"

template <BSTable T>
void BST<T>::min_value(T *curr, T *&mini) {
    if (curr == nullptr)
        return;
    if (curr->price < mini->price) {
        mini = curr;
    }
    BST::min_value(curr->left, mini);
};

template <BSTable T>
void BST<T>::remove(T *value) {
    bool no_left = value->left == nullptr;
    bool no_right = value->right == nullptr;
    if (no_left && no_right) {
        if (value->parent == nullptr)
            root_ = nullptr;
        else if (value->parent->left == value)
            value->parent->left = nullptr;
        else
            value->parent->right = nullptr;

    } else if (no_left) {
        value->right->parent = value->parent;
        if (value->parent == nullptr)
            root_ = value->right;
        else if (value->parent->left == value)
            value->parent->left = value->right;
        else
            value->parent->right = value->right;

    } else if (no_right) {
        value->left->parent = value->parent;
        if (value->parent == nullptr)
            root_ = value->left;
        else if (value->parent->left == value)
            value->parent->left = value->left;
        else
            value->parent->right = value->left;

    } else {
        T *mini = value->right;
        min_value(value->right, mini);
        if (mini->parent != value)
            mini->parent->left = mini->right;
        if (mini->right)
            mini->right->parent = mini->parent;
        mini->parent = value->parent;
        mini->left = value->left;
        mini->right = (mini != value->right) ? value->right : nullptr;

        if (mini->left)
            mini->left->parent = mini;
        if (mini->right)
            mini->right->parent = mini;

        if (value->parent == nullptr)
            root_ = mini;
        else if (value->parent->left == value)
            value->parent->left = mini;
        else
            value->parent->right = mini;
    }
};

template <BSTable T>
T *BST<T>::insert(T *value) {
    if (root_ == nullptr) {
        return root_ = value;
    } else {
        return BST::insert(value, root_);
    }
};

template <BSTable T>
T *BST<T>::insert(T *value, T *curr) {
    if (value->price == curr->price) {
        return value;
    } else {
        if (value->price < curr->price) {
            if (curr->left == nullptr) {
                value->parent = curr;
                return curr->left = value;
            } else
                return BST::insert(value, curr->left);
        } else {
            if (curr->right == nullptr) {
                value->parent = curr;
                return curr->right = value;
            } else
                return BST::insert(value, curr->right);
        }
    };
};

template <BSTable T>
T *BST<T>::search(T *value) {
    if (root_ == nullptr) {
        return root_;
    } else {
        return BST::search(value, root_);
    }
};

template <BSTable T>
T *BST<T>::search(T *value, T *curr) {
    if (value->price == curr->price) {
        return curr;
    } else {
        if (value->price < curr->price) {
            if (value->left == nullptr)
                return nullptr;
            else
                return BST::search(value, curr->left);
        } else {
            if (value->right == nullptr)
                return nullptr;
            else
                return BST::search(value, curr->right);
        }
    }
};

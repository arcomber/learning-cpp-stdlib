/* example:

	8                                      4
   / \                                    /  \
  5   10                                 2    6
 / \    \                              / \   / \
1   7   12                            1  3  5   7

All nodes on left contain values < parent node
All nodes on right contain values > parent node

Simple example of binary search tree operations:
1. basic data structure with value, left and right links
2. search - recursive - find()
3. insert - add_node()
4. delete all nodes - deallocate_nodes() - cleanup
5. various traversal operations

This first example is a set.  This is a bit less work because there
is no need for a key value pair - the set is just the key

*/

#ifndef ORDERED_SET_HPP_
#define ORDERED_SET_HPP_

#include <cstddef>
#include <utility>
#include <iterator>

namespace wheel {  // as in re-inventing the wheel

struct binary_tree_node {
	int value;
	binary_tree_node* left = nullptr;
	binary_tree_node* right = nullptr;
};


  class ordered_set {
  public:

      struct iterator {

          using value_type = int; // T;
          using difference_type = std::ptrdiff_t;
          using pointer = int*; // T*;
          using reference = int&; // T&;
          using iterator_category = std::bidirectional_iterator_tag;

          constexpr iterator(binary_tree_node* p) noexcept : ptr_{ p } {}

          iterator& operator++() {
              if (ptr_) {
                  ptr_ = ptr_->left;  // this is wrong
              }
              return *this;
          }

          iterator operator++(int) {
              auto old = *this;
              if (ptr_) {
                  ptr_ = ptr_->left;  // this is wrong next;
              }
              return old;
          }

          int& operator*() const { return ptr_->value; }
          int* operator->() { return &ptr_->value; }

          bool operator==(const iterator& other) const { return ptr_ == other.ptr_; }
          bool operator!=(const iterator& other) const { return ptr_ != other.ptr_; }

          binary_tree_node* ptr_ = nullptr;
      };



    ordered_set() = default;

    ~ordered_set() {
        clear();
    }

    // Returns a pair consisting of an iterator to the inserted element(or to the 
    // element that prevented the insertion) and a bool value set to true if the 
    // insertion took place.
    std::pair<ordered_set::iterator, bool> insert(int value) {
        std::pair<ordered_set::iterator, bool> result{nullptr, false};
        if (root == nullptr) {
            root = make_node(value);
            ++size_;
            result.first = root;
            result.second = true;
        }
        else {
            binary_tree_node* inserted_node = nullptr;;
            if (add_node(root, value, inserted_node)) {
                ++size_;
                result.first = inserted_node;
                result.second = true;
            }
        }
        return result;
    }

    iterator find(const int& key) {
        binary_tree_node* node = find(root, key);
        if (node == nullptr) {
            return iterator(nullptr);
        }
        else {
            return iterator(node);
        }
    }
    //const_iterator find(const int& key) const {
//
//    }

    // INVESTIGATE why c++11 version has noexcept
    void clear() {
        deallocate_nodes(root);
        root = nullptr;
        size_ = 0;
    }

    size_t size() const {
        return size_;
    }

   // O(1)
    iterator end() {
        return nullptr;
    }

  private:
      binary_tree_node* make_node(int value) {
          binary_tree_node* node = new binary_tree_node;
          node->value = value;
          node->left = nullptr;
          node->right = nullptr;
          return node;
      }

      bool add_node(binary_tree_node* tree, int value, binary_tree_node*& inserted_node) {
          if (value < tree->value) {
              if (tree->left == nullptr) {
                  tree->left = make_node(value);
                  inserted_node = tree->left;
                  return true;
              }
              else {
                  return add_node(tree->left, value, inserted_node);
              }
          }
          else if (value > tree->value) {
              if (tree->right == nullptr) {
                  tree->right = make_node(value);
                  inserted_node = tree->right;
                  return true;
              }
              else {
                  return add_node(tree->right, value, inserted_node);
              }
          }
          // note that if value is equal, do nothing right now
          return false; // means value already added
      }

      binary_tree_node* find(binary_tree_node* tree, int value) {
          if (tree == nullptr || tree->value == value)
              return tree;
          else {
              if (value < tree->value)
                  return find(tree->left, value);
              else
                  return find(tree->right, value);
          }
      }

      void deallocate_nodes(binary_tree_node* tree) {
          if (tree != nullptr) {
              deallocate_nodes(tree->left);
              deallocate_nodes(tree->right);
              delete tree;
          }
      }

    binary_tree_node* root = nullptr;
    size_t size_ = 0;

  };
  

}  // namespace wheel

#endif // ORDERED_SET_HPP_

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <random>
#include <utility>
#include <vector>

class TreapArray {
  struct Node {
    Node(int64_t priority, const long long &value)
        : size(1),
          priority(priority),
          delta(value),
          value(value),
          left(nullptr),
          right(nullptr) {}

    int64_t size;
    int64_t priority;
    long long delta;
    long long value;
    Node *left;
    Node *right;
  };

 public:
  TreapArray() : root_(nullptr) {}

  ~TreapArray() { Clear(root_); }

  int64_t Size() { return Size(root_); }

  bool Empty() { return Size(root_) == 0; }

  void Erase(int64_t pos) {
    auto [left, right_with_pos] = Split(root_, pos);
    auto [pos_tree, right] = Split(right_with_pos, 1);
    delete pos_tree;
    root_ = Merge(left, right);
  }

  long long GetMin(long long left, long long right) {
    std::pair p1 = Split(root_, left);
    std::pair p2 = Split(p1.second, right - left + 1);
    long long minimal_value = DeltaIn(p2.first);
    root_ = Merge(p1.first, Merge(p2.first, p2.second));
    return minimal_value;
  }

  void Insert(int64_t pos, const long long &value) {
    int64_t priority = distribution_(gen_);
    Node *node = new Node(priority, value);
    auto [first, second] = Split(root_, pos);
    root_ = Merge(Merge(first, node), second);
  }

  void UpdateTreeNode(long long index, const long long &val) {
    std::pair p1 = Split(root_, index);
    std::pair p2 = Split(p1.second, 1);
    p2.second->value = val;
    p2.second->delta = val;
    root_ = Merge(p1.first, Merge(p2.first, p2.second));
  }

  long long &operator[](int64_t pos) {
    auto [parent, pos_node] = Find(nullptr, root_, pos);
    return pos_node->value;
  }

 private:
  std::pair<Node *, Node *> Find(Node *parent, Node *node, int64_t pos) {
    if (node == nullptr) {
      return {parent, node};
    }
    int64_t left_size = Size(node->left);
    if (pos == left_size) {
      return {parent, node};
    }
    if (pos < left_size) {
      return Find(node, node->left, pos);
    }
    return Find(node, node->right, pos - left_size - 1);
  }

  void Clear(Node *node) {
    if (node == nullptr) {
      return;
    }
    Clear(node->left);
    Clear(node->right);
    delete node;
  }

  Node *Merge(Node *first, Node *second) {
    if (first == nullptr) {
      return second;
    }
    if (second == nullptr) {
      return first;
    }
    if (first->priority > second->priority) {
      first->right = Merge(first->right, second);
      Update(first->right);
      Update(first);
      return first;
    }
    second->left = Merge(first, second->left);
    Update(first->left);
    Update(second);
    return second;
  }

  std::pair<Node *, Node *> Split(Node *node, int64_t pos) {
    if (node == nullptr) {
      return {nullptr, nullptr};
    }
    int64_t left_size = Size(node->left);
    if (pos <= left_size) {
      auto [left, right] = Split(node->left, pos);
      node->left = right;
      Update(right);
      Update(node);
      return {left, node};
    }
    auto [left, right] = Split(node->right, pos - left_size - 1);
    node->right = left;
    Update(left);
    Update(node);
    return {node, right};
  }

  void Update(Node *node) {
    if (node == nullptr) {
      return;
    }
    node->delta = std::min(DeltaIn(node->left),
                           std::min(DeltaIn(node->right), node->value));
    node->size = 1 + Size(node->left) + Size(node->right);
  }

  int64_t Size(Node *node) {
    if (node == nullptr) {
      return 0;
    }
    return node->size;
  }

  long long DeltaIn(Node *node) {
    if (node == nullptr) {
      return kMaxVal;
    }
    return node->delta;
  }

  Node *root_;
  std::mt19937 gen_;
  std::uniform_int_distribution<int64_t> distribution_;
  const long long kMaxVal = 1000000100;
};

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  char operation[5];
  int n;
  long long x, y;
  std::cin >> n;
  TreapArray *treap = new TreapArray();
  for (int i = 0; i < n; ++i) {
    std::cin >> operation;
    if (operation[0] == '+') {
      std::cin >> x >> y;
      treap->Insert(x, y);
    } else {
      std::cin >> x >> y;
      std::cout << treap->GetMin(x - 1, y - 1) << std::endl;
    }
  }
  delete treap;
  return 0;
}

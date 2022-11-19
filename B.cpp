#include <iostream>

class SegmentTree {
 private:
  const static int kMaxSize = 10000000;
  int size_;
  int tree_[kMaxSize];

 public:
  SegmentTree(int size) {
    size_ = size;
    for (int i = 0; i < size_; ++i) {
      tree_[size_ + i] = 0;
    }
    for (int i = size_ - 1; i > 0; --i) {
      tree_[i] = tree_[i << 1] + tree_[i << 1 | 1];
    }
  }

  int Size() { return size_; }

  void UpdateTreeNode(int index, int value) {
    tree_[index + size_] = value;
    index += size_;
    for (int i = index; i > 1; i >>= 1) {
      tree_[i >> 1] = tree_[i] + tree_[i ^ 1];
    }
  }

  int SumOnSegment(int left, int right) {
    int result = 0;
    for (left += size_, right += size_; left < right; left >>= 1, right >>= 1) {
      if ((bool)(left & 1)) {
        result += tree_[left];
        ++left;
      }
      if ((bool)(right & 1)) {
        --right;
        result += tree_[right];
      }
    }
    return result;
  }
};

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  int n, requests, x, y, z;
  std::cin >> n;
  SegmentTree* segment_odd = new SegmentTree(n);
  SegmentTree* segment_even = new SegmentTree(n);
  for (int i = 0; i < n; ++i) {
    std::cin >> x;
    if (i % 2 == 0) {
      segment_even->UpdateTreeNode(i, x);
      segment_odd->UpdateTreeNode(i, 0);
    } else {
      segment_even->UpdateTreeNode(i, 0);
      segment_odd->UpdateTreeNode(i, x);
    }
  }
  std::cin >> requests;
  for (int i = 0; i < requests; ++i) {
    std::cin >> x >> y >> z;
    if (x == 0) {
      --y;
      if (y % 2 == 0) {
        segment_even->UpdateTreeNode(y, z);
      } else {
        segment_odd->UpdateTreeNode(y, z);
      }
    } else {
      --y;
      if (y % 2 == 0) {
        std::cout << segment_even->SumOnSegment(y, z) -
                         segment_odd->SumOnSegment(y, z)
                  << '\n';
      } else {
        std::cout << segment_odd->SumOnSegment(y, z) -
                         segment_even->SumOnSegment(y, z)
                  << '\n';
      }
    }
  }
  delete segment_odd;
  delete segment_even;
}

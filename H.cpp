#include <iostream>

#include <vector>

#include "algorithm"

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
    tree_[index + size_] += value;
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

int BinarySearch(std::vector<int>& array_of_elements, int left, int right,
                 int target) {
  while ((right - left) > 1) {
    int mid = (left + right) / 2;
    if (array_of_elements[mid] == target) {
      return mid;
    }
    if (array_of_elements[mid] > target) {
      right = mid;
    } else {
      left = mid;
    }
  }
  return left;
}

std::vector<std::pair<int, int>> CompressOfIndexes(int n, std::vector<std::pair<int, int>> a, std::vector<int> b) {
  std::sort(b.begin(), b.end());
  for (int i = 0; i < n; ++i) {
    a[i].first = BinarySearch(b, 0, b.size(), a[i].first);
    a[i].second = BinarySearch(b, 0, b.size(), a[i].second);
  }
  return a;
}

struct Point {
  Point(int i, int val, bool tp) {
    id = i;
    value = val;
    type = tp;
  }
  int id;
  int value;
  bool type;
};

bool cmp(const Point& v, const Point& other) {
  return v.value < other.value;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  int n;
  std::cin >> n;
  SegmentTree* sg = new SegmentTree(n);
  int l, r;
  std::vector<std::pair<int, int>> arr(n);
  std::vector<int> compress_vector;
  for (int i = 0; i < n; ++i) {
    std::cin >> l >> r;
    arr[i] = {l, r};
    if (std::count(compress_vector.begin(), compress_vector.end(), l) == 0) {
      compress_vector.push_back(l);
    }
    if (std::count(compress_vector.begin(), compress_vector.end(), r) == 0) {
      compress_vector.push_back(r);
    }
  }
  arr = CompressOfIndexes(n, arr, compress_vector);
  std::sort(arr.begin(), arr.end());
  std::vector<Point> vec;
  for (int i = 0; i < n; ++i) {
    vec.push_back({i, arr[i].first, true});
    vec.push_back({i, arr[i].second, false});
  }
  long long res = 0;
  std::sort(vec.begin(), vec.end(), cmp);
  for (int i = 0; i < (int)vec.size(); ++i) {
    if (!(vec[i].type)) {
      res += sg->SumOnSegment(arr[vec[i].id].first, arr[vec[i].id].second);
      sg->UpdateTreeNode(arr[vec[i].id].first, 1);
    }
    if (i >= 2) {
      if ((vec[i - 1].value == vec[i - 2].value) && (!(vec[i - 1].type))) {
        res += sg->SumOnSegment(arr[vec[i - 1].id].first, arr[vec[i - 1].id].second);
      }
    }
  }
  std::cout << res;
  delete sg;
}

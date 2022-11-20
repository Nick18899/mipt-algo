#include <iostream>
#include <vector>

#include "algorithm"

class Fenwiktree {
 public:
  std::vector<int> vec;
  int n;

  Fenwiktree(int size) {
    n = size;
    vec.assign(n, 0);
  }

  int Sum(int x) {
    long res = 0;
    for (int i = x; i >= 0; i = (i & (i + 1)) - 1) {
      res += vec[i];
    }
    return res;
  }

  int SumOnSegment(int left, int right) {
    if (left == 0) {
      return Sum(right);
    }
    return Sum(right) - Sum(left - 1);
  }

  void UpdateTreeNode(int index, int delta) {
    for (int i = index; i < n; i |= (i + 1)) {
      vec[i] += delta;
    }
  }
};

struct Segment {
  int index;
  int left;
  int right;
};

bool LeftCmp(const Segment& seg, const Segment& other) {
  return (seg.left > other.left) ||
         (seg.left == other.left && seg.right < other.right);
}

bool RightCmp(const Segment& seg, const Segment& other) {
  return seg.right < other.right;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  int n;
  std::cin >> n;
  std::vector<Segment> arr(n);
  for (int i = 0; i < n; ++i) {
    std::cin >> arr[i].left >> arr[i].right;
  }
  std::sort(arr.begin(), arr.end(), RightCmp);
  int index = 0;
  int prevo = 0;
  for (int i = 0; i < n; ++i) {
    if (arr[i].right != arr[prevo].right) {
      prevo = i;
      ++index;
    }
    arr[i].index = index;
  }
  long long res = 0;
  std::sort(arr.begin(), arr.end(), LeftCmp);
  auto* sg = new Fenwiktree(index + 1);
  Segment previous_seg = {-1, -1, 0};
  long long pans = 0;
  for (auto& i : arr) {
    if (i.left != previous_seg.left || i.right != previous_seg.right) {
      pans = sg->Sum(i.index);
    }
    res += pans;
    sg->UpdateTreeNode(i.index, 1);
    previous_seg = i;
  }
  std::cout << res << std::endl;
  delete sg;
}

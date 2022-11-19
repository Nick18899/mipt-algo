#include <iomanip>
#include <iostream>
#include <vector>

class Fenwiktree {
 public:
  std::vector<double> vec;
  int n;

  Fenwiktree(int size) {
    n = size;
    vec.assign(n, 0);
  }

  double Sum(int x) {
    double res = 0;
    for (int i = x; i >= 0; i = (i & (i + 1)) - 1) {
      res += vec[i];
    }
    return res;
  }

  double SumOnSegment(int left, int right) {
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

void InputTreatment(const char* cmd, Fenwiktree*& segment_tree,
                    std::vector<int>& arr, int& k) {
  int inserting_value;
  std::cin >> inserting_value;
  switch (cmd[0]) {
    case 'C':
      if (arr.at(inserting_value) > 0) {
        if (k == 1) {
          std::cout << 1 << std::endl;
        } else {
          std::cout << std::setprecision(6)
                    << segment_tree->Sum(arr.at(inserting_value) - 1) / (k - 1)
                    << std::endl;
        }
      } else {
        std::cout << 0 << std::endl;
      }
      break;
    case 'R':
      int ins_val_2;
      std::cin >> ins_val_2;
      segment_tree->UpdateTreeNode(ins_val_2, 1);
      if (arr.at(inserting_value) != 0) {
        segment_tree->UpdateTreeNode(arr.at(inserting_value), -1);
      }
      if (arr.at(inserting_value) == 0) {
        ++k;
      }
      arr.at(inserting_value) = ins_val_2;
      break;
    default:
      break;
  }
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  char str[7];
  int n;
  int k = 0;
  std::vector<int> arr;
  std::cin >> n;
  auto* segment_tree = new Fenwiktree(50000);
  int x = 100000 - n;
  arr.assign(n + x + 1, 0);
  for (int i = 0; i < n; ++i) {
    std::cin >> str;
    InputTreatment(str, segment_tree, arr, k);
  }
  delete segment_tree;
}

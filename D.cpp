#include <iostream>
#include "vector"

class Fenwiktree{
 public:
  std::vector<std::vector<std::vector<int>>> vec;
  int n;

  Fenwiktree(int size) {
    n = size;
    vec.assign(n, std::vector<std::vector<int>>(n));
    for (int i = 0; i < size; ++i) {
      vec[i].assign(n, std::vector<int>(n));
      for (int j = 0; j < size; ++j) {
        vec[i][j].assign(n, 0);
      }
    }
  }

  long long Sum(int x, int y, int z) {
    long long res = 0;
    for (int i = x; i >= 0; i = (i & (i+1)) - 1) {
      for (int j = y; j >= 0; j = (j & (j+1)) - 1) {
        for (int k = z; k >= 0; k = (k & (k+1)) - 1) {
          res += vec[i][j][k];
        }
      }
    }
    return res;
  }

  long long SumOnCube(int x1, int y1, int z1, int x2, int y2, int z2) {
    long long sum1 = 0;
    long long sum2 = 0;
    long long sum3 = 0;
    if (x1 != 0 && y1 != 0) {
      sum1 = Sum(x1 - 1, y1 - 1, z2);
    }
    if (y1 != 0) {
      sum2 = Sum(x2, y1 - 1, z2);
    }
    if (x1 != 0) {
      sum3 = Sum(x1 - 1, y2, z2);
    }
    long long pre_res_sum = Sum(x2, y2, z2) + sum1 - sum2 - sum3;
    long long sum4 = 0;
    if (z1 == 0) {
      sum1 = 0;
      sum2 = 0;
      sum3 = 0;
    } else {
      sum1 = sum1 == 0 ? 0 : Sum(x1 - 1, y1 - 1, z1 - 1);
      sum2 = sum2 == 0 ? 0 : Sum(x2, y1 - 1, z1 - 1);
      sum3 = sum3 == 0 ? 0 : Sum(x1 - 1, y2, z1 - 1);
      sum4 = Sum(x2, y2, z1 - 1);
    }
    return pre_res_sum + sum2 + sum3 - sum4 - sum1;
  }

  void UpdateTreeNode (int x, int y, int z, int delta) {
    for (int i = x; i < n; i |= (i+1)) {
      for (int j = y; j < n; j |= (j+1)) {
        for (int k = z; k < n; k |= (k+1)) {
          vec[i][j][k] += delta;
        }
      }
    }
  }
};

int main() {
  int n, m;
  std::cin >> n;
  int arr[6];
  Fenwiktree* ftree = new Fenwiktree(n);
  while(true) {
    std::cin >> m;
    if (m == 3) {
      break;
    }
    switch (m) {
      case 1:
        std::cin >> arr[0] >> arr[1] >> arr[2] >> arr[3];
        ftree->UpdateTreeNode(arr[0], arr[1], arr[2], arr[3]);
        break;
      case 2:
        std::cin >> arr[0] >> arr[1] >> arr[2] >> arr[3] >> arr[4] >> arr[5];
        std::cout << ftree->SumOnCube(arr[0], arr[1], arr[2], arr[3], arr[4], arr[5]) << '\n';
        break;
      default:
        break;
    }
  }
  delete ftree;
  return 0;
}

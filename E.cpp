#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>

class SparseTable {
 public:
  SparseTable(const std::vector<long>& data)
      : n_(data.size()), logs_(data.size() + 1, 0) {
    PrecalcLogs(data.size());
    log_n_ = logs_[n_];
    sparse_.assign(log_n_ + 1,
                   std::vector<std::pair<long, long>>(
                       n_ + 1, std::pair<long, long>(kMaxSize, kMaxSize)));
    BuildSparse(data);
  }

  long Query(size_t l, size_t r) {
    size_t lg = logs_[r - l];
    return Compar(sparse_[lg][l], sparse_[lg][r - (1 << lg) + 1]).second;
  }

 private:
  static std::pair<long, long> Compar(const std::pair<long, long>& x,
                                      const std::pair<long, long>& y) {
    std::vector<long> vec;
    vec.push_back(x.first);
    vec.push_back(x.second);
    vec.push_back(y.first);
    vec.push_back(y.second);
    std::sort(vec.begin(), vec.end());
    long minima = vec[0];
    for (int i = 1; i < 4; ++i) {
      if (vec[i] != minima) {
        return {minima, vec[i]};
      }
    }
    return {minima, minima};
  }

  void BuildSparse(const std::vector<long>& data) {
    for (size_t i = 0; i < data.size(); ++i) {
      sparse_[0][i] = {data[i], kMaxSize};
    }
    for (size_t k = 1; k <= log_n_; ++k) {
      for (size_t i = 0; i + (1 << k) <= n_; ++i) {
        sparse_[k][i] =
            Compar(sparse_[k - 1][i], sparse_[k - 1][i + (1 << (k - 1))]);
      }
    }
  }

  void PrecalcLogs(size_t n) {
    logs_[0] = 0;
    logs_[1] = 0;
    for (size_t i = 2; i <= n; ++i) {
      logs_[i] = logs_[i / 2] + 1;
    }
  }

  const long long kMaxSize = 1000000100;
  size_t n_;
  size_t log_n_;
  std::vector<size_t> logs_;
  std::vector<std::vector<std::pair<long, long>>> sparse_;
};

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  size_t n, m, x, y;
  std::cin >> n >> m;
  std::vector<long> array_of_inputs(n);
  for (size_t i = 0; i < n; ++i) {
    std::cin >> array_of_inputs[i];
  }
  auto* sparce = new SparseTable(array_of_inputs);
  for (size_t i = 0; i < m; ++i) {
    std::cin >> x >> y;
    std::cout << sparce->Query(x - 1, y - 1) << std::endl;
  }
  delete sparce;
  return 0;
}

#include <bits/stdc++.h>
using namespace std;

vector<int> findLIS(const vector<int> &nums) {
  if (nums.empty())
    return {};

  int n = nums.size();
  vector<int> tails;
  vector<int> tails_indices;
  vector<int> parents(n, -1);

  for (int i = 0; i < n; ++i) {
    int pos = lower_bound(tails.begin(), tails.end(), nums[i]) - tails.begin();

    if (pos == tails.size()) {
      tails.push_back(nums[i]);
      tails_indices.push_back(i);
    } else {
      tails[pos] = nums[i];
      tails_indices[pos] = i;
    }

    if (pos != 0) {
      parents[i] = tails_indices[pos - 1];
    }
  }

  int lis_length = tails.size();
  vector<int> lis(lis_length);
  int k = tails_indices.back();

  for (int i = lis_length - 1; i >= 0; --i) {
    lis[i] = nums[k];
    k = parents[k];
  }

  return lis;
}

int main() {
  vector<int> nums = {10, 9, 2, 5, 3, 7, 101, 18};

  vector<int> lis = findLIS(nums);

  cout << "Longest Increasing Subsequence: ";
  for (int num : lis) {
    cout << num << " ";
  }
  cout << endl;

  return 0;
}

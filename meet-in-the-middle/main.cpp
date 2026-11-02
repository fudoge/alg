#include <bits/stdc++.h>
using namespace std;

void generateSubsetSums(const vector<int> &arr, vector<int> &subsetSums) {
  int size = arr.size();
  int numSubsets = 1 << size;
  for (int mask = 0; mask < numSubsets; ++mask) {
    int sum = 0;
    for (int i = 0; i < size; ++i) {
      if (mask & (1 << i)) // i번째 요소가 부분 집합에 포함되면
        sum += arr[i];
    }
    subsetSums.push_back(sum);
  }
}

int main() {
  int n, s;
  n = 40, s = 100; // (예시)
  vector<int> arr(n);
  for (int i = 0; i < n; ++i) {
    arr[i] = i;
  }

  // 배열 나누기
  vector<int> leftArr(arr.begin(), arr.begin() + n / 2);
  vector<int> rightArr(arr.begin() + n / 2, arr.end());

  // 부분 집합 합 계산
  vector<int> leftSums, rightSums;
  generateSubsetSums(leftArr, leftSums);
  generateSubsetSums(rightArr, rightSums);

  // 정렬
  sort(rightSums.begin(), rightSums.end());

  // 이진 탐색으로 대응되는 합 맞춰서 개수 누적
  int ans = 0;
  for (int leftSum : leftSums) {
    int target = s - leftSum;
    ans += upper_bound(rightSums.begin(), rightSums.end(), target) -
           lower_bound(rightSums.begin(), rightSums.end(), target);
  }

  cout << ans << "\n";

  return 0;
}

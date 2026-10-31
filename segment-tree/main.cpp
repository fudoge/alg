#include <bits/stdc++.h>

using namespace std;

int arr[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9}, seg[40];

void build(int index, int l, int r) {
  // leaf
  if (l == r) {
    seg[index] = arr[l];
    return;
  }

  // internal
  int mid = (l + r) >> 1;
  build(index * 2 + 1, l, mid);
  build(index * 2 + 2, mid + 1, r);
  seg[index] = seg[index * 2 + 1] + seg[index * 2 + 2];
}

void update(int index, int target, int val, int l, int r) {
  if (l == r) {
    seg[index] = val;
    return;
  }

  int mid = (l + r) >> 1;
  if (target <= mid) {
    update(index * 2 + 1, target, val, l, mid);
  } else {
    update(index * 2 + 2, target, val, mid + 1, r);
  }
  seg[index] = seg[index * 2 + 1] + seg[index * 2 + 2];
}

int query(int index, int l, int r, int ql, int qr) {
  if (ql > r || qr < l) {
    return 0;
  }

  if (ql <= l && r <= qr) {
    return seg[index];
  }

  int mid = (l + r) >> 1;
  int lres = query(index * 2 + 1, l, mid, ql, qr);
  int rres = query(index * 2 + 2, mid + 1, r, ql, qr);
  return lres + rres;
}

int main() {
  int n = 10;
  build(0, 0, n - 1);

  cout << "Sum of range (2, 8): " << query(0, 0, n - 1, 2, 8) << endl;

  cout << "Updating index 5 to 15..." << endl;
  update(0, 5, 15, 0, n - 1);

  cout << "Sum of range (2, 8) after update: " << query(0, 0, n - 1, 2, 8)
       << endl;

  return 0;
}

#include <bits/stdc++.h>

using namespace std;

int arr[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9}, seg[40], lazy[40];

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

void propagate(int index, int l, int r) {
  if (lazy[index] != 0) {
    seg[index] += lazy[index] * (r - l + 1);
    if (l != r) {
      lazy[index * 2 + 1] += lazy[index];
      lazy[index * 2 + 2] += lazy[index];
    }
    lazy[index] = 0;
  }
}

void update(int index, int l, int r, int ql, int qr, int val) {
  propagate(index, l, r);
  if (ql > r || qr < l) {
    return;
  }
  if (ql <= l && r <= qr) {
    lazy[index] += val;
    propagate(index, l, r);
    return;
  }
  int mid = (l + r) >> 1;
  update(index * 2 + 1, l, mid, ql, qr, val);
  update(index * 2 + 2, mid + 1, r, ql, qr, val);
  seg[index] = seg[index * 2 + 1] + seg[index * 2 + 2];
}

int query(int index, int l, int r, int ql, int qr) {
  propagate(index, l, r);
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

  cout << "Initial sum (0-9): " << query(0, 0, n - 1, 0, 9) << endl;
  update(0, 0, n - 1, 2, 5, 3);
  cout << "After update (5-8) += 2, sum (0-9): " << query(0, 0, n - 1, 0, 9)
       << endl;

  cout << "Query (3-6): " << query(0, 0, n - 1, 3, 6) << endl;

  return 0;
}

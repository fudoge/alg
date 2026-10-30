#include <bits/stdc++.h>
using namespace std;

vector<int> parent, depth, heavy, head, pos;
vector<long long> seg, val, base;

int cur_pos;
int n;

int dfs(int v, const vector<vector<int>> &adj) {
  int size = 1;
  int max_c_size = 0;

  for (int c : adj[v]) {
    if (c == parent[v])
      continue;

    parent[c] = v;
    depth[c] = depth[v] + 1;

    int c_size = dfs(c, adj);
    size += c_size;

    if (c_size > max_c_size) {
      max_c_size = c_size;
      heavy[v] = c;
    }
  }

  return size;
}

void decompose(int v, int h, const vector<vector<int>> &adj) {
  head[v] = h;
  pos[v] = cur_pos++;

  if (heavy[v] != -1) {
    decompose(heavy[v], h, adj);
  }

  for (int c : adj[v]) {
    if (c == parent[v] || c == heavy[v])
      continue;

    decompose(c, c, adj);
  }
}

void seg_init(int idx, int l, int r) {
  if (l == r) {
    seg[idx] = base[l];
    return;
  }

  int mid = (l + r) >> 1;

  seg_init(idx * 2, l, mid);
  seg_init(idx * 2 + 1, mid + 1, r);

  seg[idx] = seg[idx * 2] + seg[idx * 2 + 1];
}

long long seg_query(int idx, int ql, int qr, int l, int r) {
  if (qr < l || r < ql)
    return 0;

  if (ql <= l && r <= qr)
    return seg[idx];

  int mid = (l + r) >> 1;

  return seg_query(idx * 2, ql, qr, l, mid) +
         seg_query(idx * 2 + 1, ql, qr, mid + 1, r);
}

void init(const vector<vector<int>> &adj) {
  n = adj.size();

  parent.resize(n);
  depth.resize(n);
  heavy.assign(n, -1);
  head.resize(n);
  pos.resize(n);

  cur_pos = 0;
  parent[0] = -1;

  dfs(0, adj);
  decompose(0, 0, adj);

  base.resize(n);

  for (int u = 0; u < n; ++u) {
    base[pos[u]] = val[u];
  }

  seg.assign(4 * n, 0);
  seg_init(1, 0, n - 1);
}

long long query(int a, int b) {
  long long res = 0;

  while (head[a] != head[b]) {
    if (depth[head[a]] > depth[head[b]])
      swap(a, b);

    res += seg_query(1, pos[head[b]], pos[b], 0, n - 1);

    b = parent[head[b]];
  }

  if (depth[a] > depth[b])
    swap(a, b);

  res += seg_query(1, pos[a], pos[b], 0, n - 1);

  return res;
}

void seg_update(int idx, int p, long long value, int l, int r) {
  if (l == r) {
    seg[idx] = value;
    return;
  }

  int mid = (l + r) >> 1;

  if (p <= mid)
    seg_update(idx * 2, p, value, l, mid);
  else
    seg_update(idx * 2 + 1, p, value, mid + 1, r);

  seg[idx] = seg[idx * 2] + seg[idx * 2 + 1];
}

void update(int u, long long value) {
  val[u] = value;
  seg_update(1, pos[u], value, 0, n - 1);
}

int main() {
  vector<vector<int>> adj = {{1, 2, 3}, {0, 4, 5}, {0, 6}, {0}, {1}, {1, 7},
                             {2, 8, 9}, {5, 10},   {6},    {6}, {7}};

  val = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};

  init(adj);

  cout << query(1, 9) << '\n';
  update(6, 77);
  cout << query(1, 9) << '\n';

  return 0;
}

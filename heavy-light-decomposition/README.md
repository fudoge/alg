트리에서 두 가지 작업을 한다고 해보자:

- `update(x, k)`: x번째 정점의 값을 k로 변경
  -  `query(a, b)`: 정점 a에서 b까지의 경로에 여러 연산들을 적용. (ex. sum, max등)

(아래 트리에서 정점 왼쪽 위 숫자는 정점의 id, 정점 속 숫자는 값이다.)

작업을 하나 처리하는데 나이브한 경우에서 기본적으로 `O(n)`의 시간복잡도가 걸린다.

그러나, `N`이 10만, 작업을 요청하는 쿼리의 개수 Q가 10만개 이상이라면?

즉, $O(QN)$으로, 시간초과가 걸릴 수 있다.

HLD는 이를 최적화해준다.

- 검색 쿼리는 $O(log^2 N)$
- 값 수정 쿼리는 $O(log N)$

### 핵심 아이디어

heavy edge는 서브트리에서 가장 큰 서브트리를 가진 자식으로 가는 간선이라고 보면 된다.

구성 방법은 다음과 같다:

1. 내려가는 heavy edge를 가지지 않는 각 정점에서 시작
2. heavy edge를 거슬러 위로 올라간다.
3. 루트에 도달하거나 light edge 하나를 지나면 멈춘다.

즉, 다음의 경로들이 나온다:

```text
10 -> 7 -> 5 -> 1 -> 0
8 -> 6 -> 2 -> 0
3 -> 0
4 -> 1
9 -> 6
```

이 체인들은 edge-disjoint하다. 실제 구현에서의 편리함을 위해, 각 경로의 마지막 light edge를 제거하여 vertex-disjoint하게 만든다.

그러면 아래처럼 된다:

```text
0 -> 1 -> 5 -> 7 -> 10 (light edge가 아닌 루트에 의한 종료이므로 여기선 줄어들지않음)
2 -> 6 -> 8
3
4
9
```

#### 거쳐야 하는 Heavy Path의 개수

**어떤 노드이던, 루트까지 O(log N)개의 Heavy Path를 거치면 도달가능하다.** 

트리에서 노드 $v$의 서브트리의 크기를 $S(v)$라고 하자.

다음이 성립한다:

이때, 부모 $v$와 자식 $c$를 연결하는 간선은 다음 조건을 만족하면 heavy edge라고 한다:

만족하지 않는 간선들은 모두 light edge이다.

즉, light edge를 따라 내려갈때 마다 현재 서브트리의 크기는 절반 이하로 줄어든다.

light edge를 $k$번 지난 정점의 서브트리 크기는 

 이고, 서브트리의 크기는 최소 1이므로,

양변에 로그를 취하면

이 된다.

즉, Light Edge를 지나는 횟수는 $O(log N)$으로 제한된다.

Heavy Path내부에서는 Heavy Edge만 이동해서 같은 Path에 있고, 다른 Heavy Path로 이동하려면 반드시 Light Edge를 지나야 한다.

Light Edge는 루트에서 임의의 정점까지 $O(log N)$번만 지날 수 있다.

**즉, 경로상의 Heavy Path의 수는 O(log N)이다. (정확히는 Light Edge의 수보다 하나 많거나 같다)**

그러나, Heavy Edge를 지나는 횟수에는 로그 수준의 제한이 없다. 일자 형태의 경우를 생각해보자.

N-1개의 Heavy edge를 지나지만, 모두 heavy path에 속해서 체인을 바꾸는 횟수는 0번이다.

근데 여기서 의문이 하나 들 수 있다.

"만약, 서브트리 크기가 5짜리가 있고, 양쪽이 각각 2짜리면? heavy edge가 없는거 아닌가?"

유의할 점은, heavy edge는 없어도 증명에 문제가 없다는 것이다. 어디로 가든 절반보다 더 서브트리의 크기가 작아질 것이기 때문이다.

실제 구현에서는 꼭 heavy edge가 서브트리의 절반 이상을 점유하는 서브트리로 가는 간선일 필요도 없다.

그저 가장 큰 서브트리를 가진 쪽으로 이어주면 된다.

**증명에서 절반을 이야기하는 것의 결론은 다음과 같다:**

- Heavy Edge로 이동: 같은 Path에 속하므로 몇 번 이동하든 상관없다
- Light Edge로 이동: 다른 Path로 넘어가는데, 서브트리의 크기는 절반 미만으로 감소한다.

### HLD 실전 적용

가장 대표적인 HLD의 문제는 두 정점 사이의 경로의 최댓값을 구하거나, 합을 구하는 문제이다.

각 heavy chain마다의 segment tree를 만들어서 관리할 수 있다.

예를 들어, 위의 그림에서 정점 1~9까지의 합을 구하고 싶다면,

다음과 같이 쪼갤 수 있다:

```text
1 - 0
2 - 6
9
```

즉, 이렇게 3개의 세그먼트 트리 쿼리를 날리고, 집계하면 된다.

한 경로에서 거치는 Heavy Path는 $O(log N)$ 이다.

세그먼트 트리의 한 번의 쿼리는 $O(log N)$이다.

즉, 검색의 시간복잡도는 $O(log^2 N)$이다.

여기서 최적화가 가능한데, 마지막 공유하는 체인의 경우를 제외하면, 각 체인에서 쿼리하는 구간은 항상 체인의 맨 위부터 현재 정점까지의 Prefix이다.

즉, 각 체인의 Prefix Maximum을 미리 계산하면, 대부분의 구간을 $O(1)$에 처리할 수 있다.

전체 쿼리가 $O(log N)$에 가까워지는 것이다.

대신, 중간에 값이 바뀌는 경우에는 말이 달라진다.

구현을 할 때에는 모든 heavy path마다 세그먼트 트리를 만들지 말고, 단일 세그먼트 트리에서 disjoint segments를 유지하면 된다.

업데이트의 시간복잡도는 세그먼트 트리를 업데이트 하는 것이므로 $O(log N)$이다.

### 초기화 단계

주요 자료구조는 다음과 같다:

- `parent[u]`: u의 부모 정점
- `depth[u]`: 루트에서 u까지의 깊이
- `heavy[u]`: u의 Heavy Child
- `head[u]`: u가 속한 Heavy Path의 최상단 정점
- `pos[u]`: 1차원 배열에서의 u의 인덱스

두 번의 dfs가 이루어진다:

1. `dfs()`: parent, depth, heavy 초기화

- 각 자식들에 대해서
    - `parent`를 자신으로 등록하고, `depth`역시 채워넣는다.
    - 자식에게 `dfs()`를 재귀호출 시켜서, 자식 서브트리의 크기를 알아낸다.
    - 자신의 서브트리의 크기를 누적한다.
    - 가장 큰 자식의 서브트리의 크기를 추적 및 업데이트한다.
    - 또한, 가장 서브트리가 큰 자식은 자신의 heavy에 저장한다.

`heavy[리프노드] = -1`로 유지한다.

2. `decompose()`: head 및 초기화

- 현재 자신의 `head`를 초기화한다. `pos`도 초기화한다.
- 리프노드가 아니라면, 자신의 `heavy` 자식먼저 재귀호출한다. 같은 `head`를 공유하므로 `head`값을 바로 내려준다.
- `light`한 자식들은 `light`한 자식 자체가 새로운 `head`로 출발한다. `head`를 `c`로 초기화한다.

```cpp
vector<int> parent, depth, heavy, head, pos;
int cur_pos;

int dfs(int v, vector<vector<int>> const &adj) {
  int size = 1;
  int max_c_size = 0;
  for (int c : adj[v]) {
    if (c != parent[v]) {
      parent[c] = v, depth[c] = depth[v] + 1;
      int c_size = dfs(c, adj);
      size += c_size;
      if (c_size > max_c_size)
        max_c_size = c_size, heavy[v] = c;
    }
  }

  return size;
}

void decompose(int v, int h, vector<vector<int>> const &adj) {
  head[v] = h, pos[v] = cur_pos++;
  if (heavy[v] != -1) {
    decompose(heavy[v], h, adj);
  }
  for (int c : adj[v]) {
    if (c != parent[v] && c != heavy[v]) {
      decompose(c, c, adj);
    }
  }
}

void init(vector<vector<int>> const &adj) {
  int n = adj.size();
  parent = vector<int>(n);
  depth = vector<int>(n);
  heavy = vector<int>(n, -1);
  head = vector<int>(n);
  pos = vector<int>(n);
  cur_pos = 0;

  dfs(0, adj);
  decompose(0, 0, adj);
}
```

위의 그림을 예시로 출력 후 검증해보자:

```cpp
int main(int argc, char *argv[]) {
  vector<vector<int>> adj = {{1, 2, 3}, {0, 4, 5}, {0, 6}, {0}, {1}, {1, 7},
                             {2, 8, 9}, {5, 10},   {6},    {6}, {7}};

  init(adj);
  for (int i = 0; i < 11; i++) {
    cout << "vertex " << i << ": \n";
    cout << "  parent: " << parent[i] << "\n";
    cout << "  depth: " << depth[i] << "\n";
    cout << "  heavy: " << heavy[i] << "\n";
    cout << "  head: " << head[i] << "\n";
    cout << "  pos: " << pos[i] << "\n";
  }

  return 0;
}
```

총 5개의 heavy path가 계산된다.

| Head | 경로                   | pos범위    |
| ---- | ---------------------- | ---------- |
| 0    | 0 -> 1 -> 5 -> 7 -> 10 | \[0, 4\]   |
| 4    | 4                      | \[5, 5\]   |
| 2    | 2 -> 6 -> 8            | \[6, 8\]   |
| 9    | 9                      | \[9, 9\]   |
| 3    | 3                      | \[10, 10\] |

pos배열에는 다음과 같이 담긴다:
`[ 0, 1, 5, 8, 10, 4, 2, 6, 8, 9, 3 ]`

항상 범위 내 맨 앞이 head이다.

### **쿼리**

```cpp
long long query(int a, int b) {
    long long res = 0;
    for (; head[a] != head[b]; b = parent[head[b]]) {
        if (depth[head[a]] > depth[head[b]])
            swap(a, b);
        long long cur = segment_tree_query(pos[head[b]], pos[b]);
        res += cur;
    }

    if (depth[a] > depth[b])
        swap(a, b);

    res += segment_tree_query(pos[a], pos[b]);

    return res;
}
```

정점 `a`와 `b` 사이의 경로상의 합계를 구한다고 해보자. 이제는 어떻게 쿼리하느냐이다.

- 정점 `a`와 `b`의 head가 다른동안, 즉 서로 다른 path에 있는 동안은, 다음의 누적을 한다:
    - 둘중 `head`가 더 깊은 쪽을 `b`라고 하자. 만약 `a`가 더 깊었다면 서로 바꾸자.
    - segment tree로 구간 쿼리(`pos[head[b]] ~ pos[b]`)하고, 그 값을 누적한다.
    - 그 뒤, `b`를 `b`의 `head`의 부모로 업데이트한다.

이제, `a`와 `b`는 같은 체인에 있다. `a ~ b`의 구간쿼리까지 누적하고 리턴하면 된다.

### 값 업데이트

값을 업데이트 하는 부분은 간단하다. 세그먼트 트리의 업데이트의 wrapper만 해주면 된다.

### **구현(C++)**

실제 세그먼트 트리까지 붙이고, 쿼리를 날려보자:

```cpp
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
```

실행까지 해보자. 구간 질이의 값 업데이트까지 잘 되는 걸 볼 수 있다.

```text
23
93
```

### References

[https://cp-algorithms.com/graph/hld.html](https://cp-algorithms.com/graph/hld.html)

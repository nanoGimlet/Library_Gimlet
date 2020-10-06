// UnionFind
struct UnionFind {
    vector<int> d;
    UnionFind(int n=0): d(n,-1) {}

    // ある要素がどの集合に属しているかを返す
    int find(int x) {
      if (d[x] < 0) return x;
      return d[x] = find(d[x]);
    }

    // 集合をマージする、すでに同じ集合ならfalseが返る
    bool unite(int x, int y) {
      x = find(x); y = find(y);
      if (x == y) return false;
      if (d[x] > d[y]) swap(x,y);
      d[x] += d[y];
      d[y] = x;
      return true;
    }

    // 2つの要素が同じ集合か判定する
    bool same(int x, int y) { return find(x) == find(y);}
  
    // ある集合の大きさを答える
    int size(int x) { return -d[find(x)];}
};
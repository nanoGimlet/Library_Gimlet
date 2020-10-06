struct Strongly_Connected_Component {
  int n;
  vector< vector<int> > G, rG, res, emt; // G:グラフ, rG:逆辺, res:DAGの辺
  vector<int> vs, cmp, used;  // vs:逆から頂点を探索していく
  Strongly_Connected_Component(int newv = 1) {
    n = newv;
    G.resize(n);
    rG.resize(n);
    used.resize(n, 0);
    cmp.resize(n, -1);
  }

  // グラフを構築
  void add_edge(int from, int to) {
    G[from].push_back(to);
    rG[to].push_back(from);
  }

  // 1回目のDFSで頂点の探索の優先順位を付ける
  void dfs(int v) {
    used[v] = true;
    for(auto to : G[v]) {
      if(!used[to]) dfs(to);
    }
    vs.push_back(v);
  }

  // 逆辺についてDFSしていく
  void rdfs(int v, int k) {
    used[v] = true;
    cmp[v] = k;
    for(auto to : rG[v]) {
      if(!used[to]) rdfs(to, k);
    }
  }

  bool issame(int l, int r) { return cmp[l] == cmp[r]; }

  int build() {
    used.assign(n, 0);
    vs.clear();
    for(int v = 0; v < n; ++v) if(!used[v]) dfs(v);
    used.assign(n, 0);
    int k = 0;
    for(int i = (int)vs.size()-1; i >= 0; --i) {
      if(!used[vs[i]]) rdfs(vs[i], k++);
    }

    res.resize(k);
    for(int i = 0; i < n; ++i) {
      for(auto& to : G[i]) {
        if(!issame(i, to)) res[cmp[i]].push_back(cmp[to]);
      }
    }
    for(int i = 0; i < k; ++i) {
      sort(res[i].begin(), res[i].end());
      res[i].erase(unique(res[i].begin(), res[i].end()), res[i].end());
    }

    emt.resize(k);
    for(int i = 0; i < n; ++i) {
        emt[cmp[i]].push_back(i);
    }
    return k;
  }

  // DAGを構築する際の辺
  vector< vector<int> > graph() { return res; };
  // 強連結成分分解後の要素
  vector< vector<int> > vertices() { return emt; };
};
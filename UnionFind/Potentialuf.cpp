template<class Abel> struct UnionFind {
    vector<int> par;
    vector<int> rank;
    vector<Abel> diff_weight;

    UnionFind(int n = 1, Abel SUM_UNITY = 0) {
        init(n, SUM_UNITY);
    }

    void init(int n = 1, Abel SUM_UNITY = 0) {
        par.resize(n); rank.resize(n); diff_weight.resize(n);
        for(int i = 0; i < n; ++i) {
            par[i] = i; rank[i] = 0; diff_weight[i] = SUM_UNITY;
        }
    }

    int find(int x) {
        if(par[x] == x) return x;
        else {
            int r = find(par[x]);
            diff_weight[x] += diff_weight[par[x]];  // 累積和を取る
            return par[x] = r;
        }
    }

    Abel weight(int x) {
        find(x);
        return diff_weight[x];
    }

    bool same(int x, int y) {
        return find(x) == find(y);
    }

    bool unite(int x, int y, Abel w) {
        w += weight(x); w -= weight(y); // xとyそれぞれについて、rootとの重み差分を補正
        x = find(x); y = find(y);
        if(x == y) return false;
        if(rank[x] < rank[y]) swap(x, y), w = -w;   // rank[x] >= rank[y]になるようにswap
        if(rank[x] == rank[y]) ++rank[x];
        par[y] = x;
        diff_weight[y] = w; // xがyの親になるので、xとyの差分をdiff_weight[y]に記録
        return true;
    }

    // 重みを取得する
    Abel diff(int x, int y) {
        return weight(y) - weight(x);
    }
};
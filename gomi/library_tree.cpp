#include <bits/stdc++.h>
using namespace std;

// Dijkstraのときのpriority_queの優先順位の付け方
struct pqdata {
    int island; long long cost;
    bool operator<(const pqdata &r) const { return cost > r.cost; }
};

// UnionFindの実装
struct UnionFind {
    vector<int> data;
    UnionFind(int size) : data(size, -1) { }

    // 集合をマージする
    // すでに同じ集合ならfalseが返る
    bool merge(int x, int y) {
        x = root(x); y = root(y);
        if(x == y) return false;
        //要素数の少ない方を多い方に繋げる
        if(data.at(y) < data.at(x)) swap(x, y);
        data.at(x) += data.at(y);
        data.at(y) = x;
        return true;
    }

    // ある要素がどの集合に属しているかを返す
    int root(int x) {
        // 根に直接つなぎ直す
        return data.at(x) < 0 ? x : (data.at(x) = root(data.at(x)));
    }

    // ある集合の大きさを答える
    int size(int x) {
        return -data.at(root(x));
    }

    // ある二つの要素が同じ集合か判定
    bool same(int x, int y) {
        x = root(x); y = root(y);
        return x == y;
    }
};

int main() {

}
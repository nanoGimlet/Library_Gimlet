#include <bits/stdc++.h>
using namespace std;
template<class T> inline bool chmax(T& a, T b) { if (a < b) { a = b; return true; } return false; }
template<class T> inline bool chmin(T& a, T b) { if (a > b) { a = b; return true; } return false; }
using ll = long long;
using P = pair<ll, ll>;
const long double PI = acos(-1.0L);
ll GCD(ll a, ll b) { return b?GCD(b, a%b):a; }
ll LCM(ll a, ll b) { return a/GCD(a, b)*b; }

template< typename sum_t, typename key_t >
struct Rerooting {
    struct Edge {
        int to;
        key_t data;
        sum_t dp, ndp;
    };
    
    using F = function< sum_t(sum_t, sum_t) >;
    using G = function< sum_t(sum_t, key_t) >;
    
    vector< vector<Edge> > g;
    vector<sum_t> subdp, dp;
    const sum_t ident;
    const F f;
    const G gg;

    Rerooting(int V, const F f, const G g, const sum_t &ident)
        : g(V), f(f), gg(g), ident(ident), subdp(V, ident), dp(V, ident) {}
    
    void add_edge(int u, int v, const key_t &d) {
        g[u].emplace_back((Edge){v, d, ident, ident});
        g[v].emplace_back((Edge){u, d, ident, ident});
    }
    void add_edge_bi(int u, int v, const key_t &d, const key_t &e) {
        g[u].emplace_back((Edge){v, d, ident, ident});
        g[v].emplace_back((Edge){u, e, ident, ident});
    }

    void dfs_sub(int idx, int par) {
        for(auto &e : g[idx]) {
            if(e.to == par) continue;
            dfs_sub(e.to, idx);
            subdp[idx] = f(subdp[idx], gg(subdp[e.to], e.data));
        }
    }
    void dfs_all(int idx, int par, const sum_t &top) {
        sum_t buff{ident};
        for(int i = 0; i < (int)g[idx].size(); ++i) {
            auto &e = g[idx][i];
            e.ndp = buff;
            e.dp = gg(par == e.to ? top:subdp[e.to], e.data);
            buff = f(buff, e.dp);
        }
        dp[idx] = buff;
        buff = ident;
        for(int i = (int)g[idx].size()-1; i >= 0; --i) {
            auto &e = g[idx][i];
            if(e.to != par) dfs_all(e.to, idx, f(e.ndp, buff));
            e.ndp = f(e.ndp, buff);
            buff = f(buff, e.dp);
        }
    }

    vector<sum_t> build() {
        dfs_sub(0, -1);
        dfs_all(0, -1, ident);
        return dp;
    }
};

int main() {
    int n; cin >> n;
    auto f1 = [](int a, int b) { return max(a, b); };   // mergeする関数
    auto f2 = [](int a, int data) { return a+data; };   // fは親に対する関数

    Rerooting<int, int> g(n, f1, f2, 0);    // 単位元はmege関数の単位元
    for(int i = 1; i < n; ++i) {
        int s, t; cin >> s >> t;
        s--; t--;
        g.add_edge(s, t, 1);
    }

    g.build();
    for(auto &p : g.dp) {
        cout << (n-1)*2-p << endl;
    }
}
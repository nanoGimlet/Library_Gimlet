// Tenka2015
#include <bits/stdc++.h>
using namespace std;
template<class T> inline bool chmax(T& a, T b) { if (a < b) { a = b; return true; } return false; }
template<class T> inline bool chmin(T& a, T b) { if (a > b) { a = b; return true; } return false; }
using ll = long long;
using P = pair<ll, ll>;
ll GCD(ll a, ll b) { return b?GCD(b, a%b):a; }
ll LCM(ll a, ll b) { return a/GCD(a, b)*b; }

#define inf 2000000000  // 1e17
template<class T>
struct Dinic {
    // 辺を表す構造体(行き先、容量、逆辺のid)
    struct edge {
        int to;
        T cap;
        int rev;
    };
    
    vector< vector<edge> > G;   // グラフの隣接リスト表現
    vector<int> level;          // sからの距離
    vector<int> iter;           // どこまで調べ終わったか
    Dinic(int n = 1) {
        G.clear();
        G.resize(n);
        level.resize(n);
        iter.resize(n);
    }

    // fromからtoへ向かう容量capの辺をグラフに追加する
    bool add_edge(int from, int to, T capacity) {
        G[from].push_back((edge){to, capacity, (int)G[to].size()});
        G[to].push_back((edge){from, 0, (int)G[from].size()-1});
        return 1;
    }

    // sからの最短距離をBFSで計算する
    void bfs(int s) {
        level.assign(level.size(), -1);
        queue<int> que;
        level[s] = 0;
        que.push(s);
        while(!que.empty()) {
            int now = que.front(); que.pop();
            for(auto e : G[now]) {
                if(e.cap > 0 && level[e.to] < 0) {
                    level[e.to] = level[now] + 1;
                    que.push(e.to);
                }
            }
        }
    }

    // 増加パスをDFSで探す
    T pathdfs(int now, int t, T nf) {
        if(now == t) return nf;
        for(int &i = iter[now]; i < (int)G[now].size(); ++i) {
            edge *e = &G[now][i];
            if(e->cap > 0 && level[now] < level[e->to]) {
                T res = pathdfs(e->to, t, min(nf, e->cap));
                if(res > 0) {
                    e->cap -= res;
                    G[e->to][e->rev].cap += res;
                    return res;
                }
            }
        }
        return 0;
    }

    // sからtへお最大流を求める
    T max_flow(int s, int t) {
        T res = 0, nf = 0;
        while(1) {
            bfs(s);
            if(level[t] < 0) return res;
            iter.assign(iter.size(), 0);
            while((nf = pathdfs(s, t, inf)) > 0) res += nf;
        }
        return -1;
    }
};

int n, m;
int dx[] = {-1, 0, 1, 0};
int dy[] = {0, -1, 0, 1};

int main() {
    cin >> n >> m;
    vector< vector<int> > Amap(n, vector<int>(m, 0));
    for(int i = 0; i < n; ++i) {
        for(int j = 0; j < m; ++j) {
            cin >> Amap.at(i).at(j);
        }
    }
    vector< vector<int> > Bmap(n, vector<int>(m, 0));
    for(int i = 0; i < n; ++i) {
        for(int j = 0; j < m; ++j) {
            cin >> Bmap.at(i).at(j);
        }
    }

    // 黒から白にするべきなマスと白から黒にするべきマスが上下左右に有れば1個ずつ入れ替えるより効率がいい
    // 上の2種類を最大で何個作れるかマッチングする
    // sからコスト1の辺を引く、tへコスト1の辺を引くことを忘れない

    Dinic<int> din(n*m + 2);    // +2はs,tを追加、sはn*m, tはn*m+1に割り当て
    int ans = 0;
    for(int i = 0; i < n; ++i) {
        for(int j = 0; j < m; ++j) {
            if(Amap[i][j] != Bmap[i][j]) {
                ++ans;          // ansは1個ずつ入れ替えた場合の回数
                if(Amap[i][j]) {
                    din.add_edge(n*m, i*m+j, 1);    // sから黒色に対してコスト1の辺
                    for(int k = 0; k < 4; ++k) {
                        int nx = i+dx[k]; int ny = j+dy[k];
                        if(nx >= 0 && nx < n && ny >= 0 && ny < m) {
                            if(Amap[nx][ny] != Bmap[nx][ny]) {
                                if(!Amap[nx][ny]) {
                                    // Aで白だけどBで黒にしなければいけないやつと辺を結ぶ
                                    din.add_edge(i*m+j, nx*m+ny, 1);
                                }
                            }
                        }
                    }
                }else {
                    // 白色からtに対してコスト1の辺を結ぶ
                    din.add_edge(i*m+j, n*m+1, 1);
                }
            }
        }
    }
    // 求めたいのはs, tの二部マッチング
    cout << ans - din.max_flow(n*m, n*m+1) << endl;
}
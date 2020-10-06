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
            edge &e = G[now][i];
            if(e.cap > 0 && level[now] < level[e.to]) {
                T res = pathdfs(e.to, t, min(nf, e.cap));
                if(res > 0) {
                    e.cap -= res;
                    G[e.to][e.rev].cap += res;
                    return res;
                }
            }
        }
        return 0;
    }

    // sからtへ最大流を求める
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
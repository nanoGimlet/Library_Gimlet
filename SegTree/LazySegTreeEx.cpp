#include <bits/stdc++.h>
using namespace std;
template<class T> inline bool chmax(T& a, T b) { if (a < b) { a = b; return true; } return false; }
template<class T> inline bool chmin(T& a, T b) { if (a > b) { a = b; return true; } return false; }
using ll = long long;
using P = pair<ll, ll>;
ll GCD(ll a, ll b) { return b?GCD(b, a%b):a; }
ll LCM(ll a, ll b) { return a/GCD(a, b)*b; }

template<typename T, typename E>
struct LazeSegTree {
    // 0-indexed
    typedef function<T(T, T)> F;
    typedef function<T(T, E)> G;
    typedef function<E(E, E)> H;
    int n, height;
    F f;
    G g;
    H h;
    T ti;
    E ei;
    vector<T> dat;
    vector<E> laz;
    LazeSegTree(F f, G g, H h, T ti, E ei): 
        f(f), g(g), h(h), ti(ti), ei(ei) {}

    void init(int n_) {
        n = 1, height = 0;
        while(n < n_) n <<= 1, ++height;
        dat.assign(n<<1, ti);
        laz.assign(n<<1, ei);
    }

    void build(const vector<T> &v) {
        int n_ = v.size();
        init(n_);
        for(int i = 0; i < n_; ++i) dat[n+i] = v[i];
        for(int i = n-1; i; --i) dat[i] = f(dat[(i<<1)|0], dat[(i<<1)|1]);
    }

    inline T reflect(int k) {
        return laz[k] == ei ? dat[k] : g(dat[k], laz[k]);
    }

    inline void propagate(int k) {
        if(laz[k] == ei) return;
        laz[(k<<1)|0] = h(laz[(k<<1)|0], laz[k]);
        laz[(k<<1)|1] = h(laz[(k<<1)|1], laz[k]);
        dat[k] = reflect(k);
        laz[k] = ei;
    }

    inline void thrust(int k) {
        for(int i = height; i; --i) propagate(k>>i);
        dat[k] = reflect(k);
        laz[k] = ei;
    }

    inline void recalc(int k) {
        while(k >>= 1) dat[k] = f(reflect((k<<1)|0), reflect((k<<1)|1));
    }

    void update(int a, int b, E x) {
        if(a >= b) return;
        thrust(a += n);
        thrust(b += n-1);
        for(int l = a, r = b+1; l < r; l >>= 1, r >>= 1) {
            if(l&1) laz[l] = h(laz[l], x), l++;
            if(r&1) --r, laz[r] = h(laz[r], x);
        }
        recalc(a);
        recalc(b);
    }

    void set_val(int a, T x) {
        thrust(a += n);
        dat[a] = x; laz[a] = ei;
        recalc(a);
    }

    T query(int a, int b) {
        if(a >= b) return ti;
        thrust(a += n);
        thrust(b += n-1);
        T vl = ti, vr = ti;
        for(int l = a, r = b+1; l < r; l >>= 1, r >>= 1) {
            if(l&1) vl = f(vl, reflect(l++));
            if(r&1) vr = f(reflect(--r), vr);
        }
        return f(vl, vr);
    }

    template<typename C>
    int find(int st, C &check, T &acc, int k, int l, int r) {
        if(l+1 == r) {
            acc = f(acc, reflect(k));
            return check(acc) ? k-n : -1;
        }
        propagate(k);
        int m = (l+r)>>1;
        if(m <= st) return find(st, check, acc, (k<<1)|1, m, r);
        if(st <= 1 && !check(f(acc, dat[k]))) {
            acc = f(acc, dat[k]);
            return -1;
        }
        int vl = find(st, check, acc, (k<<1)|0, l, m);
        if(~vl) return vl;
        return find(st, check, acc, (k<<1)|1, m, r);
    }

    template<typename C>
    int find(int st, C &check) {
        T acc = ti;
        return find(st, check, acc, 1, 0, n);
    }
};

int main() {
    int n, q; cin >> n >> q;
    // f:ノードをpairで管理し、fstにクエリ処理の値、sndに長さを格納する
    // g:遅延処理を実行する、今回は和なので長さ*更新の値を追加するg(dat[i], laz[i])のイメージ
    // h:遅延処理の値を新しいのに更新する
    auto f = [](P l, P r) { return P(l.first+r.first, l.second+r.second); };
    auto g = [](P l, ll r) { return P(r*l.second, l.second); };
    auto h = [](ll l, ll r) { return r; };

    LazeSegTree<P, ll> lseg(f, g, h, P(0, 0), -10000);
    // for(int i = 0; i < n; ++i) lseg.set_val(i, P(0, 1));
    lseg.build(vector<P>(n, P(0, 1)));
    for(int i = 0; i < q; ++i) {
        ll c, s, t, x;
        cin >> c;
        if(c) {
            cin >> s >> t;
            cout << lseg.query(s, t+1).first << endl;
        }else {
            cin >> s >> t >> x;
            lseg.update(s, t+1, x);
        }
    }
}
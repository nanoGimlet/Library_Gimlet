template <typename T, typename E>
struct LazeSegTree {
    // 0-indexed
    typedef function<T(T, T)> F;
    typedef function<T(T, E)> G;
    typedef function<E(E, E)> H;
    int n, height;
    F f;    // クエリ処理の関数
    G g;    // 遅延伝搬を反映させる関数
    H h;    // 遅延処理の更新を行う関数
    T ti;   // クエリ処理の単位元
    E ei;   // 遅延配列の単位元
    vector<T> dat;
    vector<E> laz;
    LazeSegTree(){}
    LazeSegTree(F f, G g, H h, T ti, E ei): 
        f(f), g(g), h(h), ti(ti), ei(ei) {}
    
    // 全体を初期化
    void init(int n_) {
        n = 1, height = 0;
        while(n < n_) n <<= 1, ++height;
        dat.assign(n<<1, ti);
        laz.assign(n<<1, ei);
    }

    // 初期配列をセグ木の葉に
    void build(const vector<T> &v) {
        int n_ = v.size();
        init(n_);
        for(int i = 0; i < n_; ++i) dat[n+i] = v[i];
        for(int i = n-1; i; --i) dat[i] = f(dat[(i<<1)|0], dat[(i<<1)|1]);
    }

    // 遅延伝搬の処理を行う
    inline T reflect(int k) {
        return laz[k] == ei ? dat[k] : g(dat[k], laz[k]);
    }

    // 遅延を子ノードに伝搬
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

    // クエリ処理をマージ
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

    // buildをバラバラに行う
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

    template <typename C>
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

    template <typename C>
    int find(int st, C &check) {
        T acc = ti;
        return find(st, check, acc, 1, 0, n);
    }
};
using CP = complex<long double>;

const long double EPS = 1e-10;     // 許容する誤差ε
#define EQ(a, b) (abs((a)-(b)) < EPS)   // 2つのスカラーが等しいかどうか
#define EQV(a, b) (EQ((a).real(), (b).real()) && EQ((a).imag(), (b).imag()))    // 2つのベクトルが等しいかどうか

// double length = abs(a);     // ベクトルaの絶対値
// double distance = abs(a-b);     // 2点a,b間の距離
// CP b = a/abs(a);    // ベクトルaの単位ベクトル
// CP n1 = a*CP(0,+1); CP n2 = a*CP(0,-1);  // ベクトルaの法線ベクトル
// CP un1 = (a*CP(0,+1)/abs(a)); CP un2 = (a*CP(0,-1)/abs(a));  // ベクトルaの単位法線ベクトル

// 内積(dot product) : a・b = |a||b|cosΘ
long double dot(CP a, CP b) {
    return (a.real()*b.real() + a.imag()*b.imag());
}

// 外積(cross product) : a×b = |a||b|sinΘ
long double cross(CP a, CP b) {
    return (a.real()*b.imag() - a.imag()*b.real());
}

// 直線の表現
struct Segment {
    CP s, t;
    Segment(long double sx = 0.0L, long double sy = 0.0L,
            long double tx = 0.0L, long double ty = 0.0L)
            : s(CP(sx, sy)), t(CP(tx, ty)) {}
    Segment(CP _s, CP _t) : s(_s), t(_t) {}
}; typedef Segment Line;

// 2直線の直交判定 : a⊥b ⇔ dot(a,b) = 0
int is_orthogonal(CP a1, CP a2, CP b1, CP b2) {
    return EQ(dot(a1-a2, b1-b2), 0.0);
}

// 2直線の平行判定 : a//b ⇔ cross(a,b) = 0
int is_parallel(CP a1, CP a2, CP b1, CP b2) {
    return EQ(cross(a1-a2, b1-b2), 0.0);
}

// 点cが直線a,b上にあるかないか
int is_point_on_line(CP a, CP b, CP c) {
    return EQ(cross(b-a, c-a), 0.0);
}

// 点cが線分a,b上にあるかないか
int is_point_on_lines(CP a, CP b, CP c) {
    // |a-c|+|c-b| <= |a-b|なら線分上
    return (abs(a-c)+abs(c-b) < abs(a-b)+EPS);
}

// a1,a2を端点とする線分とb1,b2を端点とする線分の交差判定
int is_intersected_lines(CP a1, CP a2, CP b1, CP b2) {
    if(is_parallel(a1, a2, b1, b2)) {
        // 平行なので線分の重なり判定
        return is_point_on_lines(a1, a2, b1) || is_point_on_lines(a1, a2, b2) ||
               is_point_on_lines(b1, b2, a1) || is_point_on_lines(b1, b2, a2);
    }
    return (cross(a2-a1, b1-a1)*cross(a2-a1, b2-a1) < EPS) &&
           (cross(b2-b1, a1-b1)*cross(b2-b1, a2-b1) < EPS);
}

// a1,a2を端点とする線分とb1,b2を端点とする線分の交点計算
CP intersection_lines(CP a1, CP a2, CP b1, CP b2) {
    CP b = b2-b1;
    long double d1 = abs(cross(b, a1-b1));
    long double d2 = abs(cross(b, a2-b1));
    long double t = d1/(d1+d2);
    return a1+(a2-a1)*t;
}

// a1,a2を通る直線とb1,b2を通る直線の交差判定
int is_intersected_line(CP a1, CP a2, CP b1, CP b2) {
    return !EQ(cross(a1-a2, b1-b2), 0.0);
}

// a1,a2を通る直線とb1,b2を通る直線の交点計算(平行ではない前提)
CP intersection_line(CP a1, CP a2, CP b1, CP b2) {
    CP a = a2-a1; CP b = b2-b1;
    return a1 + a*cross(b, b1-a1)/cross(b, a);
}

// 点a,bを通る直線と点cとの距離
long double distance_line_p(CP a, CP b, CP c) {
    return abs(cross(b-a, c-a))/abs(b-a);
}

// 点a,bを端点とする線分と点cとの距離
long double distance_lines_p(CP a, CP b, CP c) {
    if(dot(b-a, c-a) < EPS) return abs(c-a);
    if(dot(a-b, c-b) < EPS) return abs(c-b);
    return abs(cross(b-a, c-a))/abs(b-a);
}

// 点a1,a2を端点とする線分と点b1,b2を端点とする線分の最短距離
long double distance_lines_lines(CP a1, CP a2, CP b1, CP b2) {
    long double res = 1e18;
    if(is_intersected_lines(a1, a2, b1, b2)) return 0.0L;
    res = min(res, distance_lines_p(a1, a2, b1));
    res = min(res, distance_lines_p(a1, a2, b2));
    res = min(res, distance_lines_p(b1, b2, a1));
    res = min(res, distance_lines_p(b1, b2, a2));
    return res;
}

// s,tを通る直線に対する点pの射影
CP projection(CP s, CP t, CP p) {
    if(EQV(s, t)) return s;
    CP base = t-s;
    return s + base*(dot(p-s, base)/norm(base));
}

// s,tを通る直線に対する点pの反射
CP reflection(CP s, CP t, CP p) {
    CP tmp = projection(s, t, p) - p;
    return p + tmp*2.0L;
}

// 角度Θ回転
CP translate(CP v, long double theta) {
    CP res = v * CP(cosl(theta), sinl(theta));
    return res;
}

// n多角形の面積計算
long double polygon_area(const vector<CP> &v) {
    int n = v.size();
    long double res = 0;
    for(int i = 0; i < n; ++i) {
        res += cross(v[(i+n-1)%n], v[(i+n)%n]);
    }
    return fabsl(res)/2.0L;
}

// n多角形の凸性判定
int is_convex(const vector<CP> &v) {
    int n = v.size();
    for(int i = 0; i < n; ++i) {
        if(cross(v[(i+1)%n]-v[i], v[(i+2)%n]-v[(i+1)%n]) < -EPS) return 0;
    }
    return 1;
}

// 凸包(全ての頂点が一直線に並ぶときは例外)
vector<CP> convex_hull(vector<CP> &v) {
    // x座標→y座標で頂点をソート
    auto lmd = [&](const CP &l, const CP &r) {
        if(l.imag() != r.imag()) return l.imag() < r.imag();
        return l.real() < r.real();
        return l.imag() < r.imag();
    };
    vector<CP> res;     // 凸包を構成する頂点
    int n = v.size();
    sort(v.begin(), v.end(), lmd);
    bool flgx = true;
    for(int i = 0; i < n; ++i) {
        if(v[i].real() != v[(i+1)%n].real()) {
            flgx = false;
            break;
        }
    }
    bool flgy = true;
    for(int i = 0; i < n; ++i) {
        if(v[i].imag() != v[(i+1)%n].imag()) {
            flgy = false;
            break;
        }
    }
    if(flgx || flgy) {
        res.emplace_back(v[0]);
        res.emplace_back(v[n-1]);
        return res;
    }
    int k = 0;
    res.resize(n*2);
    // 下側凸包
    for(int i = 0; i < n; ++i) {
        while(k > 1 && cross(res[k-1]-res[k-2], v[i]-res[k-1]) < 0) {
            --k;
        }
        res[k++] = v[i];
    }
    // 上側凸包
    for(int i = n-2, t = k; i >= 0; --i) {
        while(k > t && cross(res[k-1]-res[k-2], v[i]-res[k-1]) < 0) {
            --k;
        }
        res[k++] = v[i];
    }
    res.resize(k-1);
    return res;
}

// 凸多角形の直径
long double convex_diameter(vector<CP> &v) {
    vector<CP> cv = convex_hull(v);
    int n = cv.size();
    if(n == 2) return abs(cv[0]-cv[1]);     // 凸包が潰れている例外処理
    int i = 0, j = 0;   // x軸方向に最も遠い点対
    for(int k = 0; k < n; ++k) {
        if(cv[k].real() < cv[i].real()) i = k;
        if(cv[k].real() > cv[j].real()) j = k;
    }
    long double res = 0;
    int si = i, sj = j;
    while(i != sj || j != si) {     // 180度回転で終了
        res = max(res, abs(cv[i]-cv[j]));
        if(cross(cv[(i+1)%n]-cv[i], cv[(j+1)%n]-cv[j]) < 0) {
            (++i) %= n;
        }else {
            (++j) %= n;
        }
    }
    return res;
}

// 凸多角形を点s,tを通る直線で切断(左側が取得できる)
vector<CP> convex_cut(const vector<CP> &v, const CP &s, const CP &t) {
    vector<CP> res;
    int n = v.size();
    for(int i = 0; i < n; ++i) {
        CP nows = v[i], nowt = v[(i+1)%n];
        if(cross(t-s, nows-s) >= -EPS) res.emplace_back(nows);
        if(cross(t-s, nows-s)*cross(t-s, nowt-s) < 0) {
            res.emplace_back(intersection_line(s, t, nows, nowt));
        }
    }
    return res;
}

// n多角形に対する点pの包含関係(自己交差多角形は例外)
int contain_polygon_point(const vector<CP> &v, CP p) {
    int contain = 0, on_segment = 0;
    int n = v.size();
    for(int i = 0; i < n; ++i) {
        on_segment |= is_point_on_lines(v[i], v[(i+1)%n], p);   // 辺上判定
        CP s = v[i]-p, t = v[(i+1)%n]-p;
        if(s.imag() > t.imag()) swap(s, t);     // 下側を基準にする
        if(s.imag()*t.imag() <= 0 && t.imag() > 0 && cross(s, t) > 0) {
            contain = !contain;     // 交差回数が奇数なら内側
        }
    }
    if(on_segment) return 1;    // 辺上
    if(contain) return 2;       // 内側
    return 0;       // 外側
}

// 最近点対距離
long double closest_pair(vector<CP> &v, int l = -1, 
                         int r = -1, bool reqsqrt = 0) {
    if(l == r && l == -1) {
        l = 0; r = v.size(); reqsqrt = 1;
        // x座標→y座標で昇順ソート
        auto lmd = [&](const CP &l, const CP &r) {
            if(l.real() != r.real()) return l.real() < r.real();
            return l.imag() < r.imag();
        };
        sort(v.begin(), v.end(), lmd);
    }
    if(r-l < 2) return 1e18;    // 2点存在しない
    if(r-l == 2) {      // ちょうど2点の時
        if(v[l].imag() > v[l+1].imag()) swap(v[l], v[l+1]);
        if(reqsqrt) return abs(v[l]-v[l+1]);
        return norm(v[l]-v[l+1]);
    }
    // 2点以上に関して分割統治法
    int mid = (l+r)/2;
    long double x = v[mid].real();  // 分断する線のx座標
    // 左半分，右半分について再帰，同一領域内の最小距離resを求める
    long double res = min(closest_pair(v, l, mid), closest_pair(v, mid, r));
    auto f = [](CP pl, CP pr) { return pl.imag() < pr.imag(); };
    inplace_merge(v.begin()+l, v.begin()+mid, v.begin()+r, f);
    vector<CP> tmp;
    // 異なる領域の2点について最小距離res未満で探索
    for(int i = l; i < r; ++i) {
        long double dx = abs(v[i].real()-x);
        int tsize = tmp.size();
        if(dx*dx >= res) continue;
        for(int j = 0; j < tsize; ++j) {
            CP delta = v[i]-tmp[tsize-1-j];
            if(delta.imag()*delta.imag() >= res) break;
            res = min(res, norm(delta));
        }
        tmp.emplace_back(v[i]);
    }
    if(reqsqrt) res = sqrtl(res);
    return res;
}

// 円の表現
struct Circle {
    CP o;
    long double r;
    Circle(long double _x = 0.0L, long double _y = 0.0L,
           long double _r = 0.0L)
        : o(CP(_x, _y)), r(_r) {}
    Circle(CP _o, long double _r = 0.0) : o(_o), r(_r) {}
}; typedef vector<CP> Polygon;

// 2円の位置関係
int is_cross_circles(Circle l, Circle r) {
    long double distlr = abs(l.o-r.o);
    if(l.r+r.r+EPS < distlr) return 4;         // 交点無し外側
    if(r.r+distlr+EPS < l.r) return -2;          // 交点無し内側(R in L)
    if(l.r+distlr+EPS < r.r) return 2;          // 交点無し内側(L in R)
    if(abs(l.r+r.r-distlr) < EPS) return 3;    // 外接
    if(abs(l.r+distlr-r.r) < EPS) return -1;     // 内接(R in L)
    if(abs(r.r+distlr-l.r) < EPS) return 1;     // 内接(L in R)
    return 0;                                   // 2点で交わる
}

// 円に対する点の包含
bool contain_point(Circle c, CP p) {
    long double dist = abs(c.o-p);
    return dist < c.r+EPS;
}

// 円と直線の交点
vector<CP> intersection_circle_line(Circle ci, CP s, CP t) {
    vector<CP> res(2);  // 1点の場合同じ座標
    res[0] = res[1] = projection(s, t, ci.o);   // 2等分点の座標
    long double d = ci.r*ci.r - norm(res[0]-ci.o);
    if(d <= EPS || t == s) return res;
    CP ust = (t-s)/abs(t-s);    // stの単位ベクトル
    res[0] += (ust*sqrtl(d));
    res[1] -= (ust*sqrtl(d));
    if(res[0].real() > res[1].real() ||
       (res[0].real() == res[1].real() && res[0].imag() > res[1].imag())) {
           swap(res[0], res[1]);
    }
    return res;
}

// 2円の交点(1つは交点が存在する)
vector<CP> intersection_circle_circle(Circle c1, Circle c2) {
    vector<CP> res(2);  // 1点の場合同じ座標
    long double d = abs(c1.o-c2.o);
    long double arg = acosl((c1.r*c1.r+d*d-c2.r*c2.r)/(2.0L*c1.r*d));
    CP c1p = (c2.o-c1.o)*CP(cosl(arg), sinl(arg));
    CP uc1p = c1p/abs(c1p);     // c1の中心から交点p方向への単位ベクトル
    res[0] = c1.o + c1.r*uc1p;
    res[1] = reflection(c1.o, c2.o, res[0]);    // p,qの位置関係は反射
    if(res[0].real() > res[1].real() ||
       (res[0].real() == res[1].real() && res[0].imag() > res[1].imag())) {
           swap(res[0], res[1]);
    }
    return res;
}

// 線分lrの垂直二等分線
Line vertical_bisector(CP l, CP r) {
    Circle c1 = Circle(l, abs(l-r)), c2 = Circle(r, abs(l-r));
    vector<CP> res = intersection_circle_circle(c1, c2);
    if(cross(r-l, res[0]-l) > 0) swap(res[0], res[1]);
    return Line(res[0], res[1]);
}

// 点pを通る円の接線の接点
vector<CP> contact_circle_point(Circle ci, CP p) {
    vector<CP> res(2);
    long double d = abs(ci.o-p);
    if(abs(d-ci.r) <= EPS) {
        res[0] = res[1] = p;
    }else if(d < ci.r) {
        res[0] = res[1] = CP(-1e18, -1e18);
    }else {
        long double arg = asinl(ci.r/d);
        CP p0 = (ci.o-p)*CP(cosl(arg), sinl(arg));
        CP up0 = p0/abs(p0);
        res[0] = p + d*cosl(arg)*up0;
        res[1] = reflection(p, ci.o, res[0]);
        if(res[0].real() > res[1].real() || 
           (res[0].real() == res[1].real() && res[0].imag() > res[1].imag())) {
               swap(res[0], res[1]);
        }
    }
    return res;
}

// 2円の共通接線
vector<Line> tangent_circle_circle(Circle cl, Circle cr) {
    vector<Line> res;
    if(cl.r < cr.r) swap(cl, cr);
    long double g = abs(cl.o-cr.o);
    if(abs(g-0.0L) <= EPS) return res;  // 内包
    CP hor = (cr.o-cl.o)/g;
    CP ver = hor*CP(cosl(PI*0.5L), sinl(PI*0.5L));
    for(int s : {-1, 1}) {
        long double h = (cl.r + (long double)s*cr.r)/g;
        if(abs(1-h*h) <= EPS) {
            // 2円が接しているときの共通接線
            res.emplace_back(cl.o + hor*cl.r, cl.o + (hor+ver)*cl.r);
        }else if(1-h*h > 0) {
            // 内側に引かれる2接線+外側に引かれる2接線
            CP nhor = hor*h, nver = ver*sqrtl(1-h*h);
            res.emplace_back(cl.o + (nhor+nver)*cl.r, 
                             cr.o - (nhor+nver)*(cr.r*(long double)s));
            res.emplace_back(cl.o + (nhor-nver)*cl.r, 
                             cr.o - (nhor-nver)*(cr.r*(long double)s));
        }
    }
    return res;
}

// 三角形の内接円
Circle inscribed_circle(CP A, CP B, CP C) {
    if(cross(B-A, C-A) < 0) swap(B, C);
    long double a = abs(B-C), b = abs(C-A), c = abs(A-B);
    long double alpha = acosl((b*b+c*c-a*a)/(2.0L*b*c));
    long double beta = acosl((c*c+a*a-b*b)/(2.0L*c*a));
    // AとxABを通る直線とBとxBCを通る直線の交点が内心
    CP I = intersection_line(A, A+translate(B-A, alpha/2.0L), B, B+translate(C-B, beta/2.0L));
    // ABとIの最短距離が内心半径
    long double Ir = distance_lines_p(A, B, I);
    return Circle(I, Ir);
}

// 三角形の外接円
Circle circumscribed_circle(CP A, CP B, CP C) {
    // 2つの垂直二等分線の交点
    Line AB = vertical_bisector(A, B);
    Line BC = vertical_bisector(B, C);
    CP O = intersection_line(AB.s, AB.t, BC.s, BC.t);
    long double Or = abs(A-O);
    return Circle(O, Or);
}

// 2円の共通面積
long double common_area(Circle cl, Circle cr) {
    if(cl.r < cr.r) swap(cl, cr);
    int num = is_cross_circles(cl, cr);
    if(num >= 3) return 0.0L;
    if(num >= 1) return PI*cr.r*cr.r;
    if(num < 0) return PI*cl.r*cl.r;
    long double d = abs(cl.o-cr.o);
    long double res = 0.0L;
    for(int i = 0; i < 2; ++i) {
        long double theta = 2.0L * acosl((d*d+cl.r*cl.r-cr.r*cr.r)/(2.0L*d*cl.r));
        res += (theta-sinl(theta))*cl.r*cl.r/2.0L;
        swap(cl, cr);
    }
    return res;
}

// 円と多角形の共通面積
long double common_area(Polygon p, Circle c) {
    if(p.size() < 3) return 0.0L;
    function<long double(Circle, CP, CP)> dfs = [&](Circle c, CP a, CP b) {
        CP va = c.o-a, vb = c.o-b;
        long double f = cross(va, vb), res = 0;
        if(EQ(f, 0.0)) return res;
        if(max(abs(va), abs(vb)) < c.r+EPS) return f;
        CP d(dot(va, vb), cross(va, vb));
        if(distance_lines_p(a, b, c.o) > c.r-EPS) {
            return c.r*c.r*(atan2(d.imag(), d.real()));
        }
        auto u = intersection_circle_line(c, a, b);
        if(u.empty()) return res;
        if(u.size() > 1 && dot(u[1]-u[0], a-u[0]) > 0) swap(u[0], u[1]);
        u.emplace(u.begin(), a);
        u.emplace_back(b);
        for(int i = 1; i < (int)u.size(); ++i) res += dfs(c, u[i-1], u[i]);
        return res;
    };
    long double res = 0.0L;
    for(int i = 0; i < (int)p.size(); ++i) res += dfs(c, p[i], p[(i+1)%p.size()]);
    return res/2;
}

// 3点の位置関係を判定
int calc_clockwise(CP p0, CP p1, CP p2) {
    CP x = p1-p0, y = p2-p0;
    if(cross(x, y) > EPS) return 1;     // "COUNTER_CLOCKWISE"
    if(cross(x, y) < -EPS) return -1;   // "CLOCKWISE"
    if(dot(x, y) < 0) return 2;         // "ONLINE_BACK"
    if(norm(x) < norm(y)) return -2;    // "ONLINE_FRONT"
    return 0;                           // "ON_SEGMENT" 
}
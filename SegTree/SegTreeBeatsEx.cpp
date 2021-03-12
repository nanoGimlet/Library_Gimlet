#include <bits/stdc++.h>
using namespace std;
template<class T> inline bool chmax(T& a, T b) { if (a < b) { a = b; return true; } return false; }
template<class T> inline bool chmin(T& a, T b) { if (a > b) { a = b; return true; } return false; }
using ll = long long;
using P = pair<ll, ll>;
const long double PI = acos(-1.0L);
ll GCD(ll a, ll b) { return b?GCD(b, a%b):a; }
ll LCM(ll a, ll b) { return a/GCD(a, b)*b; }

const ll INF = 1e18;
template<typename T>
struct Segment_Tree_Beats {
    int N;
    vector<T> max_v, smax_v, min_v, smin_v, sum;   // 1番目の最大値(最小値)と2番目の最大値(最小値)
    vector<int> maxc, minc, len;    // 最大値(最小値)の個数
    void update_max(int i, T x) {
        sum[i] += (x-max_v[i])*maxc[i];     // 差分を伝搬させる
        if(max_v[i] == min_v[i]) {
            min_v[i] = x;
        }else if(max_v[i] == smin_v[i]) {
            smin_v[i] = x;
        } 
        max_v[i] = x;
    }
    void update_min(int i, T x) {
        sum[i] += (x-min_v[i])*minc[i];
        if(min_v[i] == max_v[i]) {
            max_v[i] = x;
        }else if(min_v[i] == smax_v[i]) {
            smax_v[i] = x;
        }
        min_v[i] = x;
    }
    void push(int i) {
        if(i >= N-1) return;
        int l = i*2+1;
        int r = i*2+2;
        if(max_v[i] < max_v[l]) {
            update_max(l, max_v[i]);
        }
        if(max_v[i] < max_v[r]) {
            update_max(r, max_v[i]);
        }
        if(min_v[i] > min_v[l]) {
            update_min(l, min_v[i]);
        }
        if(min_v[i] > min_v[r]) {
            update_min(r, min_v[i]);
        }
    }
    void update(int i) {
        int l = i*2+1;
        int r = i*2+2;
        sum[i] = sum[l]+sum[r];
        if(max_v[l] > max_v[r]) {
            max_v[i] = max_v[l];
            smax_v[i] = max(smax_v[l], max_v[r]);
            maxc[i] = maxc[l];
        }else if(max_v[l] < max_v[r]) {
            max_v[i] = max_v[r];
            smax_v[i] = max(max_v[l], smax_v[r]);
            maxc[i] = maxc[r];
        }else {
            max_v[i] = max_v[l];
            smax_v[i] = max(smax_v[l], smax_v[r]);
            maxc[i] = maxc[l]+maxc[r];
        }
        if(min_v[l] < min_v[r]) {
            min_v[i] = min_v[l];
            smin_v[i] = min(smin_v[l], min_v[r]);
            minc[i] = minc[l];
        }else if(min_v[l] > min_v[r]) {
            min_v[i] = min_v[r];
            smin_v[i] = min(min_v[l], smin_v[r]);
            minc[i] = minc[r];
        }else {
            min_v[i] = min_v[l];
            smin_v[i] = min(smin_v[l], smin_v[r]);
            minc[i] = minc[l]+minc[r];
        }
    }

    Segment_Tree_Beats(vector<T> v) {
        int n = v.size(); N = 1;
        while(N < n) N *= 2;
        max_v = vector<T>(N*2-1, -INF);
        smax_v = vector<T>(N*2-1, -INF);
        min_v = vector<T>(N*2-1, INF);
        smin_v = vector<T>(N*2-1, INF);
        sum = vector<T>(N*2-1, 0);
        maxc = vector<int>(N*2-1, 1);
        minc = vector<int>(N*2-1, 1);
        len = vector<int>(N*2-1, 1);
        for(int i = 0; i < n; ++i) {
            max_v[N-1+i] = v[i];
            min_v[N-1+i] = v[i];
            sum[N-1+i] = v[i];
        }
        for(int i = N-2; i >= 0; --i) {
            len[i] = len[i*2+1]+len[i*2+2];
            update(i);
        }
    }
    void range_chmin(int a, int b, T x, int i, int l, int r) {
        if(r <= a || b <= l || x >= max_v[i]) return;
        else if(a <= l && r <= b && x > smax_v[i]) {
            update_max(i, x);
            return;
        }
        push(i);
        int mid = (l+r)/2;
        range_chmin(a, b, x, i*2+1, l, mid);
        range_chmin(a, b, x, i*2+2, mid, r);
        update(i);
    }
    void range_chmax(int a, int b, T x, int i, int l, int r) {
        if(r <= a || b <= l || x <= min_v[i]) return;
        else if(a <= l && r <= b && x < smin_v[i]) {
            update_min(i, x);
            return;
        }
        push(i);
        int mid = (l+r)/2;
        range_chmax(a, b, x, i*2+1, l, mid);
        range_chmax(a, b, x, i*2+2, mid, r);
        update(i);
    }
    T range_sum(int a, int b, int i, int l, int r) {
        if(r < a || b <= l) return 0;
        else if(a <= l && r <= b) {
            return sum[i];
        }
        push(i);
        int mid = (l+r)/2;
        return range_sum(a, b, i*2+1, l, mid)+range_sum(a, b, i*2+2, mid, r);
    }
    void range_chmin(int a, int b, T x) {
        range_chmin(a, b, x, 0, 0, N);
    }
    void range_chmax(int a, int b, T x) {
        range_chmax(a, b, x, 0, 0, N);
    }
    T range_sum(int a, int b) {
        return range_sum(a, b, 0, 0, N);
    }
};

int main() {
    int n; cin >> n;
    vector<ll> avec(n, 0);
    for(int i = 0; i < n; ++i) cin >> avec[i];

    Segment_Tree_Beats<ll> stb(vector<ll>(n, INF));
    ll ans = 0;
    for(int i = 0; i < n; ++i) {
        stb.range_chmin(0, i+1, avec[i]);
        ans += stb.range_sum(0, i+1);
    }
    cout << ans << endl;
}
#include <bits/stdc++.h>
using namespace std;
template<class T> inline bool chmax(T& a, T b) { if (a < b) { a = b; return true; } return false; }
template<class T> inline bool chmin(T& a, T b) { if (a > b) { a = b; return true; } return false; }
using ll = long long;
using P = pair<ll, ll>;
const long double PI = acos(-1.0L);
ll GCD(ll a, ll b) { return b?GCD(b, a%b):a; }
ll LCM(ll a, ll b) { return a/GCD(a, b)*b; }

struct Dice {
    // int x, y;
    long long l, r, f, b, d, u, x, y;
    Dice(long long _l = 4, long long _r = 3, long long _f = 2,
         long long _b = 5, long long _d = 6, long long _u = 1,
         long long _x = 0, long long _y = 0)
        : l(_l), r(_r), f(_f), b(_b), d(_d), u(_u), x(_x), y(_y) {}
    void RollS() {
        // ++y;
        long long buff = d;
        d = f;
        f = u; 
        u = b;
        b = buff;
    }
    void RollN() {
        // --y;
        long long buff = d;
        d = b;
        b = u;
        u = f;
        f = buff;
    }
    void RollL() {
        long long buff = f;
        f = l;
        l = b;
        b = r;
        r = buff;
    }
    void RollR() {
        long long buff = f;
        f = r;
        r = b;
        b = l;
        l = buff;
    }
    void RollW() {
        // --x;
        long long buff = d;
        d = l;
        l = u;
        u = r;
        r = buff;
    }
    void RollE() {
        // ++x;
        long long buff = d;
        d = r;
        r = u;
        u = l;
        l = buff;
    }

    vector<Dice> makeDice() {
        vector<Dice> ret;
        for(int i = 0; i < 6; ++i) {
            Dice d(*this);
            if(i == 1) d.RollN();
            if(i == 2) d.RollS();
            if(i == 3) d.RollS(), d.RollS();
            if(i == 4) d.RollL();
            if(i == 5) d.RollR();
            for(int j = 0; j < 4; ++j) {
                ret.emplace_back(d);
                d.RollE();
            }
        }
        return ret;
    }
};

int a, b, c, d, e, f;
string s;

int main() {
    cin >> a >> b >> c >> d >> e >> f;
    cin >> s;
    Dice dice = Dice(d, c, b, e, f, a);
    for(int i = 0; i < (int)s.length(); ++i) {
        if(s[i] == 'E') dice.RollE();
        else if(s[i] == 'N') dice.RollN();
        else if(s[i] == 'S') dice.RollS();
        else dice.RollW();
    }

    cout << dice.u << endl;
}
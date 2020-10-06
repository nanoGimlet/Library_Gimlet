#include <bits/stdc++.h>
using namespace std;

const long double PI = acos(-1.0L);
// cout << setpricision(14) << PI << endl;で出力

// 00000003を表示
// cout << setw(8) << setfill('0') << 3 << endl;

/*bool cmp(const abdata &aa, const abdata &bb) {
    return aa.a == bb.a ? aa.b < bb.b : aa.a < bb.a;
}*/

// popcount
// cout << __builtin_popcount(n) << endl;

// GCD
int gcd(int a, int b) {
    if(a < b) {
        int tmp = a;
        a = b;
        b = tmp;
    }
    int r = a % b;

    while(r > 0) {
        a = b;
        b = r;
        r = a % b;
    }

    return b;
}

// GCD ユークリッドの互除法バージョン
long long GCD(long long a, long long b) {
    if(b == 0) return a;
    else return GCD(b, a%b);
}

// 拡張GCD
long long extGCD(long long a, long long b, long long &x, long long &y) {
    if(b == 0) {
        x = 1; y = 0;
        return a;
    }
    long long d = extGCD(b, a%b, y, x);
    y -= a/b * x;
    return d;
}

//LCM（GCDとの併用前提）
int lcm(int a, int b) {
    int g = gcd(a, b);
    int l = a * (b / g);
    return l;
}

// 二項係数を用いる
const int MAX = 1000100;
const int MOD = 1000000007;

long long fac[MAX], finv[MAX], inv[MAX];

  // テーブルを作る前処理
  /* facが普通の階乗を求めている。invが逆数を求めている。finvが逆数の階乗を求めている。*/
void COMinit() {
    fac[0] = fac[1] = 1;
    finv[0] = finv[1] = 1;
    inv[1] = 1;
    for(int i = 2; i < MAX; i++) {
        fac[i] = fac[i-1] * i % MOD;
        inv[i] = MOD - inv[MOD%i] * (MOD/i) % MOD;
        finv[i] = finv[i-1] * inv[i] % MOD;
    }
}

  // 二項係数を計算する
long long COM(int n, int k) {
    if(n < k) return 0;
    if(n < 0 || k < 0) return 0;
    return fac[n] * (finv[k] * finv[n-k] % MOD) % MOD;
}

// nもkも小さいときの二項係数の求め方
const int MAX_C = 1000;
long long Com[MAX_C][MAX_C];

void calc_com() {
    memset(Com, 0, sizeof(Com));
    Com[0][0] = 1;
    for(int i = 1; i < MAX_C; ++i) {
        Com[i][0] = 1;
        for(int j = 1; j < MAX_C; ++j) {
            Com[i][j] = (Com[i-1][j-1] + Com[i-1][j]) % MOD;
        }
    }
}

// Eratosthenesの篩
vector<bool> primeno(1000100, true);
void Eratosthenes(int n) {
    primeno.at(0) = primeno.at(1) = false;
    int limit = sqrt(n)+1;
    for(int i = 2; i < limit; ++i) {
        if(primeno.at(i)) {
            for(int j = 0; i*(j+2) < n; ++j) {
                primeno.at(i*(j+2)) = false;
            }
        }
    }
}

// binary_search
// fは単調性を持ったbool型の判定でtrueであればmidにokがfalseであればmidにngが入る。
// -1~INFで使うとよい

vector<int> a;

bool isOK(int index, int key) {
    if(a.at(index) >= key) return true;
    else return false;
}

int binary_search(int key) {
    int ng = -1;
    int ok = (int)a.size();

    while(abs(ok-ng) > 1) {
        int mid = (ok+ng) / 2;
        if(isOK(mid, key)) ok = mid;
        else ng = mid;
    }
    return ok;
}

// next_permutationはソートしてから使う
void Permutation(vector<int> &ppl) {
    int pl = ppl.size();
    do {
        for(int i = 0; i < pl; ++i) {
            if(i) cout << ",";
            cout << ppl.at(i);
        }
        cout << endl;
    }while(next_permutation(ppl.begin(), ppl.end()));
}

int main() {
    cout << gcd(12, 9) << endl;
    cout << lcm(12, 9) << endl;
}
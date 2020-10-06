#include <bits/stdc++.h>
using namespace std;
template<class T> inline bool chmax(T& a, T b) { if (a < b) { a = b; return true; } return false; }
template<class T> inline bool chmin(T& a, T b) { if (a > b) { a = b; return true; } return false; }
using ll = long long;
using P = pair<ll, ll>;
ll GCD(ll a, ll b) { return b?GCD(b, a%b):a; }
ll LCM(ll a, ll b) { return a/GCD(a, b)*b; }

typedef string::const_iterator State;
class ParseError {};    // 例外処理

// Stateが参照になっているのでパースしてる前後にアクセスしやすい

// 四則演算の式をパースして、その評価結果を返す
int expression(State &begin) {
    int ret = term(begin);
    for(;;) {
        if(*begin == '+') {
            begin++;
            ret += term(begin);
        }else if(*begin == '-') {
            begin++;
            ret -= term(begin);
        }else break;
    }
    return ret;
}

// 乗算除算の式をパースして、その評価結果を返す
int term(State &begin) {
    int ret = number(begin);
    for(;;) {
        // 掛け算なら前の値に掛け算
        // 割り算なら前の値に割り算
        // それ以外が来たらbreak
        if(*begin == '*') {
            begin++;
            ret *= number(begin);
        }else if(*begin == '/') {
            begin++;
            ret /= number(begin);
        }else break;
    }
    return ret;
}

// 数字の列をパースして、その数を返す
// beginは文字列全体が入ってる
int number(State &begin) {
    int ret = 0;
    // isdigitは文字が10進数かどうかを返す
    while(isdigit(*begin)) {
        ret *= 10;
        ret += *begin - '0';
        // begin++をすることで次の文字にポインタを移動
        begin++;
    }
    return ret;
}

int main() {

}
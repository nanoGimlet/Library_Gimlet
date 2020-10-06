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
int number(State &begin);
int term(State &begin);
int expression(State &begin);
int factor(State &begin);
void consume(State &begin, char expected);

// Stateが参照になっているのでパースしてる前後にアクセスしやすい

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

// 乗算除算の式をパースして、その評価結果を返す
int term(State &begin) {
    int ret = factor(begin);
    for(;;) {
        // 掛け算なら前の値に掛け算
        // 割り算なら前の値に割り算
        // それ以外が来たらbreak
        if(*begin == '*') {
            begin++;
            ret *= factor(begin);
        }else if(*begin == '/') {
            begin++;
            ret /= factor(begin);
        }else break;
    }
    return ret;
}

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

// 括弧か数をパースして、その評価結果を返す
int factor(State &begin) {
    if(*begin == '(') {
        consume(begin, '(');    // '('を飛ばす
        int ret = expression(begin);
        consume(begin, ')');    // ')'を飛ばす
        return ret;
    }else return number(begin);
}

// beginがexpectedを指していたらbeginを1つ進める
void consume(State &begin, char expected) {
    if(*begin == expected) begin++;
    else {
        cerr << "Expected '" << expected << "' but got '" << *begin << "'" << endl;
        cerr << "Rest string is '";
        while(*begin) {
            cerr << *begin++;
        }
        cerr << "'" << endl;
        throw ParseError();
    }
}

int main() {
    int n; cin >> n;
    cin.ignore();   // nで読み込んだ改行や空白をgetlineに影響しないようにするため
    for(int i = 0; i < n; ++i) {
        string s;
        getline(cin, s);    // cinだと空白区切りしてしまうから駄目

        State begin = s.begin();
        int ans = expression(begin);
        consume(begin, '=');
        cout << ans << endl;
    }
}
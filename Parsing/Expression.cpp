typedef string::const_iterator State;
class ParseError {};

struct Parser {
    // エラー検出
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

    // 数字の列をパースして、その数を返す
    ll number(State &begin) {
        ll ret = 0;
        while(isdigit(*begin)) {
            ret *= 10;
            ret += (*begin - '0');
            begin++;
        }
        return ret;
    }

    // 括弧か数をパースして、その評価結果を返す
    ll factor(State &begin) {
        if(*begin == '(') {
            consume(begin, '(');    // '('を飛ばす
            ll ret = expression(begin);
            consume(begin, ')');    // ')'を飛ばす
            return ret;
        }else return number(begin);
    }

    // 乗算除算の式をパースして、その評価結果を返す
    ll term(State &begin) {
        ll ret = factor(begin);
        while(1) {
            if(*begin == '*') {
                consume(begin, '*');
                ret *= factor(begin);
            }else if(*begin == '/') {
                consume(begin, '/');
                ret /= factor(begin);
            }else break;
        }
        return ret;
    }

    // 四則演算の式をパースして、その評価結果を返す
    ll expression(State &begin) {
        ll ret = term(begin);
        while(1) {
            if(*begin == '+') {
                consume(begin, '+');
                ret += term(begin);
            }else if(*begin == '-') {
                consume(begin, '-');
                ret -= term(begin);
            }else break;
        }
        return ret;
    }
};
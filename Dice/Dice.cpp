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
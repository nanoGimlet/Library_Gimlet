vector<int> z_algorithm(const string &s) {
    int slen = s.length();
    vector<int> prefix(slen, 0);
    prefix[0] = slen;
    int i = 1, j = 0;
    while(i < slen) {
        while(i+j < slen && s[j] == s[i+j]) ++j;
        prefix[i] = j;
        if(j == 0) {
            ++i;
            continue;
        }
        int k = 1;
        while(k < j && k+prefix[k] < j) {
            prefix[i+k] = prefix[k];
            ++k;
        }
        i += k;
        j -= k;
    }
    return prefix;
}

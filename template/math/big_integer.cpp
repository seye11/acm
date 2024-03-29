struct bign {
    int len, s[numlen];
    bign() {
        memset(s, 0, sizeof(s));
        len = 1;
    }
    bign(int num) { *this = num; }
    bign(const char *num) { *this = num; }
    bign operator = (const int num) {
        char s[numlen];
        sprintf(s, "%d", num);
        *this = s;
        return *this;
    }
    bign operator = (const char *num) {
        len = strlen(num);
        while (len > 1 && num[0] == '0') num++, len--;
        for (int i = 0; i < len; i++) s[i] = num[len - i - 1] - '0';
        return *this;
    }

    void deal() {
        while (len > 1 && !s[len - 1]) len--;
    }

    bign operator + (const bign &a) const {
        bign ret;
        ret.len = 0;
        for (int i = 0; add || i < top; i++) {
            int now = add;
            if (i < len) now += s[i];
            if (i < a.len)   now += a.s[i];
            ret.s[ret.len++] = now % 10;
            add = now / 10;
        }
        return ret;
    }
    bign operator - (const bign &a) const {
        bign ret;
        ret.len = 0;
        int cal = 0;
        for (int i = 0; i < len; i++) {
            int now = s[i] - cal;
            if (i < a.len)   now -= a.s[i];
            if (now >= 0)    cal = 0;
            else {
                cal = 1; now += 10;
            }
            ret.s[ret.len++] = now;
        }
        ret.deal();
        return ret;
    }
    bign operator * (const bign &a) const {
        bign ret;
        ret.len = len + a.len;
        for (int i = 0; i < len; i++) {
            for (int j = 0; j < a.len; j++)
                ret.s[i + j] += s[i] * a.s[j];
        }
        for (int i = 0; i < ret.len; i++) {
            ret.s[i + 1] += ret.s[i] / 10;
            ret.s[i] %= 10;
        }
        ret.deal();
        return ret;
    }

    //乘以小数，直接乘快点
    bign operator * (const int num) {
        bign ret;
        ret.len = 0;
        int bb = 0;
        for (int i = 0; i < len; i++) {
            int now = bb + s[i] * num;
            ret.s[ret.len++] = now % 10;
            bb = now / 10;
        }
        while (bb) {
            ret.s[ret.len++] = bb % 10;
            bb /= 10;
        }
        ret.deal();
        return ret;
    }

    bign operator / (const bign &a) const {
        bign ret, cur = 0;
        ret.len = len;
        for (int i = len - 1; i >= 0; i--) {
            cur = cur * 10;
            cur.s[0] = s[i];
            while (cur >= a) {
                cur -= a;
                ret.s[i]++;
            }
        }
        ret.deal();
        return ret;
    }

    bign operator % (const bign &a) const {
        bign b = *this / a;
        return *this - b * a;
    }

    bign operator += (const bign &a) { *this = *this + a; return *this; }
    bign operator -= (const bign &a) { *this = *this - a; return *this; }
    bign operator *= (const bign &a) { *this = *this * a; return *this; }
    bign operator /= (const bign &a) { *this = *this / a; return *this; }
    bign operator %= (const bign &a) { *this = *this % a; return *this; }

    bool operator < (const bign &a) const {
        if (len != a.len)    return len < a.len;
        for (int i = len - 1; i >= 0; i--) if (s[i] != a.s[i])
                return s[i] < a.s[i];
        return false;
    }
    bool operator > (const bign &a) const  { return a < *this; }
    bool operator <= (const bign &a) const { return !(*this > a); }
    bool operator >= (const bign &a) const { return !(*this < a); }
    bool operator == (const bign &a) const { return !(*this > a || *this < a); }
    bool operator != (const bign &a) const { return *this > a || *this < a; }

    string str() const {
        string ret = "";
        for (int i = 0; i < len; i++) ret = char(s[i] + '0') + ret;
        return ret;
    }
};
istream& operator >> (istream &in, bign &x) {
    string s;
    in >> s;
    x = s.c_str();
    return in;
}
ostream& operator << (ostream &out, const bign &x) {
    out << x.str();
    return out;
}
// 大数开平方
bign Sqrt(bign x) {
    int a[numlen / 2];
    int top = 0;
    for (int i = 0; i < x.len; i += 2) {
        if (i == x.len - 1) {
            a[top++] = x.s[i];
        } else
            a[top++] = x.s[i] + x.s[i + 1] * 10;
    }
    bign ret = (int)sqrt((double)a[top - 1]);
    int xx = (int)sqrt((double)a[top - 1]);
    bign pre = a[top - 1] - xx * xx;
    bign cc;
    for (int i = top - 2; i >= 0; i--) {
        pre = pre * 100 + a[i];
        cc = ret * 20;
        for (int j = 9; j >= 0; j--) {
            bign now = (cc + j) * j;
            if (now <= pre) {
                ret = ret * 10 + j;
                pre -= now;
                break;
            }
        }
    }
    return ret;
}
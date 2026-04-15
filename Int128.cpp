#include "Int128.hpp"
#include <algorithm>

__int128 Int128::to_native() const {
    return ( (__int128)high << 64 ) | low;
}

Int128 Int128::from_native(__int128 v) {
    Int128 res;
    res.high = (int64_t)(v >> 64);
    res.low = (uint64_t)v;
    return res;
}

Int128::Int128() : high(0), low(0) {}

Int128::Int128(int64_t v) {
    __int128 tmp = v;
    high = (int64_t)(tmp >> 64);
    low = (uint64_t)tmp;
}

Int128::Int128(int64_t h, uint64_t l) : high(h), low(l) {}

Int128::Int128(std::string_view s) {
    __int128 val = 0;
    bool neg = false;

    if (!s.empty() && s[0] == '-') {
        neg = true;
        s.remove_prefix(1);
    }

    for (char c : s) {
        val = val * 10 + (c - '0');
    }

    if (neg) val = -val;

    high = (int64_t)(val >> 64);
    low = (uint64_t)val;
}

Int128::operator int64_t() const {
    return (int64_t)to_native();
}

Int128::operator double() const {
    return (double)to_native();
}

Int128& Int128::operator+=(const Int128& rhs) {
    *this = from_native(to_native() + rhs.to_native());
    return *this;
}

Int128& Int128::operator-=(const Int128& rhs) {
    *this = from_native(to_native() - rhs.to_native());
    return *this;
}

Int128& Int128::operator*=(const Int128& rhs) {
    *this = from_native(to_native() * rhs.to_native());
    return *this;
}

Int128& Int128::operator/=(const Int128& rhs) {
    *this = from_native(to_native() / rhs.to_native());
    return *this;
}

Int128 Int128::operator-() const {
    return from_native(-to_native());
}

bool Int128::operator==(const Int128& rhs) const {
    return high == rhs.high && low == rhs.low;
}

bool Int128::operator!=(const Int128& rhs) const {
    return !(*this == rhs);
}

std::string Int128::str() const {
    __int128 val = to_native();

    if (val == 0) return "0";

    if (val == ((__int128)1 << 127)) {
        return "-170141183460469231731687303715884105728";
    }
    
    bool neg = val < 0;
    if (neg) val = -val;

    std::string s;
    while (val > 0) {
        s.push_back('0' + val % 10);
        val /= 10;
    }

    if (neg) s.push_back('-');
    std::reverse(s.begin(), s.end());
    return s;
}

std::ostream& operator<<(std::ostream& os, const Int128& v) {
    return os << v.str();
}

Int128 operator+(Int128 a, const Int128& b) { return a += b; }
Int128 operator-(Int128 a, const Int128& b) { return a -= b; }
Int128 operator*(Int128 a, const Int128& b) { return a *= b; }
Int128 operator/(Int128 a, const Int128& b) { return a /= b; }

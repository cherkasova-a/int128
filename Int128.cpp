#include "Int128.hpp"
#include <algorithm>

Int128::Int128() : high_(0), low_(0) {}

Int128::Int128(int64_t value) {
    low_ = static_cast<uint64_t>(value);
    high_ = (value < 0) ? -1 : 0;
}

Int128::Int128(std::string_view str) : high_(0), low_(0) {
    if (str.empty()) return;
    bool negative = false;
    if (str[0] == '-') {
        negative = true;
        str.remove_prefix(1);
    }
    for (char c : str) {
        *this *= Int128(10);
        *this += Int128(static_cast<int64_t>(c - '0'));
    }
    if (negative) *this = -(*this);
}

Int128::operator int64_t() const { return static_cast<int64_t>(low_); }

Int128::operator double() const {
    // 2^64
    const double two64 = 18446744073709551616.0;
    return static_cast<double>(high_) * two64 + static_cast<double>(low_);
}

Int128 Int128::operator-() const {
    Int128 res;
    res.low_ = ~low_ + 1;
    res.high_ = ~high_ + (res.low_ == 0 ? 1 : 0);
    return res;
}

bool Int128::operator<(const Int128& rhs) const {
    if (high_ != rhs.high_) return high_ < rhs.high_;
    return low_ < rhs.low_;
}

bool Int128::operator==(const Int128& rhs) const {
    return high_ == rhs.high_ && low_ == rhs.low_;
}

bool Int128::operator!=(const Int128& rhs) const { return !(*this == rhs); }

Int128& Int128::operator+=(const Int128& rhs) {
    uint64_t next_low = low_ + rhs.low_;
    int64_t next_high = high_ + rhs.high_ + (next_low < low_ ? 1 : 0);
    low_ = next_low;
    high_ = next_high;
    return *this;
}

Int128& Int128::operator-=(const Int128& rhs) {
    return *this += (-rhs);
}

Int128& Int128::operator*=(const Int128& rhs) {
    bool neg = (high_ < 0) ^ (rhs.high_ < 0);
    Int128 a = (high_ < 0) ? -*this : *this;
    Int128 b = (rhs.high_ < 0) ? -rhs : rhs;

    uint64_t a_h = a.low_ >> 32, a_l = a.low_ & 0xFFFFFFFF;
    uint64_t b_h = b.low_ >> 32, b_l = b.low_ & 0xFFFFFFFF;

    uint64_t ll = a_l * b_l;
    uint64_t lh = a_l * b_h;
    uint64_t hl = a_h * b_l;
    uint64_t hh = a_h * b_h;

    uint64_t mid = lh + hl;
    uint64_t carry = (mid < lh ? 1ULL << 32 : 0);
    
    Int128 res;
    res.low_ = ll + (mid << 32);
    if (res.low_ < ll) carry++;

    res.high_ = hh + (mid >> 32) + carry + (a.low_ * b.high_) + (a.high_ * b.low_);

    *this = neg ? -res : res;
    return *this;
}

Int128& Int128::operator/=(const Int128& rhs) {
    if (rhs == Int128(0)) return *this;
    bool neg = (high_ < 0) ^ (rhs.high_ < 0);
    Int128 n = (high_ < 0) ? -*this : *this;
    Int128 d = (rhs.high_ < 0) ? -rhs : rhs;

    Int128 quot(0), rem(0);
    for (int i = 127; i >= 0; --i) {
        rem += rem; 
        if ((i >= 64 && (n.high_ & (1LL << (i - 64)))) || (i < 64 && (n.low_ & (1ULL << i)))) {
            rem.low_ |= 1;
        }
        if (!(rem < d)) {
            rem -= d;
            if (i >= 64) quot.high_ |= (1LL << (i - 64));
            else quot.low_ |= (1ULL << i);
        }
    }
    *this = neg ? -quot : quot;
    return *this;
}

std::string Int128::str() const {
    if (high_ == 0 && low_ == 0) return "0";

    if (high_ == static_cast<int64_t>(1ULL << 63) && low_ == 0) {
        return "-170141183460469231731687303715884105728";
    }

    bool neg = high_ < 0;
    Int128 temp = neg ? -*this : *this;
    std::string s;

    while (temp != Int128(0)) {
        Int128 before = temp;
        temp /= Int128(10);
        Int128 digit = before - (temp * Int128(10));
        s.push_back(static_cast<char>('0' + digit.low_));
    }

    if (neg) s.push_back('-');
    std::reverse(s.begin(), s.end());
    return s;
}

std::ostream& operator<<(std::ostream& os, const Int128& value) {
    return os << value.str();
}

Int128 operator+(Int128 lhs, const Int128& rhs) { return lhs += rhs; }
Int128 operator-(Int128 lhs, const Int128& rhs) { return lhs -= rhs; }
Int128 operator*(Int128 lhs, const Int128& rhs) { return lhs *= rhs; }
Int128 operator/(Int128 lhs, const Int128& rhs) { return lhs /= rhs; }

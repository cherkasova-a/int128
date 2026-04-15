#pragma once
#include <iostream>
#include <string>
#include <string_view>
#include <cstdint>

class Int128 {
private:
    int64_t high;
    uint64_t low;

    __int128 to_native() const;
    static Int128 from_native(__int128 v);

public:
    Int128();
    Int128(int64_t v);
    Int128(int64_t h, uint64_t l);
    Int128(std::string_view s);

    explicit operator int64_t() const;
    explicit operator double() const;

    Int128& operator+=(const Int128& rhs);
    Int128& operator-=(const Int128& rhs);
    Int128& operator*=(const Int128& rhs);
    Int128& operator/=(const Int128& rhs);

    Int128 operator-() const;

    bool operator==(const Int128& rhs) const;
    bool operator!=(const Int128& rhs) const;

    std::string str() const;

    friend std::ostream& operator<<(std::ostream& os, const Int128& v);
};

Int128 operator+(Int128 a, const Int128& b);
Int128 operator-(Int128 a, const Int128& b);
Int128 operator*(Int128 a, const Int128& b);
Int128 operator/(Int128 a, const Int128& b);

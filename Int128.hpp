#pragma once
#include <cstdint>
#include <string>
#include <string_view>
#include <iostream>

class Int128 {
private:
    int64_t high_;
    uint64_t low_;

public:
    Int128();
    Int128(int64_t value);
    Int128(std::string_view str);

    explicit operator int64_t() const;
    explicit operator double() const;

    std::string str() const;

    Int128 operator-() const;

    Int128& operator+=(const Int128& rhs);
    Int128& operator-=(const Int128& rhs);
    Int128& operator*=(const Int128& rhs);
    Int128& operator/=(const Int128& rhs);

    bool operator==(const Int128& rhs) const;
    bool operator!=(const Int128& rhs) const;
    bool operator<(const Int128& rhs) const;

    friend std::ostream& operator<<(std::ostream& os, const Int128& value);
};

Int128 operator+(Int128 lhs, const Int128& rhs);
Int128 operator-(Int128 lhs, const Int128& rhs);
Int128 operator*(Int128 lhs, const Int128& rhs);
Int128 operator/(Int128 lhs, const Int128& rhs);

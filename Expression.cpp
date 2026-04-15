#include "Expression.hpp"

std::ostream& operator<<(std::ostream& os, const Expression& e) {
    e.print(os);
    return os;
}

Const::Const(Int128 v) : val(v) {}

Int128 Const::eval(const std::map<std::string, Int128>&) const {
    return val;
}

Expression* Const::clone() const {
    return new Const(val);
}

void Const::print(std::ostream& os) const {
    os << val;
}

Variable::Variable(std::string_view n) : name(n) {}

Int128 Variable::eval(const std::map<std::string, Int128>& vars) const {
    return vars.at(name);
}

Expression* Variable::clone() const {
    return new Variable(name);
}

void Variable::print(std::ostream& os) const {
    os << name;
}

BinaryOp::BinaryOp(const Expression& l, const Expression& r)
    : left(l.clone()), right(r.clone()) {}

Add::Add(const Expression& l, const Expression& r) : BinaryOp(l, r) {}

Int128 Add::eval(const std::map<std::string, Int128>& v) const {
    return left->eval(v) + right->eval(v);
}

Expression* Add::clone() const {
    return new Add(*left, *right);
}

void Add::print(std::ostream& os) const {
    os << "(" << *left << " + " << *right << ")";
}

Subtract::Subtract(const Expression& l, const Expression& r) : BinaryOp(l, r) {}

Int128 Subtract::eval(const std::map<std::string, Int128>& v) const {
    return left->eval(v) - right->eval(v);
}

Expression* Subtract::clone() const {
    return new Subtract(*left, *right);
}

void Subtract::print(std::ostream& os) const {
    os << "(" << *left << " - " << *right << ")";
}

Multiply::Multiply(const Expression& l, const Expression& r) : BinaryOp(l, r) {}

Int128 Multiply::eval(const std::map<std::string, Int128>& v) const {
    return left->eval(v) * right->eval(v);
}

Expression* Multiply::clone() const {
    return new Multiply(*left, *right);
}

void Multiply::print(std::ostream& os) const {
    os << "(" << *left << " * " << *right << ")";
}

Divide::Divide(const Expression& l, const Expression& r) : BinaryOp(l, r) {}

Int128 Divide::eval(const std::map<std::string, Int128>& v) const {
    return left->eval(v) / right->eval(v);
}

Expression* Divide::clone() const {
    return new Divide(*left, *right);
}

void Divide::print(std::ostream& os) const {
    os << "(" << *left << " / " << *right << ")";
}

Negate::Negate(const Expression& e) : expr(e.clone()) {}

Int128 Negate::eval(const std::map<std::string, Int128>& v) const {
    return -expr->eval(v);
}

Expression* Negate::clone() const {
    return new Negate(*expr);
}

void Negate::print(std::ostream& os) const {
    os << "(-" << *expr << ")";
}

Add operator+(const Expression& l, const Expression& r) {
    return Add(l, r);
}

Subtract operator-(const Expression& l, const Expression& r) {
    return Subtract(l, r);
}

Multiply operator*(const Expression& l, const Expression& r) {
    return Multiply(l, r);
}

Divide operator/(const Expression& l, const Expression& r) {
    return Divide(l, r);
}

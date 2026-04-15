#pragma once
#include "Int128.hpp"
#include <map>
#include <memory>
#include <string>
#include <iostream>

class Expression {
public:
    virtual ~Expression() = default;
    virtual Int128 eval(const std::map<std::string, Int128>& vars) const = 0;
    virtual Expression* clone() const = 0;
    virtual void print(std::ostream& os) const = 0;

    friend std::ostream& operator<<(std::ostream& os, const Expression& e);
};

class Const : public Expression {
    Int128 val;
public:
    Const(Int128 v);
    Int128 eval(const std::map<std::string, Int128>&) const override;
    Expression* clone() const override;
    void print(std::ostream& os) const override;
};

class Variable : public Expression {
    std::string name;
public:
    Variable(std::string_view n);
    Int128 eval(const std::map<std::string, Int128>& vars) const override;
    Expression* clone() const override;
    void print(std::ostream& os) const override;
};

class BinaryOp : public Expression {
protected:
    std::unique_ptr<Expression> left, right;
public:
    BinaryOp(const Expression& l, const Expression& r);
};

class Add : public BinaryOp {
public:
    Add(const Expression& l, const Expression& r);
    Int128 eval(const std::map<std::string, Int128>&) const override;
    Expression* clone() const override;
    void print(std::ostream& os) const override;
};

class Subtract : public BinaryOp {
public:
    Subtract(const Expression& l, const Expression& r);
    Int128 eval(const std::map<std::string, Int128>&) const override;
    Expression* clone() const override;
    void print(std::ostream& os) const override;
};

class Multiply : public BinaryOp {
public:
    Multiply(const Expression& l, const Expression& r);
    Int128 eval(const std::map<std::string, Int128>&) const override;
    Expression* clone() const override;
    void print(std::ostream& os) const override;
};

class Divide : public BinaryOp {
public:
    Divide(const Expression& l, const Expression& r);
    Int128 eval(const std::map<std::string, Int128>&) const override;
    Expression* clone() const override;
    void print(std::ostream& os) const override;
};

class Negate : public Expression {
    std::unique_ptr<Expression> expr;
public:
    Negate(const Expression& e);
    Int128 eval(const std::map<std::string, Int128>&) const override;
    Expression* clone() const override;
    void print(std::ostream& os) const override;
};

Add operator+(const Expression& l, const Expression& r);
Subtract operator-(const Expression& l, const Expression& r);
Multiply operator*(const Expression& l, const Expression& r);
Divide operator/(const Expression& l, const Expression& r);

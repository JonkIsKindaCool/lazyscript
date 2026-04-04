#pragma once

#include <memory>
#include <iostream>
#include "lazyscript/lexer.hpp"

struct Declaration
{
    virtual ~Declaration() = default;
};
using DeclarationPtr = std::unique_ptr<Declaration>;

struct Statement
{
    virtual ~Statement() = default;
};

using StatementPtr = std::unique_ptr<Statement>;

struct Expression
{
    Span span;
    virtual ~Expression() = default;
};

using ExpressionPtr = std::unique_ptr<Expression>;

struct VariableType
{
    std::string name;
    std::vector<std::unique_ptr<VariableType>> generics;

    VariableType(std::string name, std::vector<std::unique_ptr<VariableType>> generics) : name(std::move(name)), generics(std::move(generics)) {}
};

using VariableTypePtr = std::unique_ptr<VariableType>;

struct FunctionArgument
{
    std::string name;
    VariableTypePtr type;
    bool optional;

    FunctionArgument(std::string name, VariableTypePtr type, bool optional) : name(std::move(name)), type(std::move(type)), optional(optional) {}
};

using FunctionArgumentPtr = std::unique_ptr<FunctionArgument>;

/*
    Declarations
 */

struct FunctionDeclaration : Declaration
{
    std::string name;
    std::vector<FunctionArgumentPtr> args;
    std::vector<StatementPtr> body;

    FunctionDeclaration(std::string n, std::vector<FunctionArgumentPtr> args, std::vector<StatementPtr> body) : name(std::move(n)), args(std::move(args)), body(std::move(body)) {}
};

/*
    Statements
*/

struct VarStmt : Statement
{
    std::string name;
    ExpressionPtr expr;

    VarStmt(std::string s, ExpressionPtr val) : name(std::move(s)), expr(std::move(val)) {}
};

struct ExpressionStmt : Statement
{
    ExpressionPtr expr;

    ExpressionStmt(ExpressionPtr expr) : expr(std::move(expr)) {}
};


/*
    Operators
*/
enum class BinaryOpType
{
    Add,      // +
    Subtract, // -
    Multiply, // *
    Divide,   // /
    Modulo,   // %

    Equal,        // ==
    NotEqual,     // !=
    Greater,      // >
    Less,         //
    GreaterEqual, // >=
    LessEqual,    // <=

    And, // &&
    Or,  // ||

    BitwiseAnd, // &
    BitwiseOr,  // |
    BitwiseXor, // ^
    LeftShift,  // <<
    RightShift, // >>

    AddAssign, // +=
    SubAssign, // -=
    MulAssign, // *=
    DivAssign, // /=
};

enum class UnaryOpType
{
    Negate, // -

    Not, // !

    BitwiseNot, // ~

    Reference,   // &
    Dereference, // *
};

enum class AssignmentOpType
{
    Assign,    // =
    AddAssign, // +=
    SubAssign, // -=
    MulAssign, // *=
    DivAssign, // /=
};

/*
    Expressions
*/
struct IntExpression : Expression
{
    int num;

    IntExpression(int n) : num(std::move(n)) {}
};

struct FloatExpression : Expression
{
    float num;

    FloatExpression(float n) : num(std::move(n)) {}
};

struct StringExpression : Expression
{
    std::string val;

    StringExpression(std::string s) : val(std::move(s)) {}
};

struct IdentifierLiteral : Expression
{
    std::string name;

    IdentifierLiteral(std::string s) : name(std::move(s)) {}
};

struct BinaryOp : Expression
{
    BinaryOpType op;
    ExpressionPtr left;
    ExpressionPtr right;

    BinaryOp(BinaryOpType o, ExpressionPtr l, ExpressionPtr r) : op(o), left(std::move(l)), right(std::move(r)) {}
};

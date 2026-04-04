#pragma once

#include <memory>
#include <iostream>

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
    virtual ~Expression() = default;
};

using ExpressionPtr = std::unique_ptr<Expression>;

/*
    Declarations
 */
struct FunctionDeclaration : Declaration
{
    std::string name;

    FunctionDeclaration(std::string n) : name(std::move(n)) {}
};

/*
    Statements
*/

struct VarStatement : Statement
{
    std::string name;
    ExpressionPtr expr;

    VarStatement(std::string s, ExpressionPtr val) : name(std::move(s)), expr(std::move(val)) {}
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

    IntExpression(int n) : num(n) {}
};

struct FloatExpression : Expression
{
    float num;

    FloatExpression(float n) : num(n) {}
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

    BinaryOp(BinaryOpType o, ExpressionPtr l, ExpressionPtr r) : op(o), left(std::move(l)), right(std::move(r))  {}
};

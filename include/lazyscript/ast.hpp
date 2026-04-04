#pragma once

#include <memory>
#include <iostream>
#include "lazyscript/lexer.hpp"

enum class DeclarationKind
{
    FUNCTION,
    CLASS,
    STRUCT,
    ENUM,
    TYPE,
};

struct Declaration
{
    virtual ~Declaration() = default;
};
using DeclarationPtr = std::unique_ptr<Declaration>;

enum class StatementKind
{
    IF,
    FUNCTION,
    WHILE,
    FOR,
    EXPRESSION
};

struct Statement
{
    virtual ~Statement() = default;
};

using StatementPtr = std::unique_ptr<Statement>;

enum class ExpressionKind
{
    INTEGER,
    FLOAT,
    STRING,
    IDENT,
    BINOP,
    UNOP,
    CALL,
    FIELD
};

struct Expression
{
    Span span;
    ExpressionKind kind;
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

    IntExpression(Span span, int n) : num(std::move(n))
    {
        span = std::move(span);
        kind = ExpressionKind::INTEGER;
    }
};

struct FloatExpression : Expression
{
    float num;

    FloatExpression(Span span, float n) : num(std::move(n))
    {
        span = std::move(span);
        kind = ExpressionKind::FLOAT;
    }
};

struct StringExpression : Expression
{
    std::string val;

    StringExpression(Span span, std::string s) : val(std::move(s))
    {
        span = std::move(span);
        kind = ExpressionKind::STRING;
    }
};

struct IdentifierLiteral : Expression
{
    std::string name;

    IdentifierLiteral(Span span, std::string s) : name(std::move(s))
    {
        span = std::move(span);
        kind = ExpressionKind::IDENT;
    }
};

struct BinaryOp : Expression
{
    BinaryOpType op;
    ExpressionPtr left;
    ExpressionPtr right;

    BinaryOp(Span span, BinaryOpType o, ExpressionPtr l, ExpressionPtr r) : op(o), left(std::move(l)), right(std::move(r))
    {
        span = std::move(span);
        kind = ExpressionKind::BINOP;
    }
};

struct AssignmentOp : Expression
{
    AssignmentOpType op;
    ExpressionPtr left;
    ExpressionPtr right;

    AssignmentOp(Span span, AssignmentOpType o, ExpressionPtr l, ExpressionPtr r) : op(o), left(std::move(l)), right(std::move(r))
    {
        span = std::move(span);
        kind = ExpressionKind::BINOP;
    }
};

struct FieldExpr : Expression
{
    ExpressionPtr parent;
    std::string field;

    FieldExpr(Span span, ExpressionPtr parent, std::string field) : parent(std::move(parent)), field(std::move(field))
    {
        span = std::move(span);
        kind = ExpressionKind::FIELD;
    }
};
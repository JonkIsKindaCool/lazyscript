#pragma once

#include <memory>
#include <vector>
#include <iostream>
#include "lazyscript/lexer.hpp"

enum class DeclarationKind
{
    FUNCTION,
    CLASS,
    STRUCT,
    ENUM,
    TYPE,
    VAR,
    CONST
};

struct Declaration
{
    DeclarationKind kind;
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
    ASSIGNMENT,
    BINOP,
    UNOP,
    CALL,
    FIELD,
    ACCESS
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
    VariableTypePtr type;

    FunctionDeclaration(std::string n, std::vector<FunctionArgumentPtr> args, std::vector<StatementPtr> body, VariableTypePtr type) : name(std::move(n)), args(std::move(args)), body(std::move(body)), type(std::move(type))
    {
        this->kind = DeclarationKind::FUNCTION;
    }
};

struct VarDecl : Declaration
{
    std::string name;
    VariableTypePtr type;
    ExpressionPtr expr;

    bool constant;

    VarDecl(std::string s, VariableTypePtr type, ExpressionPtr val, bool constant) : name(std::move(s)), expr(std::move(val)), type(std::move(type)), constant(constant)
    {
        if (constant)
        {
            this->kind = DeclarationKind::CONST;
        }
        else
        {
            this->kind = DeclarationKind::VAR;
        }
    }
};

/*
    Statements
*/

struct VarStmt : Statement
{
    std::string name;
    VariableTypePtr type;
    ExpressionPtr expr;

    bool constant;

    VarStmt(std::string s, VariableTypePtr type, ExpressionPtr val, bool constant) : name(std::move(s)), expr(std::move(val)), type(std::move(type)), constant(constant) {}
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
        this->span = std::move(span);
        this->kind = ExpressionKind::INTEGER;
    }
};

struct FloatExpression : Expression
{
    float num;

    FloatExpression(Span span, float n) : num(std::move(n))
    {
        this->span = std::move(span);
        this->kind = ExpressionKind::FLOAT;
    }
};

struct StringExpression : Expression
{
    std::string val;

    StringExpression(Span span, std::string s) : val(std::move(s))
    {
        this->span = std::move(span);
        this->kind = ExpressionKind::STRING;
    }
};

struct IdentifierLiteral : Expression
{
    std::string name;

    IdentifierLiteral(Span span, std::string s) : name(std::move(s))
    {
        this->span = std::move(span);
        this->kind = ExpressionKind::IDENT;
    }
};

struct BinaryOp : Expression
{
    BinaryOpType op;
    ExpressionPtr left;
    ExpressionPtr right;

    BinaryOp(Span span, BinaryOpType o, ExpressionPtr l, ExpressionPtr r) : op(o), left(std::move(l)), right(std::move(r))
    {
        this->span = std::move(span);
        this->kind = ExpressionKind::BINOP;
    }
};

struct AssignmentOp : Expression
{
    AssignmentOpType op;
    ExpressionPtr left;
    ExpressionPtr right;

    AssignmentOp(Span span, AssignmentOpType o, ExpressionPtr l, ExpressionPtr r) : op(o), left(std::move(l)), right(std::move(r))
    {
        this->span = std::move(span);
        this->kind = ExpressionKind::ASSIGNMENT;
    }
};

struct FieldExpr : Expression
{
    ExpressionPtr parent;
    std::string field;

    FieldExpr(Span span, ExpressionPtr parent, std::string field) : parent(std::move(parent)), field(std::move(field))
    {
        this->span = std::move(span);
        this->kind = ExpressionKind::FIELD;
    }
};

struct CallExpr : Expression
{
    ExpressionPtr parent;
    std::vector<ExpressionPtr> args;

    CallExpr(Span span, ExpressionPtr parent, std::vector<ExpressionPtr> args) : parent(std::move(parent)), args(std::move(args))
    {
        this->span = std::move(span);
        this->kind = ExpressionKind::CALL;
    }
};

struct AccessExpr : Expression
{
    ExpressionPtr parent;
    ExpressionPtr index;

    AccessExpr(Span span, ExpressionPtr parent, ExpressionPtr index) : parent(std::move(parent)), index(std::move(index))
    {
        this->span = std::move(span);
        this->kind = ExpressionKind::ACCESS;
    }
};
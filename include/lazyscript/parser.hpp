#pragma once

#include <memory>
#include <iostream>

struct Declaration
{
    virtual ~Declaration() = default;
};
using DeclarationPtr = std::unique_ptr<Declaration>;

struct FunctionDeclaration : Declaration
{
    std::string name;

    FunctionDeclaration(std::string n) : name(n) {}
};

struct Statement
{
    virtual ~Statement() = default;
};

using StatementPtr = std::unique_ptr<Statement>;

struct VarStatement : Statement
{
    std::string name;
};

struct Expression
{
    virtual ~Expression() = default;
};

using ExpressionPtr = std::unique_ptr<Expression>;


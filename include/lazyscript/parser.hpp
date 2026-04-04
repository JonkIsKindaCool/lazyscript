#pragma once

#include <vector>
#include <format>
#include <memory>

#include "lazyscript/ast.hpp"
#include "lazyscript/lexer.hpp"
#include "lazyscript/token_formatter.hpp"

class Parser
{
private:
    std::string src;
    std::string name;
    Lexer lexer;

    int pos;
private:
    Token peek();
    Token advance();

    DeclarationPtr parseDeclaration();

    StatementPtr parseStatement();

    ExpressionPtr parseExpression();
    
    ExpressionPtr parsePrimitive();

    void unexpected(Token t);
    void expect(TokenKind t);
    bool maybe(TokenKind t);
    VariableTypePtr parseType();
    std::string get_ident();


public:
    Parser(std::string src, std::string name);
    ~Parser();
    
    std::vector<DeclarationPtr> parse();
};
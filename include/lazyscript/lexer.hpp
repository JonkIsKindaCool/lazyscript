#pragma once

#include <string>
#include <cctype>
#include <unordered_map>
#include <stdexcept>
#include <format>

enum class TokenKind
{
    FUNC,
    LET,
    CONST,
    IF,
    ELSE,
    WHILE,
    FOR,
    IN,
    RETURN,
    STRUCT,

    I32,
    I64,
    F32,
    F64,
    BOOL,
    CHAR,
    STRING,
    PTR,

    TRUE,
    FALSE,
    NULL_LITERAL,

    IDENTIFIER,

    INTEGER_LITERAL,
    FLOAT_LITERAL,
    STRING_LITERAL,
    CHAR_LITERAL,

    LPAREN,
    RPAREN,
    LBRACE,
    RBRACE,
    LBRACKET,
    RBRACKET,
    COMMA,
    COLON,
    SEMICOLON,
    DOT,
    ARROW,
    RANGE,

    ASSIGN,
    PLUS,
    MINUS,
    STAR,
    SLASH,
    PERCENT,
    EQUAL_EQUAL,
    NOT_EQUAL,
    GREATER,
    LESS,
    GREATER_EQUAL,
    LESS_EQUAL,
    AMPERSAND_AMPERSAND,
    PIPE_PIPE,
    BANG,
    AMPERSAND,
    PIPE,
    CARET,
    TILDE,
    LESS_LESS,
    GREATER_GREATER,

    PLUS_EQUAL,
    MINUS_EQUAL,
    STAR_EQUAL,
    SLASH_EQUAL,

    EOF_TOKEN,
    ERROR
};

struct Span
{
    int line;
    int startPos;
    int endPos;
    int rawpos;
};

struct Token
{
    TokenKind kind;
    std::string value;
    Span span;
};

class Lexer
{
private:
    std::string src;
    std::string name;

    int rawPos;
    int line;
    int pos;

    Token cached;
    bool hasCached;

private:
    char current();
    char nextc();
    void advance();

    void skipWhitespace();

    Token makeToken(TokenKind kind, int start, int end, std::string value = "");

    Token identifier();
    Token number();
    Token stringLiteral();
    Token charLiteral();

    Token lexToken();

public:
    Lexer(std::string src, std::string name);
    ~Lexer();

    Token peek();
    Token next();

    bool maybe(TokenKind kind);
};
#include "lazyscript/lexer.hpp"

static std::unordered_map<std::string, TokenKind> keywords = {
    {"func", FUNC}, {"let", LET}, {"const", CONST}, {"if", IF}, {"else", ELSE}, {"while", WHILE}, {"for", FOR}, {"in", IN}, {"return", RETURN}, {"struct", STRUCT},

    {"i32", I32},
    {"i64", I64},
    {"f32", F32},
    {"f64", F64},
    {"bool", BOOL},
    {"char", CHAR},
    {"string", STRING},
    {"ptr", PTR},

    {"true", TRUE},
    {"false", FALSE},
    {"null", NULL_LITERAL}};

Lexer::Lexer(std::string src, std::string name)
    : src(src), name(name), rawPos(0), line(1), pos(0), hasCached(false) {}

Lexer::~Lexer() {}

char Lexer::current()
{
    if (pos >= (int)src.size())
        return '\0';
    return src[pos];
}

char Lexer::nextc()
{
    if (pos + 1 >= (int)src.size())
        return '\0';
    return src[pos + 1];
}

void Lexer::advance()
{
    if (current() == '\n')
        line++;
    pos++;
    rawPos++;
}

void Lexer::skipWhitespace()
{
    while (true)
    {
        char c = current();

        if (c == ' ' || c == '\t' || c == '\r')
        {
            advance();
        }
        else if (c == '\n')
        {
            advance();
        }
        else if (c == '/' && nextc() == '/')
        {
            while (current() != '\n' && current() != '\0')
                advance();
        }
        else
        {
            break;
        }
    }
}

Token Lexer::makeToken(TokenKind kind, int start, int end, std::string value)
{
    return Token{
        kind,
        value,
        Span{line, start, end, rawPos}};
}

Token Lexer::identifier()
{
    int start = pos;

    while (std::isalnum(current()) || current() == '_')
        advance();

    std::string text = src.substr(start, pos - start);

    if (keywords.contains(text))
        return makeToken(keywords[text], start, pos - 1, text);

    return makeToken(IDENTIFIER, start, pos - 1, text);
}

Token Lexer::number()
{
    int start = pos;
    bool isFloat = false;

    while (std::isdigit(current()))
        advance();

    if (current() == '.' && std::isdigit(nextc()))
    {
        isFloat = true;
        advance();

        while (std::isdigit(current()))
            advance();
    }

    std::string text = src.substr(start, pos - start);

    return makeToken(
        isFloat ? FLOAT_LITERAL : INTEGER_LITERAL,
        start,
        pos - 1,
        text);
}

Token Lexer::stringLiteral()
{
    int start = pos;
    advance();

    std::string value;

    while (current() != '"' && current() != '\0')
    {
        value += current();
        advance();
    }

    if (current() != '"')
        throw std::runtime_error("Unterminated string");

    advance();

    return makeToken(STRING_LITERAL, start, pos - 1, value);
}

Token Lexer::charLiteral()
{
    int start = pos;
    advance();
    char value = current();
    advance();

    if (current() != '\'')
        throw std::runtime_error("Invalid char literal");

    advance();

    return makeToken(CHAR_LITERAL, start, pos - 1, std::string(1, value));
}

Token Lexer::lexToken()
{
    skipWhitespace();

    int start = pos;
    char c = current();

    if (c == '\0')
        return makeToken(EOF_TOKEN, pos, pos);

    if (std::isalpha(c) || c == '_')
        return identifier();

    if (std::isdigit(c))
        return number();

    switch (c)
    {
    case '(':
        advance();
        return makeToken(LPAREN, start, pos - 1);
    case ')':
        advance();
        return makeToken(RPAREN, start, pos - 1);
    case '{':
        advance();
        return makeToken(LBRACE, start, pos - 1);
    case '}':
        advance();
        return makeToken(RBRACE, start, pos - 1);
    case '[':
        advance();
        return makeToken(LBRACKET, start, pos - 1);
    case ']':
        advance();
        return makeToken(RBRACKET, start, pos - 1);

    case ',':
        advance();
        return makeToken(COMMA, start, pos - 1);
    case ':':
        advance();
        return makeToken(COLON, start, pos - 1);
    case ';':
        advance();
        return makeToken(SEMICOLON, start, pos - 1);

    case '.':
        if (nextc() == '.')
        {
            advance();
            advance();
            return makeToken(RANGE, start, pos - 1);
        }
        advance();
        return makeToken(DOT, start, pos - 1);

    case '"':
        return stringLiteral();
    case '\'':
        return charLiteral();

    case '+':
        if (nextc() == '=')
        {
            advance();
            advance();
            return makeToken(PLUS_EQUAL, start, pos - 1);
        }
        advance();
        return makeToken(PLUS, start, pos - 1);

    case '-':
        if (nextc() == '>')
        {
            advance();
            advance();
            return makeToken(ARROW, start, pos - 1);
        }
        if (nextc() == '=')
        {
            advance();
            advance();
            return makeToken(MINUS_EQUAL, start, pos - 1);
        }
        advance();
        return makeToken(MINUS, start, pos - 1);

    case '*':
        if (nextc() == '=')
        {
            advance();
            advance();
            return makeToken(STAR_EQUAL, start, pos - 1);
        }
        advance();
        return makeToken(STAR, start, pos - 1);

    case '/':
        if (nextc() == '=')
        {
            advance();
            advance();
            return makeToken(SLASH_EQUAL, start, pos - 1);
        }
        advance();
        return makeToken(SLASH, start, pos - 1);

    case '=':
        if (nextc() == '=')
        {
            advance();
            advance();
            return makeToken(EQUAL_EQUAL, start, pos - 1);
        }
        advance();
        return makeToken(ASSIGN, start, pos - 1);

    case '!':
        if (nextc() == '=')
        {
            advance();
            advance();
            return makeToken(NOT_EQUAL, start, pos - 1);
        }
        advance();
        return makeToken(BANG, start, pos - 1);

    case '<':
        if (nextc() == '<')
        {
            advance();
            advance();
            return makeToken(LESS_LESS, start, pos - 1);
        }
        if (nextc() == '=')
        {
            advance();
            advance();
            return makeToken(LESS_EQUAL, start, pos - 1);
        }
        advance();
        return makeToken(LESS, start, pos - 1);

    case '>':
        if (nextc() == '>')
        {
            advance();
            advance();
            return makeToken(GREATER_GREATER, start, pos - 1);
        }
        if (nextc() == '=')
        {
            advance();
            advance();
            return makeToken(GREATER_EQUAL, start, pos - 1);
        }
        advance();
        return makeToken(GREATER, start, pos - 1);

    case '&':
        if (nextc() == '&')
        {
            advance();
            advance();
            return makeToken(AMPERSAND_AMPERSAND, start, pos - 1);
        }
        advance();
        return makeToken(AMPERSAND, start, pos - 1);

    case '|':
        if (nextc() == '|')
        {
            advance();
            advance();
            return makeToken(PIPE_PIPE, start, pos - 1);
        }
        advance();
        return makeToken(PIPE, start, pos - 1);

    case '^':
        advance();
        return makeToken(CARET, start, pos - 1);
    case '~':
        advance();
        return makeToken(TILDE, start, pos - 1);
    case '%':
        advance();
        return makeToken(PERCENT, start, pos - 1);
    }

    throw std::runtime_error(
        std::format("LEXER ERROR: {}:{}:{} unexpected '{}'", name, line, pos, c));
}

Token Lexer::peek()
{
    if (!hasCached)
    {
        cached = lexToken();
        hasCached = true;
    }
    return cached;
}

Token Lexer::next()
{
    if (hasCached)
    {
        hasCached = false;
        return cached;
    }
    return lexToken();
}

bool Lexer::maybe(TokenKind kind)
{
    if (peek().kind == kind)
    {
        next();
        return true;
    }
    return false;
}
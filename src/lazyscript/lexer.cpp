#include "lazyscript/lexer.hpp"

static std::unordered_map<std::string, TokenKind> keywords = {
    {"func", TokenKind::FUNC}, 
    {"let", TokenKind::LET}, 
    {"const", TokenKind::CONST}, 
    {"if", TokenKind::IF}, 
    {"else", TokenKind::ELSE}, 
    {"while", TokenKind::WHILE}, 
    {"for", TokenKind::FOR}, 
    {"in", TokenKind::IN}, 
    {"return", TokenKind::RETURN}, 
    {"struct", TokenKind::STRUCT},

    {"true", TokenKind::TRUE},
    {"false", TokenKind::FALSE},
    {"null", TokenKind::NULL_LITERAL}};

Lexer::Lexer(std::string src, std::string name)
    : src(std::move(src)), name(std::move(name)), rawPos(0), line(1), pos(0), hasCached(false) {}

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

    return makeToken(TokenKind::IDENTIFIER, start, pos - 1, text);
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
        isFloat ? TokenKind::FLOAT_LITERAL : TokenKind::INTEGER_LITERAL,
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

    return makeToken(TokenKind::STRING_LITERAL, start, pos - 1, value);
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

    return makeToken(TokenKind::CHAR_LITERAL, start, pos - 1, std::string(1, value));
}

Token Lexer::lexToken()
{
    skipWhitespace();

    int start = pos;
    char c = current();

    if (c == '\0')
        return makeToken(TokenKind::EOF_TOKEN, pos, pos);

    if (std::isalpha(c) || c == '_')
        return identifier();

    if (std::isdigit(c))
        return number();

    switch (c)
    {
    case '(':
        advance();
        return makeToken(TokenKind::LPAREN, start, pos - 1);
    case ')':
        advance();
        return makeToken(TokenKind::RPAREN, start, pos - 1);
    case '{':
        advance();
        return makeToken(TokenKind::LBRACE, start, pos - 1);
    case '}':
        advance();
        return makeToken(TokenKind::RBRACE, start, pos - 1);
    case '[':
        advance();
        return makeToken(TokenKind::LBRACKET, start, pos - 1);
    case ']':
        advance();
        return makeToken(TokenKind::RBRACKET, start, pos - 1);

    case ',':
        advance();
        return makeToken(TokenKind::COMMA, start, pos - 1);
    case ':':
        advance();
        return makeToken(TokenKind::COLON, start, pos - 1);
    case ';':
        advance();
        return makeToken(TokenKind::SEMICOLON, start, pos - 1);

    case '.':
        if (nextc() == '.')
        {
            advance();
            advance();
            return makeToken(TokenKind::RANGE, start, pos - 1);
        }
        advance();
        return makeToken(TokenKind::DOT, start, pos - 1);

    case '"':
        return stringLiteral();
    case '\'':
        return charLiteral();

    case '+':
        if (nextc() == '=')
        {
            advance();
            advance();
            return makeToken(TokenKind::PLUS_EQUAL, start, pos - 1);
        }
        advance();
        return makeToken(TokenKind::PLUS, start, pos - 1);

    case '-':
        if (nextc() == '>')
        {
            advance();
            advance();
            return makeToken(TokenKind::ARROW, start, pos - 1);
        }
        if (nextc() == '=')
        {
            advance();
            advance();
            return makeToken(TokenKind::MINUS_EQUAL, start, pos - 1);
        }
        advance();
        return makeToken(TokenKind::MINUS, start, pos - 1);

    case '*':
        if (nextc() == '=')
        {
            advance();
            advance();
            return makeToken(TokenKind::STAR_EQUAL, start, pos - 1);
        }
        advance();
        return makeToken(TokenKind::STAR, start, pos - 1);

    case '/':
        if (nextc() == '=')
        {
            advance();
            advance();
            return makeToken(TokenKind::SLASH_EQUAL, start, pos - 1);
        }
        advance();
        return makeToken(TokenKind::SLASH, start, pos - 1);

    case '=':
        if (nextc() == '=')
        {
            advance();
            advance();
            return makeToken(TokenKind::EQUAL_EQUAL, start, pos - 1);
        }
        advance();
        return makeToken(TokenKind::ASSIGN, start, pos - 1);

    case '!':
        if (nextc() == '=')
        {
            advance();
            advance();
            return makeToken(TokenKind::NOT_EQUAL, start, pos - 1);
        }
        advance();
        return makeToken(TokenKind::BANG, start, pos - 1);

    case '<':
        if (nextc() == '<')
        {
            advance();
            advance();
            return makeToken(TokenKind::LESS_LESS, start, pos - 1);
        }
        if (nextc() == '=')
        {
            advance();
            advance();
            return makeToken(TokenKind::LESS_EQUAL, start, pos - 1);
        }
        advance();
        return makeToken(TokenKind::LESS, start, pos - 1);

    case '>':
        if (nextc() == '>')
        {
            advance();
            advance();
            return makeToken(TokenKind::GREATER_GREATER, start, pos - 1);
        }
        if (nextc() == '=')
        {
            advance();
            advance();
            return makeToken(TokenKind::GREATER_EQUAL, start, pos - 1);
        }
        advance();
        return makeToken(TokenKind::GREATER, start, pos - 1);

    case '&':
        if (nextc() == '&')
        {
            advance();
            advance();
            return makeToken(TokenKind::AMPERSAND_AMPERSAND, start, pos - 1);
        }
        advance();
        return makeToken(TokenKind::AMPERSAND, start, pos - 1);

    case '|':
        if (nextc() == '|')
        {
            advance();
            advance();
            return makeToken(TokenKind::PIPE_PIPE, start, pos - 1);
        }
        advance();
        return makeToken(TokenKind::PIPE, start, pos - 1);

    case '^':
        advance();
        return makeToken(TokenKind::CARET, start, pos - 1);
    case '~':
        advance();
        return makeToken(TokenKind::TILDE, start, pos - 1);
    case '%':
        advance();
        return makeToken(TokenKind::PERCENT, start, pos - 1);
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
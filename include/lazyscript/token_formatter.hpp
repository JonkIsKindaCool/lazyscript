#pragma once

#include <string>
#include "lazyscript/lexer.hpp"
#include <string_view>

constexpr std::string_view token_to_string(TokenKind kind) noexcept
{
    switch (kind)
    {
        case TokenKind::FUNC:           return "FUNC";
        case TokenKind::LET:            return "LET";
        case TokenKind::CONST:          return "CONST";
        case TokenKind::IF:             return "IF";
        case TokenKind::ELSE:           return "ELSE";
        case TokenKind::WHILE:          return "WHILE";
        case TokenKind::FOR:            return "FOR";
        case TokenKind::IN:             return "IN";
        case TokenKind::RETURN:         return "RETURN";
        case TokenKind::STRUCT:         return "STRUCT";

        case TokenKind::I32:            return "I32";
        case TokenKind::I64:            return "I64";
        case TokenKind::F32:            return "F32";
        case TokenKind::F64:            return "F64";
        case TokenKind::BOOL:           return "BOOL";
        case TokenKind::CHAR:           return "CHAR";
        case TokenKind::STRING:         return "STRING";
        case TokenKind::PTR:            return "PTR";

        case TokenKind::TRUE:           return "TRUE";
        case TokenKind::FALSE:          return "FALSE";
        case TokenKind::NULL_LITERAL:   return "NULL";

        case TokenKind::IDENTIFIER:     return "IDENTIFIER";

        case TokenKind::INTEGER_LITERAL:return "INTEGER_LITERAL";
        case TokenKind::FLOAT_LITERAL:  return "FLOAT_LITERAL";
        case TokenKind::STRING_LITERAL: return "STRING_LITERAL";
        case TokenKind::CHAR_LITERAL:   return "CHAR_LITERAL";

        case TokenKind::LPAREN:         return "(";
        case TokenKind::RPAREN:         return ")";
        case TokenKind::LBRACE:         return "{";
        case TokenKind::RBRACE:         return "}";
        case TokenKind::LBRACKET:       return "[";
        case TokenKind::RBRACKET:       return "]";
        case TokenKind::COMMA:          return ",";
        case TokenKind::COLON:          return ":";
        case TokenKind::SEMICOLON:      return ";";
        case TokenKind::DOT:            return ".";
        case TokenKind::ARROW:          return "->";
        case TokenKind::RANGE:          return "..";

        case TokenKind::ASSIGN:         return "=";
        case TokenKind::PLUS:           return "+";
        case TokenKind::MINUS:          return "-";
        case TokenKind::STAR:           return "*";
        case TokenKind::SLASH:          return "/";
        case TokenKind::PERCENT:        return "%";
        case TokenKind::EQUAL_EQUAL:    return "==";
        case TokenKind::NOT_EQUAL:      return "!=";
        case TokenKind::GREATER:        return ">";
        case TokenKind::LESS:           return "<";
        case TokenKind::GREATER_EQUAL:  return ">=";
        case TokenKind::LESS_EQUAL:     return "<=";
        case TokenKind::AMPERSAND_AMPERSAND: return "&&";
        case TokenKind::PIPE_PIPE:      return "||";
        case TokenKind::BANG:           return "!";
        case TokenKind::AMPERSAND:      return "&";
        case TokenKind::PIPE:           return "|";
        case TokenKind::CARET:          return "^";
        case TokenKind::TILDE:          return "~";
        case TokenKind::LESS_LESS:      return "<<";
        case TokenKind::GREATER_GREATER:return ">>";

        case TokenKind::PLUS_EQUAL:     return "+=";
        case TokenKind::MINUS_EQUAL:    return "-=";
        case TokenKind::STAR_EQUAL:     return "*=";
        case TokenKind::SLASH_EQUAL:    return "/=";

        case TokenKind::EOF_TOKEN:      return "EOF";
        case TokenKind::ERROR:          return "ERROR";

        default:                        return "UNKNOWN_TOKEN";
    }
}
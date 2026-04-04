#include "lazyscript/parser.hpp"
#include "lazyscript/lexer.hpp"

Parser::Parser(std::string src, std::string name)
	: src(std::move(src)),
	  name(std::move(name)),
	  lexer(this->src, this->name) {}

Parser::~Parser() {}

std::vector<DeclarationPtr> Parser::parse()
{
	std::vector<DeclarationPtr> body;

	while (!maybe(TokenKind::EOF_TOKEN))
	{
		body.push_back(std::move(parseDeclaration()));
	}

	return body;
}

DeclarationPtr Parser::parseDeclaration()
{
	Token tok = peek();

	if (tok.kind == TokenKind::FUNC)
	{
		advance();
		std::string name = get_ident();
		std::vector<FunctionArgumentPtr> args;
		std::vector<StatementPtr> body;

		expect(TokenKind::LPAREN);
		while (!maybe(TokenKind::RPAREN))
		{
			bool optional = false;

			if (maybe(TokenKind::COLON))
			{
				optional = true;
			}

			std::string name = get_ident();
			expect(TokenKind::COLON);
			VariableTypePtr type = parseType();

			args.push_back(std::make_unique<FunctionArgument>(std::move(name), std::move(type), optional));

			if (!maybe(TokenKind::COMMA))
			{
				expect(TokenKind::RPAREN);
				break;
			}
		}

		expect(TokenKind::LBRACE);

		TRACE(std::format("DECLARATION: Function {}, with number of args {} ", name, args.size()));
		TRACE("Function Expression logs: ");

		while (!maybe(TokenKind::RBRACE))
		{
			body.push_back(std::move(parseStatement()));
		}

		TRACE(std::format("Function Size: {} ", body.size()));

		return std::make_unique<FunctionDeclaration>(std::move(name), std::move(args), std::move(body));
	}
	else
	{

		unexpected(tok);
		return NULL;
	}
}

StatementPtr Parser::parseStatement()
{
	ExpressionPtr expr = parseExpression();
	maybe(TokenKind::SEMICOLON);
	return std::make_unique<ExpressionStmt>(std::move(expr));
}

ExpressionPtr Parser::parseExpression()
{
	return std::move(parsePrimitive());
}

ExpressionPtr Parser::parsePrimitive()
{
	Token tok = advance();

	if (tok.kind == TokenKind::INTEGER_LITERAL)
	{
		TRACE("EXPRESSION: Integer literal " + tok.value);
		return std::make_unique<IntExpression>(std::stoi(std::move(tok.value)));
	}
	else if (tok.kind == TokenKind::FLOAT_LITERAL)
	{
		TRACE("EXPRESSION: Float literal " + tok.value);
		return std::make_unique<FloatExpression>(std::stof(std::move(tok.value)));
	}

	unexpected(tok);
	return NULL;
}

Token Parser::peek()
{
	return lexer.peek();
}

Token Parser::advance()
{
	return lexer.next();
}

bool Parser::maybe(TokenKind t)
{
	return lexer.maybe(t);
}

void Parser::unexpected(Token t)
{
	throw std::runtime_error(
		std::format("SYNTAX ERROR: {}:{}:{} unexpected token (TYPE: {} | VALUE: {})", name, t.span.line, t.span.line, token_to_string(t.kind), t.value));
}

void Parser::expect(TokenKind t)
{
	Token tok = lexer.next();

	if (tok.kind != t)
	{
		throw std::runtime_error(
			std::format("SYNTAX ERROR: {}:{}:{} expected token {}, got token {}", name, tok.span.line, tok.span.line, token_to_string(t), token_to_string(tok.kind)));
	}
}

std::string Parser::get_ident()
{
	Token tok = lexer.peek();

	if (tok.kind == TokenKind::IDENTIFIER)
	{
		lexer.next();
		return tok.value;
	}

	unexpected(tok);
	return tok.value;
}

VariableTypePtr Parser::parseType()
{
	std::string name = get_ident();
	std::vector<VariableTypePtr> generics;

	if (maybe(TokenKind::LESS))
	{
		while (true)
		{
			generics.push_back(parseType());

			if (!maybe(TokenKind::COMMA))
			{
				expect(TokenKind::GREATER);
				break;
			}
		}
	}

	return std::make_unique<VariableType>(std::move(name), std::move(generics));
}
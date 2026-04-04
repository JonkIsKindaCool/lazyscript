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

		TRACE(std::format("DECLARATION: Function {}, ", name));

		expect(TokenKind::LPAREN);
		while (!maybe(TokenKind::RPAREN))
		{
			bool optional = false;

			if (maybe(TokenKind::COLON))
			{
				optional = true;
			}

			std::string name = get_ident();

			NEWLINE();
			TRACE("Added an argument: " + name);

			expect(TokenKind::COLON);
			VariableTypePtr type = parseType();
			NEWLINE();

			args.push_back(std::make_unique<FunctionArgument>(std::move(name), std::move(type), optional));

			if (!maybe(TokenKind::COMMA))
			{
				expect(TokenKind::RPAREN);
				break;
			}
		}

		TRACE(std::format("Total Arguments: {} ", args.size()));

		expect(TokenKind::LBRACE);

		TRACE("Function Expression logs: ");

		NEWLINE();

		while (!maybe(TokenKind::RBRACE))
		{
			body.push_back(std::move(parseStatement()));
		}

		NEWLINE();

		TRACE(std::format("Function Size: {} ", body.size()));

		TRACE("END OF FUNCTION DECLARATION");
		NEWLINE();

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
	return parseAssignment();
}

ExpressionPtr Parser::parseAssignment()
{
	TRACE("EXPR: parseAssignment");

	auto left = parseOR();

	Token tok = peek();

	AssignmentOpType op;
	bool isAssign = true;

	switch (tok.kind)
	{
	case TokenKind::ASSIGN:
		op = AssignmentOpType::Assign;
		break;
	case TokenKind::PLUS_EQUAL:
		op = AssignmentOpType::AddAssign;
		break;
	case TokenKind::MINUS_EQUAL:
		op = AssignmentOpType::SubAssign;
		break;
	case TokenKind::STAR_EQUAL:
		op = AssignmentOpType::MulAssign;
		break;
	case TokenKind::SLASH_EQUAL:
		op = AssignmentOpType::DivAssign;
		break;
	default:
		isAssign = false;
		break;
	}

	if (!isAssign)
	{
		TRACE("EXPR: no assignment");
		return left;
	}

	advance();

	TRACE("EXPR: assignment operator");

	auto right = parseAssignment(); 

	return std::make_unique<AssignmentOp>(
		tok.span,
		op,
		std::move(left),
		std::move(right));
}

ExpressionPtr Parser::parseOR()
{
	TRACE("EXPR: parseOR");

	auto left = parseAND();

	while (true)
	{
		Token op = peek();

		if (maybe(TokenKind::PIPE_PIPE))
		{
			TRACE("EXPR: OR (||)");
			auto right = parseAND();
			left = std::make_unique<BinaryOp>(op.span, BinaryOpType::Or, std::move(left), std::move(right));
		}
		else
			break;
	}

	return left;
}

ExpressionPtr Parser::parseAND()
{
	TRACE("EXPR: parseAND");

	auto left = parseBitwise_OR();

	while (true)
	{
		Token op = peek();

		if (maybe(TokenKind::AMPERSAND_AMPERSAND))
		{
			TRACE("EXPR: AND (&&)");
			auto right = parseBitwise_OR();
			left = std::make_unique<BinaryOp>(op.span, BinaryOpType::And, std::move(left), std::move(right));
		}
		else
			break;
	}

	return left;
}

ExpressionPtr Parser::parseBitwise_OR()
{
	TRACE("EXPR: parseBitwise_OR");

	auto left = parseBitwise_XOR();

	while (true)
	{
		Token op = peek();

		if (maybe(TokenKind::PIPE))
		{
			TRACE("EXPR: Bitwise OR (|)");
			auto right = parseBitwise_XOR();
			left = std::make_unique<BinaryOp>(op.span, BinaryOpType::BitwiseOr, std::move(left), std::move(right));
		}
		else
			break;
	}

	return left;
}

ExpressionPtr Parser::parseBitwise_XOR()
{
	TRACE("EXPR: parseBitwise_XOR");

	auto left = parseBitwise_AND();

	while (true)
	{
		Token op = peek();

		if (maybe(TokenKind::CARET))
		{
			TRACE("EXPR: Bitwise XOR (^)");
			auto right = parseBitwise_AND();
			left = std::make_unique<BinaryOp>(op.span, BinaryOpType::BitwiseXor, std::move(left), std::move(right));
		}
		else
			break;
	}

	return left;
}

ExpressionPtr Parser::parseBitwise_AND()
{
	TRACE("EXPR: parseBitwise_AND");

	auto left = parseEquality();

	while (true)
	{
		Token op = peek();

		if (maybe(TokenKind::AMPERSAND))
		{
			TRACE("EXPR: Bitwise AND (&)");
			auto right = parseEquality();
			left = std::make_unique<BinaryOp>(op.span, BinaryOpType::BitwiseAnd, std::move(left), std::move(right));
		}
		else
			break;
	}

	return left;
}

ExpressionPtr Parser::parseEquality()
{
	TRACE("EXPR: parseEquality");

	auto left = parseComparison();

	while (true)
	{
		Token op = peek();

		if (maybe(TokenKind::EQUAL_EQUAL))
		{
			TRACE("EXPR: Equal (==)");
			auto right = parseComparison();
			left = std::make_unique<BinaryOp>(op.span, BinaryOpType::Equal, std::move(left), std::move(right));
		}
		else if (maybe(TokenKind::NOT_EQUAL))
		{
			TRACE("EXPR: Not Equal (!=)");
			auto right = parseComparison();
			left = std::make_unique<BinaryOp>(op.span, BinaryOpType::NotEqual, std::move(left), std::move(right));
		}
		else
			break;
	}

	return left;
}

ExpressionPtr Parser::parseComparison()
{
	TRACE("EXPR: parseComparison");

	auto left = parseShift();

	while (true)
	{
		Token op = peek();

		if (maybe(TokenKind::GREATER))
		{
			TRACE("EXPR: Greater (>)");
			left = std::make_unique<BinaryOp>(op.span, BinaryOpType::Greater, std::move(left), parseShift());
		}
		else if (maybe(TokenKind::LESS))
		{
			TRACE("EXPR: Less (<)");
			left = std::make_unique<BinaryOp>(op.span, BinaryOpType::Less, std::move(left), parseShift());
		}
		else if (maybe(TokenKind::GREATER_EQUAL))
		{
			TRACE("EXPR: GreaterEqual (>=)");
			left = std::make_unique<BinaryOp>(op.span, BinaryOpType::GreaterEqual, std::move(left), parseShift());
		}
		else if (maybe(TokenKind::LESS_EQUAL))
		{
			TRACE("EXPR: LessEqual (<=)");
			left = std::make_unique<BinaryOp>(op.span, BinaryOpType::LessEqual, std::move(left), parseShift());
		}
		else
			break;
	}

	return left;
}

ExpressionPtr Parser::parseShift()
{
	TRACE("EXPR: parseShift");

	auto left = parseAdditive();

	while (true)
	{
		Token op = peek();

		if (maybe(TokenKind::LESS_LESS))
		{
			TRACE("EXPR: Left Shift (<<)");
			left = std::make_unique<BinaryOp>(op.span, BinaryOpType::LeftShift, std::move(left), parseAdditive());
		}
		else if (maybe(TokenKind::GREATER_GREATER))
		{
			TRACE("EXPR: Right Shift (>>)");
			left = std::make_unique<BinaryOp>(op.span, BinaryOpType::RightShift, std::move(left), parseAdditive());
		}
		else
			break;
	}

	return left;
}

ExpressionPtr Parser::parseAdditive()
{
	TRACE("EXPR: parseAdditive");

	auto left = parseMultiplicative();

	while (true)
	{
		Token op = peek();

		if (maybe(TokenKind::PLUS))
		{
			TRACE("EXPR: Add (+)");
			left = std::make_unique<BinaryOp>(op.span, BinaryOpType::Add, std::move(left), parseMultiplicative());
		}
		else if (maybe(TokenKind::MINUS))
		{
			TRACE("EXPR: Subtract (-)");
			left = std::make_unique<BinaryOp>(op.span, BinaryOpType::Subtract, std::move(left), parseMultiplicative());
		}
		else
			break;
	}

	return left;
}

ExpressionPtr Parser::parseMultiplicative()
{
	TRACE("EXPR: parseMultiplicative");

	auto left = parsePrimitive();

	while (true)
	{
		Token op = peek();

		if (maybe(TokenKind::STAR))
		{
			TRACE("EXPR: Multiply (*)");
			left = std::make_unique<BinaryOp>(op.span, BinaryOpType::Multiply, std::move(left), parsePrimitive());
		}
		else if (maybe(TokenKind::SLASH))
		{
			TRACE("EXPR: Divide (/)");
			left = std::make_unique<BinaryOp>(op.span, BinaryOpType::Divide, std::move(left), parsePrimitive());
		}
		else if (maybe(TokenKind::PERCENT))
		{
			TRACE("EXPR: Modulo (%)");
			left = std::make_unique<BinaryOp>(op.span, BinaryOpType::Modulo, std::move(left), parsePrimitive());
		}
		else
			break;
	}

	return left;
}

ExpressionPtr Parser::parsePrimitive()
{
	Token tok = advance();

	if (tok.kind == TokenKind::INTEGER_LITERAL)
	{
		TRACE("EXPRESSION: Integer literal " + tok.value);
		return std::make_unique<IntExpression>(std::move(tok.span), std::stoi(std::move(tok.value)));
	}
	else if (tok.kind == TokenKind::FLOAT_LITERAL)
	{
		TRACE("EXPRESSION: Float literal " + tok.value);
		return std::make_unique<FloatExpression>(std::move(tok.span), std::stof(std::move(tok.value)));
	}
	else if (tok.kind == TokenKind::STRING_LITERAL)
	{
		TRACE("EXPRESSION: String literal " + tok.value);
		return std::make_unique<StringExpression>(std::move(tok.span), std::move(tok.value));
	}
	else if (tok.kind == TokenKind::IDENTIFIER)
	{
		TRACE("EXPRESSION: Identifier literal " + tok.value);
		return std::make_unique<IdentifierLiteral>(std::move(tok.span), std::move(tok.value));
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
	TRACE("AST Type: " + name);
	std::vector<VariableTypePtr> generics;

	if (maybe(TokenKind::LESS))
	{
		TRACE("Generics: ");
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
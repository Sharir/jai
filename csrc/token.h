#ifndef _TOKEN_H
#define _TOKEN_H

#include <string>

enum TokenType {
	UNKNOWN 				= 0,
	WHITESPACE 				= 1, 
	COMMENT_SINGLE			= 2,	// //
	COMMENT_BLOCK_OPEN		= 3,	// /* 
	COMMENT_BLOCK_CLOSE 	= 4,	// */ 


	LITERAL_NUMBER 			= 5,	// 1, 1.0
	LITERAL_CHARACTER 		= 6,	// 'j'
	LITERAL_BOOLEAN 		= 7,	// true or false
	LITERAL_STRING			= 8,	// "hello"


	PAREN_LEFT				= 11,	// (
	PAREN_RIGHT				= 12,	// )
	BRACKET_LEFT			= 13,	// {
	BRACKET_RIGHT			= 14,	// }


	ANGLE_BRACKET_LEFT 		= 17,	// <
	ANGLE_BRACKET_RIGHT 	= 18,	// >


	COMMA					= 19,	// ,
	SEMI_COLON				= 20,	// ;
	DOUBLE_COLON			= 21,	// ::
	IDENTIFIER				= 22,	// a: int


	OP_DECLERATION			= 23,	// name: type
	OP_INFERENCE, 				 	// _ := _

	OP_BIN_ADD, 				  	// _ + _
	OP_BIN_SUB,						// _ - _
	OP_BIN_MULT,					// _ * _
	OP_BIN_DIV,						// _ / _
	OP_BIN_MOD,						// _ % _

	OP_BIN_BITWISE_XOR,				// _ ^ _
	OP_BIN_BITWISE_AND,				// _ & _
	OP_BIN_BITWISE_OR,				// _ | _
	OP_BIN_BITWISE_SHIFT_LEFT,		// _ << _
	OP_BIN_BITWISE_SHIFT_RIGHT,		// _ >> _

	OP_BIN_LOGICAL_AND, 			// _ && _
	OP_BIN_LOGICAL_OR,				// _ || _
	OP_BIN_EQUALS,					// _ == _
	OP_BIN_NOT_EQUALS,				// _ != _
	OP_BIN_GREATOR,					// _ > _
	OP_BIN_GREATOR_EQUAL,			// _ >= _
	OP_BIN_LESS_THAN,				// _ < _
	OP_BIN_LESS_EQUAL,				// _ >= _

	OP_ASSIGN,						// _ = _
	OP_ADD_ASSIGN,					// _ += _
	OP_SUB_ASSIGN,					// _ -= _
	OP_DIV_ASSIGN,					// _ /= _
	OP_MULT_ASSIGN,					// _ *= _
	OP_MOD_ASSIGN,					// _ %= _
	OP_XOR_ASSIGN,					// _ ^= _
	OP_AND_ASSIGN,					// _ &= _
	OP_OR_ASSIGN,					// _ |= _ 
	OP_SHIFT_LEFT_ASSIGN,			// _ <<= _
	OP_SHIFT_RIGHT_ASSIGN,			// _ >>= _
	
	OP_TURNARY,						// _ ? _ : _

	OP_UNI_INC,						// ++_
	OP_UNI_DEC,						// --_
	OP_UNI_NOT,						// !_
	OP_UNI_TILDA,					// ~_
	OP_UNI_ADDRESS, 				// &
	OP_UNI_DEREFERENCE,				// *
};

typedef struct {
	TokenType type;
	std::string lexeme;
} Token;

#endif
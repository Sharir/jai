#ifndef _TOKEN_H
#define _TOKEN_H

#include <string>

enum TokenType {
	UNKNOWN                     = 0,
	WHITESPACE                  = 1,
	COMMENT_SINGLE              = 2,	// //
	COMMENT_BLOCK_OPEN          = 3,	// /*
	COMMENT_BLOCK_CLOSE         = 4,	// */


	LITERAL_NUMBER              = 5,	// 1, 1.0
	LITERAL_CHARACTER           = 6,	// 'j'
	LITERAL_BOOLEAN             = 7,	// true, false
	LITERAL_STRING              = 8,	// "hello"


	PAREN_LEFT                  = 11,	// (
	PAREN_RIGHT                 = 12,	// )
	BRACKET_LEFT                = 13,	// {
	BRACKET_RIGHT               = 14,	// }
	// SQUARE_BRACKET_LEFT         = 15,	// [
	// SQUARE_BRACKET_RIGHT        = 16,	// ]
	ANGLE_BRACKET_LEFT          = 17,	// <
	ANGLE_BRACKET_RIGHT         = 18,	// >


	COMMA                       = 19,	// ,
	SEMI_COLON                  = 20,	// ;
	DOUBLE_COLON                = 21,	// ::
	IDENTIFIER                  = 22,	// var/const/func name


	OP_DECLERATION              = 23,	// name: type
	OP_INFERENCE                    ,	// :=

	OP_BIN_ADD                      ,	// +
	OP_BIN_SUB                      ,	// -
	OP_BIN_MULT                     ,	// *
	OP_BIN_DIV                      ,	// /
	OP_BIN_MOD                      ,	// %

	OP_BIN_BITWISE_XOR              ,	// ^
	OP_BIN_BITWISE_AND              ,	// &
	OP_BIN_BITWISE_OR               ,	// |
	OP_BIN_BITWISE_SHIFT_RIGHT      ,	// >>
	OP_BIN_BITWISE_SHIFT_LEFT       ,	// <<

	OP_BIN_LOGICAL_AND              ,	// &&
	OP_BIN_LOGICAL_OR               ,	// ||
	OP_BIN_EQUALS                   ,	// ==
	OP_BIN_NOT_EQUALS               ,	// !=
	OP_BIN_LESS                     ,	// <
	OP_BIN_LESS_EQUAL               ,	// <=
	OP_BIN_GREATER                  ,	// >
	OP_BIN_GREATER_EQUAL            ,	// >=

	OP_ASSIGN                       ,	// =
	OP_ADD_ASSIGN                   ,	// +=
	OP_SUB_ASSIGN                   ,	// -=
	OP_MULT_ASSIGN                  ,	// *=
	OP_DIV_ASSIGN                   ,	// /=
	OP_MOD_ASSIGN                   ,	// %=
	OP_XOR_ASSIGN                   ,	// ^=
	OP_AND_ASSIGN                   ,	// &=
	OP_OR_ASSIGN                    ,	// |=
	OP_SHIFT_RIGHT_ASSIGN           ,	// >>=
	OP_SHIFT_LEFT_ASSIGN            ,	// <<=

	OP_TERNARY_QUESTION             ,	// ? [bool ? expr1 : expr2]
	OP_TERNARY_COLON                ,	// : [bool ? expr1 : expr2]

	OP_UNI_INC                      ,	// ++
	OP_UNI_DEC                      ,	// --
	OP_UNI_NOT                      ,	// !
	OP_UNI_BITWISE_NOT              ,	// ~
	OP_UNI_ADDRESS                  ,	// &
	OP_UNI_DEREFERENCE              ,	// *
};

typedef struct {
	TokenType type;
	std::string lexeme;
} Token;

#endif

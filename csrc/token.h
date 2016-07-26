#ifndef _TOKEN_H
#define _TOKEN_H

#include <string>

enum TokenFlag {
	FLAG_OP = 1 << 30, //General - on if token is of type operator
	FLAG_OP_UNI = 1 << 29, //Unirary operator
	FLAG_OP_BIN = 1 << 28, //Binary operator
	FLAG_OP_TERNARY = 1 << 27, //Ternary operator
	FLAG_OP_ASSIGN = 1 << 26, //Assign-type operator
	FLAG_KW = 1 << 25, //Keyword
	FLAG_BT = 1 << 24, //Built-in type
	FLAG_DIR = 1 << 23, //Preprocessor directive
	FLAG_BRACKETS = 1 << 22, //Parentheses and brackets
	FLAG_LITERAL = 1 << 21, //Any type literal
};

enum TokenType {
	UNKNOWN                     = 0,
	WHITESPACE                  = 1,
	COMMENT_SINGLE              = 2,	// //
	COMMENT_BLOCK_OPEN          = 3,	// /*
	COMMENT_BLOCK_CLOSE         = 4,	// */


	LITERAL_INTEGER             = 5 | FLAG_LITERAL,	// 1
	LITERAL_FLOATING_POINT      = 6 | FLAG_LITERAL,	// 1.0
	LITERAL_CHARACTER           = 7 | FLAG_LITERAL,	// 'j'
	LITERAL_STRING              = 8 | FLAG_LITERAL,	// "hello"


	PAREN_LEFT                  = 11 | FLAG_BRACKETS,	// (
	PAREN_RIGHT                 = 12 | FLAG_BRACKETS,	// )
	BRACKET_LEFT                = 13 | FLAG_BRACKETS,	// {
	BRACKET_RIGHT               = 14 | FLAG_BRACKETS,	// }
	SQUARE_BRACKET_LEFT         = 15 | FLAG_BRACKETS,	// [
	SQUARE_BRACKET_RIGHT        = 16 | FLAG_BRACKETS,	// ]
	ANGLE_BRACKET_LEFT          = 17 | FLAG_BRACKETS,	// <
	ANGLE_BRACKET_RIGHT         = 18 | FLAG_BRACKETS,	// >


	COMMA                       = 19,	// ,
	SEMI_COLON                  = 20,	// ;
	DOUBLE_COLON                = 21,	// :: - access to namespaces and possibly static stuff
	IDENTIFIER                  = 22,	// var/const/func/type name


	OP_DECLARATION              = 23 | FLAG_OP,	// name: type
	OP_INFERENCE                = 24 | FLAG_OP,	// :=
	OP_FUNC_DECLARATION         = 25 | FLAG_OP,	// :: - also used for structs

	OP_BIN_ADD                  = 26 | FLAG_OP | FLAG_OP_BIN,	// +
	OP_BIN_SUB                  = 27 | FLAG_OP | FLAG_OP_BIN,	// -
	OP_BIN_MULT                 = 28 | FLAG_OP | FLAG_OP_BIN,	// *
	OP_BIN_DIV                  = 29 | FLAG_OP | FLAG_OP_BIN,	// /
	OP_BIN_MOD                  = 30 | FLAG_OP | FLAG_OP_BIN,	// %

	OP_BIN_BITWISE_XOR          = 31 | FLAG_OP | FLAG_OP_BIN,	// ^
	OP_BIN_BITWISE_AND          = 32 | FLAG_OP | FLAG_OP_BIN,	// &
	OP_BIN_BITWISE_OR           = 33 | FLAG_OP | FLAG_OP_BIN,	// |
	OP_BIN_BITWISE_SHIFT_RIGHT  = 34 | FLAG_OP | FLAG_OP_BIN,	// >>
	OP_BIN_BITWISE_SHIFT_LEFT   = 35 | FLAG_OP | FLAG_OP_BIN,	// <<

	OP_BIN_LOGICAL_AND          = 36 | FLAG_OP | FLAG_OP_BIN,	// &&
	OP_BIN_LOGICAL_OR           = 37 | FLAG_OP | FLAG_OP_BIN,	// ||
	OP_BIN_EQUALS               = 38 | FLAG_OP | FLAG_OP_BIN,	// ==
	OP_BIN_NOT_EQUALS           = 39 | FLAG_OP | FLAG_OP_BIN,	// !=
	OP_BIN_LESS                 = 40 | FLAG_OP | FLAG_OP_BIN,	// <
	OP_BIN_LESS_EQUAL           = 41 | FLAG_OP | FLAG_OP_BIN,	// <=
	OP_BIN_GREATER              = 42 | FLAG_OP | FLAG_OP_BIN,	// >
	OP_BIN_GREATER_EQUAL        = 43 | FLAG_OP | FLAG_OP_BIN,	// >=

	OP_ASSIGN                   = 44 | FLAG_OP | FLAG_OP_ASSIGN,	// =
	OP_ADD_ASSIGN               = 45 | FLAG_OP | FLAG_OP_ASSIGN,	// +=
	OP_SUB_ASSIGN               = 46 | FLAG_OP | FLAG_OP_ASSIGN,	// -=
	OP_MULT_ASSIGN              = 47 | FLAG_OP | FLAG_OP_ASSIGN,	// *=
	OP_DIV_ASSIGN               = 48 | FLAG_OP | FLAG_OP_ASSIGN,	// /=
	OP_MOD_ASSIGN               = 49 | FLAG_OP | FLAG_OP_ASSIGN,	// %=
	OP_XOR_ASSIGN               = 50 | FLAG_OP | FLAG_OP_ASSIGN,	// ^=
	OP_AND_ASSIGN               = 51 | FLAG_OP | FLAG_OP_ASSIGN,	// &=
	OP_OR_ASSIGN                = 52 | FLAG_OP | FLAG_OP_ASSIGN,	// |=
	OP_SHIFT_RIGHT_ASSIGN       = 53 | FLAG_OP | FLAG_OP_ASSIGN,	// >>=
	OP_SHIFT_LEFT_ASSIGN        = 54 | FLAG_OP | FLAG_OP_ASSIGN,	// <<=

	OP_TERNARY_QUESTION         = 55 | FLAG_OP | FLAG_OP_TERNARY,	// ? [bool ? expr1 : expr2]
	OP_TERNARY_COLON            = 56 | FLAG_OP | FLAG_OP_TERNARY,	// : [bool ? expr1 : expr2]

	OP_UNI_INCREMENT            = 57 | FLAG_OP | FLAG_OP_UNI,	// ++
	OP_UNI_DECREMENT            = 58 | FLAG_OP | FLAG_OP_UNI,	// --
	OP_UNI_NOT                  = 59 | FLAG_OP | FLAG_OP_UNI,	// !
	OP_UNI_BITWISE_NOT          = 60 | FLAG_OP | FLAG_OP_UNI,	// ~

	OP_ADDRESS                  = 61 | FLAG_OP,	// &
	OP_DEREFERENCE              = 62 | FLAG_OP,	// *
	OP_MEMBER_ACCESS            = 63 | FLAG_OP,	// .
	// OP_PTR_MEMBER_ACCESS         ,	// -> ? Not sure if we want this or prefer implicit pointer dereference


	KW_BREAK                    = 64 | FLAG_KW,
	KW_CASE                     = 65 | FLAG_KW,
	KW_CAST                     = 66 | FLAG_KW,
	KW_CONST                    = 67 | FLAG_KW,
	KW_CONTINUE                 = 68 | FLAG_KW,
	KW_DEFAULT                  = 69 | FLAG_KW,
	KW_DELETE                   = 70 | FLAG_KW,
	KW_ELSE                     = 71 | FLAG_KW,
	KW_ENUM                     = 72 | FLAG_KW,
	KW_FALSE                    = 73 | FLAG_KW,
	KW_FOR                      = 74 | FLAG_KW,
	KW_IF                       = 75 | FLAG_KW,
	KW_NAMESPACE                = 76 | FLAG_KW,
	KW_NEW                      = 77 | FLAG_KW,
	KW_RETURN                   = 78 | FLAG_KW,
	KW_SIZEOF                   = 79 | FLAG_KW,
	KW_SOA                      = 80 | FLAG_KW,
	KW_STRUCT                   = 81 | FLAG_KW,
	KW_SWITCH                   = 82 | FLAG_KW,
	// KW_THIS                     = 83, ? Not sure yet
	KW_TRUE                     = 84 | FLAG_KW,
	KW_WHILE                    = 85 | FLAG_KW,


	// Builtin types
	BT_INT8                     = 86 | FLAG_BT,	// i8,  byte
	BT_INT16                    = 87 | FLAG_BT,	// i16, short
	BT_INT32                    = 88 | FLAG_BT,	// i32, int
	BT_INT64                    = 89 | FLAG_BT,	// i64, long

	BT_UINT8                    = 90 | FLAG_BT,	// u8
	BT_UINT16                   = 91 | FLAG_BT,	// u16
	BT_UINT32                   = 92 | FLAG_BT,	// u32
	BT_UINT64                   = 93 | FLAG_BT,	// u64

	BT_FLOAT                    = 94 | FLAG_BT,	// float
	BT_DOUBLE                   = 95 | FLAG_BT,	// double

	BT_BOOL                     = 96 | FLAG_BT,	// bool
	BT_CHAR                     = 97 | FLAG_BT,	// char
	BT_STRING                   = 98 | FLAG_BT,	// string
	BT_VOID                     = 99 | FLAG_BT,	// void
	// BT_ANY                      = 100,	// any ? Not sure I like this concept


	// Compiler (preprocessor) directives
	DIR_IMPORT                  = 101 | FLAG_DIR,	// #import
	DIR_RUN                     = 102 | FLAG_DIR,	// #run
};

typedef struct {
	TokenType type;
	std::string lexeme;
} Token;

#endif

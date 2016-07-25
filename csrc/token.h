#ifndef _TOKEN_H
#define _TOKEN_H

#include <string>

enum TokenType {
	UNKNOWN                     = 0,
	WHITESPACE                  = 1,
	COMMENT_SINGLE              = 2,	// //
	COMMENT_BLOCK_OPEN          = 3,	// /*
	COMMENT_BLOCK_CLOSE         = 4,	// */


	LITERAL_INTEGER             = 5,	// 1
	LITERAL_FLOATING_POINT      = 6,	// 1.0
	LITERAL_CHARACTER           = 7,	// 'j'
	LITERAL_STRING              = 8,	// "hello"


	PAREN_LEFT                  = 11,	// (
	PAREN_RIGHT                 = 12,	// )
	BRACKET_LEFT                = 13,	// {
	BRACKET_RIGHT               = 14,	// }
	SQUARE_BRACKET_LEFT         = 15,	// [
	SQUARE_BRACKET_RIGHT        = 16,	// ]
	ANGLE_BRACKET_LEFT          = 17,	// <
	ANGLE_BRACKET_RIGHT         = 18,	// >


	COMMA                       = 19,	// ,
	SEMI_COLON                  = 20,	// ;
	DOUBLE_COLON                = 21,	// :: - access to namespaces and possibly static stuff
	IDENTIFIER                  = 22,	// var/const/func/type name


	OP_DECLARATION              = 23,	// name: type
	OP_INFERENCE                    ,	// :=
	OP_FUNC_DECLARATION             ,	// :: - also used for structs

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

	OP_UNI_INCREMENT                ,	// ++
	OP_UNI_DECREMENT                ,	// --
	OP_UNI_NOT                      ,	// !
	OP_UNI_BITWISE_NOT              ,	// ~

	OP_ADDRESS                      ,	// &
	OP_DEREFERENCE                  ,	// *
	OP_MEMBER_ACCESS                ,	// .
	// OP_PTR_MEMBER_ACCESS            ,	// -> ? Not sure if we want this or prefer implicit pointer dereference


	KW_BREAK                        ,
	KW_CASE                         ,
	KW_CAST                         ,
	KW_CONST                        ,
	KW_CONTINUE                     ,
	KW_DEFAULT                      ,
	KW_DELETE                       ,
	KW_ELSE                         ,
	KW_ENUM                         ,
	KW_FALSE                        ,
	KW_FOR                          ,
	KW_IF                           ,
	KW_NAMESPACE                    ,
	KW_NEW                          ,
	KW_RETURN                       ,
	KW_SIZEOF                       ,
	KW_SOA                          ,
	KW_STRUCT                       ,
	KW_SWITCH                       ,
	// KW_THIS                         , ? Not sure yet
	KW_TRUE                         ,
	KW_WHILE                        ,


	// Builtin types
	BT_INT8                         ,	// i8,  byte
	BT_INT16                        ,	// i16, short
	BT_INT32                        ,	// i32, int
	BT_INT64                        ,	// i64, long

	BT_UINT8                        ,	// u8
	BT_UINT16                       ,	// u16
	BT_UINT32                       ,	// u32
	BT_UINT64                       ,	// u64

	BT_FLOAT                        ,	// float
	BT_DOUBLE                       ,	// double

	BT_BOOL                         ,	// bool
	BT_CHAR                         ,	// char
	BT_STRING                       ,	// string
	BT_VOID                         ,	// void
	// BT_ANY                          ,	// any ? Not sure I like this concept


	// Compiler (preprocessor) directives
	DIR_IMPORT                      ,	// #import
	DIR_RUN                         ,	// #run
};

typedef struct {
	TokenType type;
	std::string lexeme;
} Token;

#endif

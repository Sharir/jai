#ifndef _TOKEN_H
#define _TOKEN_H

#include <string>

enum TokenType {
	UNKNOWN                     ,
	WHITESPACE                  ,


	LITERAL_INTEGER             ,	// 1
	LITERAL_FLOATING_POINT      ,	// 1.0
	LITERAL_CHARACTER           ,	// 'j'
	LITERAL_STRING              ,	// "hello"


	PAREN_LEFT                  ,	// (
	PAREN_RIGHT                 ,	// )
	BRACKET_LEFT                ,	// {
	BRACKET_RIGHT               ,	// }
	SQUARE_BRACKET_LEFT         ,	// [
	SQUARE_BRACKET_RIGHT        ,	// ]
	ANGLE_BRACKET_LEFT          ,	// <
	ANGLE_BRACKET_RIGHT         ,	// >


	COMMA                       ,	// ,
	SEMICOLON                   ,	// ;
	DOUBLE_COLON                ,	// :: - access to named imports
	IDENTIFIER                  ,	// var/const/func/type name


	OP_DECLARATION              ,	// name: type
	OP_INFERENCE                ,	// :=
	OP_FUNC_DECLARATION         , // :: - main :: () {}
	OP_STRUCT_DECLARATION       , // :: - type :: struct {}
	OP_FUNC_RET                 ,	// ->

	OP_BIN_ADD                  ,	// +
	OP_BIN_SUB                  ,	// -
	OP_BIN_MUL                  ,	// *
	OP_BIN_DIV                  ,	// /
	OP_BIN_MOD                  ,	// %

	OP_BIN_BITWISE_XOR          ,	// ^
	OP_BIN_BITWISE_AND          ,	// &
	OP_BIN_BITWISE_OR           ,	// |
	OP_BIN_BITWISE_SHIFT_LEFT   ,	// <<
	OP_BIN_BITWISE_SHIFT_RIGHT  ,	// >>

	OP_BIN_LOGICAL_AND          ,	// &&
	OP_BIN_LOGICAL_OR           ,	// ||
	OP_BIN_EQUALS               ,	// ==
	OP_BIN_NOT_EQUALS           ,	// !=
	OP_BIN_LESS                 ,	// <
	OP_BIN_LESS_EQUAL           ,	// <=
	OP_BIN_GREATER              ,	// >
	OP_BIN_GREATER_EQUAL        ,	// >=

	OP_ASSIGN                   ,	// =
	OP_ADD_ASSIGN               ,	// +=
	OP_SUB_ASSIGN               ,	// -=
	OP_MUL_ASSIGN               ,	// *=
	OP_DIV_ASSIGN               ,	// /=
	OP_MOD_ASSIGN               ,	// %=
	OP_XOR_ASSIGN               ,	// ^=
	OP_AND_ASSIGN               ,	// &=
	OP_OR_ASSIGN                ,	// |=
	OP_SHIFT_LEFT_ASSIGN        ,	// <<=
	OP_SHIFT_RIGHT_ASSIGN       ,	// >>=

	OP_TERNARY_QUESTION         ,	// ? [bool ? expr1 : expr2]
	OP_TERNARY_COLON            ,	// : [bool ? expr1 : expr2]

	OP_UNI_INCREMENT            ,	// ++
	OP_UNI_DECREMENT            ,	// --
	OP_UNI_NOT                  ,	// !
	OP_UNI_BITWISE_NOT          ,	// ~

	OP_ADDRESS                  ,	// &
	OP_PTR                      ,	// *
	OP_MEMBER_ACCESS            ,	// .


	KW_BREAK                    ,
	KW_CASE                     ,
	KW_CAST                     ,
	KW_CONST                    ,
	KW_CONTINUE                 ,
	KW_DEFAULT                  ,
	KW_DELETE                   ,
	KW_ELSE                     ,
	KW_ENUM                     ,	// var
	KW_FALSE                    ,
	KW_FOR                      ,
	KW_IF                       ,
	KW_NEW                      ,
	KW_NULL                     ,
	KW_OLD                      ,
	KW_OPAQUE                   ,	// var
	KW_RETURN                   ,
	KW_SIZEOF                   ,
	KW_SOA                      ,	// var
	KW_STRUCT                   ,	// var
	KW_SWITCH                   ,
	KW_TRUE                     ,
	KW_WHILE                    ,


	// Builtin types
	BT_INT8                     ,	// i8,  byte
	BT_INT16                    ,	// i16, short
	BT_INT32                    ,	// i32, int
	BT_INT64                    ,	// i64, long

	BT_UINT8                    ,	// u8
	BT_UINT16                   ,	// u16
	BT_UINT32                   ,	// u32
	BT_UINT64                   ,	// u64

	BT_FLOAT                    ,	// float
	BT_DOUBLE                   ,	// double

	BT_BOOL                     ,	// bool
	BT_CHAR                     ,	// char
	BT_STRING                   ,	// string
	BT_VOID                     ,	// void


	// Compiler (preprocessor) directives
	DIR_EXPORT                  ,	// #export
	DIR_IMPORT                  ,	// #import
	DIR_RUN                     ,	// #run


	// Meta types to aid the parser
	ROOT,
	LIST,
};

typedef struct {
	TokenType type;
	std::string lexeme;
	int line;
	int col;
} Token;

#endif

#include "parser.h"

int most_significant_bit_set(int num)
{
	int n = 1 << 30;

	while((num & n) != 0)
		n = n >> 1;
	
	return n;
}

int parseTokens(const vector<Token>& tokens, TreeNode* tree)
{
	int msb = 0;
	int type = 0;
	int size = tokens.size();

	for (unsigned int i = 0 ; i < size ; i++)
	{
		type = tokens[i].type;
		msb = most_significant_bit_set(type);
		switch (msb)
		{
		case TokenFlag::FLAG_OP:
			if (IS_FLAG_SET(type, FLAG_OP_UNI)) //Unirary operator
			{
				switch (type)
				{
				case OP_UNI_INCREMENT:
					break;
				case OP_UNI_DECREMENT:
					break;
				case OP_UNI_NOT:
					break;
				case OP_UNI_BITWISE_NOT:
					break;
				}
			}
			else if (IS_FLAG_SET(type, FLAG_OP_BIN)) //Binary operator
			{
				switch (type)
				{
				//Simple arithmetic binary operators
				case OP_BIN_ADD:
					break;
				case OP_BIN_SUB:
					break;
				case OP_BIN_MULT:
					break;
				case OP_BIN_DIV:
					break;
				case OP_BIN_MOD:
					break;
			
				//Bitwise binary operators
				case OP_BIN_BITWISE_XOR:
					break;
				case OP_BIN_BITWISE_AND:
					break;
				case OP_BIN_BITWISE_OR:
					break;
				case OP_BIN_BITWISE_SHIFT_RIGHT:
					break;
				case OP_BIN_BITWISE_SHIFT_LEFT:
					break;

				//Logical binary operators
				case OP_BIN_LOGICAL_AND:
					break;
				case OP_BIN_LOGICAL_OR:
					break;
			
				//Comparison binary operators
				case OP_BIN_EQUALS:
					break;
				case OP_BIN_NOT_EQUALS:
					break;
				case OP_BIN_LESS:
					break;
				case OP_BIN_LESS_EQUAL:
					break;
				case OP_BIN_GREATER:
					break;
				case OP_BIN_GREATER_EQUAL:
					break;
				}
			}
			else if (IS_FLAG_SET(type, FLAG_OP_TERNARY))
			{
				switch (type)
				{
				case OP_TERNARY_QUESTION:
					break;
				case OP_TERNARY_COLON:
					break;
				}
			}
			else
			{
				switch (type)
				{
				case OP_DECLARATION:
					break;
				case OP_INFERENCE:
					break;
				case OP_FUNC_DECLARATION:
					break;
				case OP_ADDRESS:
					break;
				case OP_DEREFERENCE:
					break;
				case OP_MEMBER_ACCESS:
					break;
				}
			}
			break;

		case TokenFlag::FLAG_KW:
			switch (type)
			{
			case KW_BREAK:
				break;
			case KW_CASE:
				break;
			case KW_CAST:
				break;
			case KW_CONST:
				break;
			case KW_CONTINUE:
				break;
			case KW_DEFAULT:
				break;
			case KW_DELETE:
				break;
			case KW_ELSE:
				break;
			case KW_ENUM:
				break;
			case KW_FALSE:
				break;
			case KW_FOR:
				break;
			case KW_IF:
				break;
			case KW_NAMESPACE:
				break;
			case KW_NEW:
				break;
			case KW_RETURN:
				break;
			case KW_SIZEOF:
				break;
			case KW_SOA:
				break;
			case KW_STRUCT:
				break;
			case KW_SWITCH:
				break;
			case KW_TRUE:
				break;
			case KW_WHILE:
				break;
			}
			break;

		case TokenFlag::FLAG_BT:
			switch (type)
			{
			case BT_INT8:
				break;
			case BT_INT16:
				break;
			case BT_INT32:
				break;
			case BT_INT64:
				break;

			case BT_UINT8:
				break;
			case BT_UINT16:
				break;
			case BT_UINT32:
				break;
			case BT_UINT64:
				break;

			case BT_FLOAT:
				break;
			case BT_DOUBLE:
				break;
				
			case BT_BOOL:
				break;
			case BT_CHAR:
				break;
			case BT_STRING:
				break;
			case BT_VOID:
				break;
			}
			break;

		case TokenFlag::FLAG_DIR: //Will move to Preprocessor
			break;

		case TokenFlag::FLAG_BRACKETS:
			switch (type)
			{
			case PAREN_LEFT:
				break;
			case PAREN_RIGHT:
				break;

			case BRACKET_LEFT:
				break;
			case BRACKET_RIGHT:
				break;

			case SQUARE_BRACKET_LEFT:
				break;
			case SQUARE_BRACKET_RIGHT:
				break;
				
			case ANGLE_BRACKET_LEFT:
				break;
			case ANGLE_BRACKET_RIGHT:
				break;
			}
			break;

		case TokenFlag::FLAG_LITERAL:
			switch (type)
			{
			case LITERAL_INTEGER:
				break;
			case LITERAL_FLOATING_POINT:
				break;
			case LITERAL_CHARACTER:
				break;
			case LITERAL_STRING:
				break;
			}
			break;

		default:
			switch (type)
			{
			case WHITESPACE:
				break;
			case COMMA:
				break;
			case SEMI_COLON:
				break;
			case DOUBLE_COLON:
				break;
			case IDENTIFIER:
				break;
			default: //Basically, case UNKNOWN, nothing to code here as in lexer.cpp the program already claims syntax error.
				break;
			}
			break;

		}
	}

	return 0; //for now
}

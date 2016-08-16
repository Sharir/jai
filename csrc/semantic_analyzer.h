#ifndef _SEMANTIC_ANALYZER_H
#define _SEMANTIC_ANALYZER_H

#include "parser.h"
#include <deque>
#include <map>

enum SymbolType {
	VARIABLE,
	FUNCTION,
	STRUCT
};

typedef struct {
	std::string id; //name of identifier
	SymbolType symbol_type; //what symbol is it?
	std::string type; //what is the type of symbol (return type of function or variable type)
	std::map<std::string, std::string>* properties; //properties in case of a struct or arguments in case of a function. NULL in variable
}Symbol;

Symbol makeSymbol(std::string id, SymbolType st, std::string t, std::map<std::string, std::string>* prop) {
	Symbol s = {id, st, t, prop};
	return s;
}

typedef struct {
	std::deque<Symbol*> deq;

	void enter_scope() { //enters a new scope
		deq.emplace_front(NULL);
	}

	const Symbol* find_symbol(std::string x) {
		for (Symbol* sp : deq) {
			if (sp)
				if (sp->id == x)
					return sp;
		}

		return NULL;
	}

	void add_symbol(Symbol* x) {
		deq.emplace_front(x);
	}

	bool check_scope(std::string x) {
		for (Symbol* sp : deq) {
			if (sp->id == x)
				return true;
		}

		return false;
	}

	void exit_scope()
	{
		while (deq.front() != NULL)
			deq.pop_front();
	}
}SymbolTable;

void analyzeTreeSemantics(TokenNode* ast, const SymbolTable& st);

#endif

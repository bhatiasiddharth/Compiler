#ifndef _PARSER_H
#define _PARSER_H
#include "lexer.h"

enum non_terminals {Program = 100, Functions, FunctionDef, fnReturn, Statements, moreStmts, Stmt, ReturnStmt, BreakStmt, DeclarationStmt, moreDeclarations, mutMod, Declaration, moreTypes, AssignStmtType2, listTypes, typeList, moreList, singleAssn, multAssn, moreAssn, IDStmts, IDStmts2, Index, moreIndex, AssignStmtType1, FunCall, MethodCall, FunCallStmt, MethodStmt, Type, parameterList, remainingList, IfStmt, ElseStmt, IStmt, OStmt, value, array, IDList, moreIds, arithExpn, moreTerms, arithTerm, moreFactors, factor, opLow, relType, opHigh, boolExpn, logicalOp, relationalOp, LoopStmt, grid, rows, moreRows, row, moreNums, boolean};

#define NONTERM_COUNT 59
#define TERM_COUNT 52
#define RULE_COUNT 116
#define RULE_MAX_SYMBOLS 20

extern int parse_table[NONTERM_COUNT][TERM_COUNT];
extern const char* nonterm_names[];

static inline void print_symbol(FILE* fp, int symbol) {
	if(symbol >= 100)
		fprintf(fp, "%s ", nonterm_names[symbol - 100]);
	else
		fprintf(fp, "%s ", token_names[symbol]);
}

void set_parse_table_cell(int terminal, int token, int rule);
#endif
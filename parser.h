#ifndef _PARSER_H
#define _PARSER_H
#include "lexer.h"
#include "tree.h"

enum non_terminals {Program = 100, Functions, FunctionDef, fnReturn, Statements, moreStmts, Stmt, ReturnStmt, BreakStmt, DeclarationStmt, moreDeclarations, mutMod, Declaration, moreTypes, AssignStmtType2, listTypes, typeList, moreList, singleAssn, multAssn, moreAssn, IDStmts, IDStmts2, Index, moreIndex, AssignStmtType1, FunCall, MethodCall, FunCallStmt, MethodStmt, Type, parameterList, remainingList, IfStmt, ElseStmt, IStmt, OStmt, value, array, IDList, moreIds, arithExpn, moreTerms, arithTerm, moreFactors, factor, opLow, relType, opHigh, boolExpn, logicalOp, relationalOp, LoopStmt, grid, rows, moreRows, row, moreNums, boolean};

extern int parse_table[NONTERM_COUNT][TERM_COUNT];
extern const char* nonterm_names[];

static inline void print_symbol(FILE* fp, int symbol) {
	if(symbol >= 100)
		fprintf(fp, "%s ", nonterm_names[symbol - 100]);
	else
		fprintf(fp, "%s ", token_names[symbol]);
}

extern void init_parse_table();
extern struct tree_node* create_parsetree();
#endif
#ifndef _PARSER_H
#define _PARSER_H
#include "lexer.h"
#include "tree.h"

enum non_terminals {Program = 100, Functions, FunctionDef, fnReturn, Statements, moreStmts, Stmt, ReturnStmt, BreakStmt, DeclarationStmt, moreDeclarations, mutMod, Declaration, moreTypes, AssignStmtType2, listTypes, typeList, moreList, singleAssn, multAssn, moreAssn, IDStmts, IDStmts2, Index, moreIndex, AssignStmtType1, FunCall, MethodCall, FunCallStmt, MethodStmt, Type, parameterList, remainingList, IfStmt, ElseStmt, IStmt, OStmt, value, array, IDList, moreIds, arithExpn, moreTerms, arithTerm, moreFactors, factor, opLow, relType, opHigh, boolExpn, logicalOp, relationalOp, LoopStmt, grid, rows, moreRows, row, moreNums, boolean};

extern int parse_table[NONTERM_COUNT][TERM_COUNT];
extern const char* nonterm_names[];

extern void init_parse_table();
extern struct tree_node* create_parsetree(const char* src_file, const char* tokens_file);
extern void print_symbol(FILE* fp, int symbol, union value value);

#endif
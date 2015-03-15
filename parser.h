#ifndef _PARSER_H
#define _PARSER_H
#include "lexer.h"

enum non_terminals {Program = 100, Functions, FunctionDef, fnReturn, Statements, moreStmts, Stmt, ReturnStmt, BreakStmt, DeclarationStmt, moreDeclarations, mutMod, Declaration, moreTypes, AssignStmtType2, listTypes, typeList, moreList, singleAssn, multAssn, moreAssn, IDStmts, IDStmts2, Index, moreIndex, AssignStmtType1, FunCall, MethodCall, FunCallStmt, MethodStmt, Type, parameterList, remainingList, IfStmt, ElseStmt, IStmt, OStmt, value, array, IDList, moreIds, arithExpn, moreTerms, arithTerm, moreFactors, factor, opLow, relType, opHigh, boolExpn, logicalOp, relationalOp, LoopStmt, grid, rows, moreRows, row, moreNums, boolean, dollar,EPSILON};



#define NONTERM_COUNT 200
#define TERM_COUNT 100
#define STOP 210


#endif
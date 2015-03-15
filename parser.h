enum non_terminals {Program = 100, Functions, FunctionDef, fnReturn, Statements, moreStmts, Stmt, ReturnStmt, BreakStmt, DeclarationStmt, moreDeclarations, mutMod, Declaration, moreTypes, AssignStmtType2, listTypes, typeList, moreList, singleAssn, multAssn, moreAssn, IDStmts, IDStmts2, Index, moreIndex, AssignStmtType1, FunCall, MethodCall, FunCallStmt, MethodStmt, Type, parameterList, remainingList, IfStmt, ElseStmt, IStmt, OStmt, value, array, IDList, moreIds, arithExpn, moreTerms, arithTerm, moreFactors, factor, opLow, relType, opHigh, boolExpn, logicalOp, relationalOp, LoopStmt, grid, rows, moreRows, row, moreNums, boolean};

#define NONTERM_COUNT 70
#define TERM_COUNT 50
#define EPSILON 200
#define STOP 210

int rules[][] = {
	{ Functions, MAIN, OPAREN, CPAREN, OBRACE, Statements, CBRACE, STOP },
	{ FunctionDef, Functions, STOP },
	{ EPSILON, STOP },
	{ FN, ID, OPAREN, parameterList, CPAREN, fnReturn, OBRACE, Statements, CBRACE, STOP },
	{ Type, STOP },
	{ EPSILON, STOP },
	{ Stmt , moreStmts, STOP },
	{ Stmt , moreStmts, STOP },
	{ EPSILON, STOP },
	{ COMMENT, STOP },
	{ ID , IDStmts, STOP },
	{ OStmt, STOP },
	{ LoopStmt, STOP },
	{ AssignStmtType2, STOP },
	{ DeclarationStmt, STOP },
	{ IStmt, STOP },
	{ IfStmt, STOP },
	{ ReturnStmt, STOP },
	{ BreakStmt, STOP },
	{ RETURN, relType, SEMICOLON, STOP },
	{ Break, SEMICOLON, STOP },
	{ LET, mutMod, moreDeclarations, SEMICOLON, STOP },
	{ AssignStmtType2, STOP },
	{ Declaration, STOP },
	{ MUT, STOP },
	{ EPSILON, STOP },
	{ ID, ASSIGNOP, arithExpn, moreTypes, STOP },
	{ COMMA, Declaration, STOP },
	{ EPSILON, STOP },
	{ listTypes, ASSIGNOP, OPAREN, multAssn, CPAREN, SEMICOLON, STOP },
	{ OPAREN, typeList, CPAREN, STOP },
	{ ID, moreList, STOP },
	{ COMMA, typeList, STOP },
	{ EPSILON, STOP },
	{ arithExpn, STOP },
	{ singleAssn, moreAssn, STOP },
	{ COMMA, multAssn, STOP },
	{ COLON, multAssn, STOP },
	{ EPSILON, STOP },
	{ OSQUARE, ID, CSQUARE, OSQUARE, ID, CSQUARE, AssignStmtType1, STOP },
	{ MethodStmt, STOP },
	{ AssignStmtType1, STOP },
	{ FunCallStmt, STOP },
	{ Index, STOP },
	{ MethodCall, STOP },
	{ FunCall, STOP },
	{ EPSILON, STOP },
	{ OSQUARE, ID, CSQUARE, moreIndex, STOP },
	{ ID, STOP },
	{ NUM, STOP },
	{ OSQUARE, ID, CSQUARE, STOP },
	{ EPSILON, STOP },
	{ ASSIGNOP, singleAssn, SEMICOLON, STOP },
	{ OPAREN, IDList ,CPAREN, STOP },
	{ DOT, ID, FunCall, STOP },
	{ FunCall, SEMICOLON, STOP },
	{ MethodCall, SEMICOLON, STOP },
	{ BOOL, STOP },
	{ F32, STOP },
	{ I32, STOP },
	{ string, STOP },
	{ ID, COLON, Type, remainingList, STOP },
	{ COMMA, parameterList, STOP },
	{ EPSILON, STOP },
	{ IF, OPAREN, boolExpn, CPAREN, OBRACE, Statements, CBRACE, ElseStmt, STOP },
	{ ELSEIF, OPAREN, boolExpn, CPAREN, OBRACE, Statements, CBRACE, ElseStmt, STOP },
	{ ELSE, OBRACE , Statements, CBRACE, STOP },
	{ EPSILON, STOP },
	{ SCAN, OPAREN, ID, CPAREN, SEMICOLON, STOP },
	{ PRINT, OPAREN, ID, CPAREN, SEMICOLON, STOP },
	{ CHARL, STOP },
	{ NUM, STOP },
	{ STRINGL, STOP },
	{ grid, STOP },
	{ array, STOP },
	{ boolean, STOP },
	{ OSQUARE, multAssn, CSQUARE, STOP },
	{ ID, moreIds, STOP },
	{ EPSILON, STOP },
	{ COMMA, IDList, STOP },
	{ EPSILON, STOP },
	{ arithTerm , moreTerms, STOP },
	{ opLow , arithExpn, STOP },
	{ EPSILON, STOP },
	{ factor , moreFactors, STOP },
	{ opHigh , arithTerm, STOP },
	{ EPSILON, STOP },
	{ OPAREN , arithExpn, CPAREN, STOP },
	{ relType, STOP },
	{ MINUS, STOP },
	{ PLUS, STOP },
	{ ID, IDStmts2, STOP },
	{ value, STOP },
	{ DIV, STOP },
	{ MUL, STOP },
	{ OPAREN, boolExpn, CPAREN, logicalOp, OPAREN, boolExpn, CPAREN, STOP },
	{ relType, relationalOp, relType, STOP },
	{ AND, STOP },
	{ NOT, STOP },
	{ OR, STOP },
	{ EQ, STOP },
	{ GT, STOP },
	{ GTE, STOP },
	{ LT, STOP },
	{ LTE, STOP },
	{ NE, STOP },
	{ WHILE, OPAREN, boolExpn, CPAREN, OBRACE, Statements, CBRACE, STOP },
	{ OBRACE, rows, CBRACE, STOP },
	{ row , moreRows, STOP },
	{ COLON, rows, STOP },
	{ EPSILON, STOP },
	{ NUM, moreNums, STOP },
	{ COMMA, NUM, moreNums, STOP },
	{ EPSILON, STOP },
	{ FALSE, STOP },
	{ TRUE, STOP }
};

int parse_table[NONTERM_COUNT][TERM_COUNT];

parse_table[Program][FN] = 0;
parse_table[Program][MAIN] = 0;
parse_table[Functions][FN] = 1;
parse_table[Functions][e] = 2;
parse_table[Functions][MAIN] = 2;
parse_table[FunctionDef][FN] = 3;
parse_table[fnReturn][BOOL] = 4;
parse_table[fnReturn][string] = 4;
parse_table[fnReturn][F32] = 4;
parse_table[fnReturn][I32] = 4;
parse_table[fnReturn][e] = 5;
parse_table[fnReturn][OBRACE] = 5;
parse_table[Statements][LET] = 6;
parse_table[Statements][COMMENT] = 6;
parse_table[Statements][ID] = 6;
parse_table[Statements][IF] = 6;
parse_table[Statements][OPAREN] = 6;
parse_table[Statements][WHILE] = 6;
parse_table[Statements][RETURN] = 6;
parse_table[Statements][Break] = 6;
parse_table[Statements][SCAN] = 6;
parse_table[Statements][PRINT] = 6;
parse_table[moreStmts][LET] = 7;
parse_table[moreStmts][COMMENT] = 7;
parse_table[moreStmts][ID] = 7;
parse_table[moreStmts][IF] = 7;
parse_table[moreStmts][OPAREN] = 7;
parse_table[moreStmts][WHILE] = 7;
parse_table[moreStmts][RETURN] = 7;
parse_table[moreStmts][Break] = 7;
parse_table[moreStmts][SCAN] = 7;
parse_table[moreStmts][PRINT] = 7;
parse_table[moreStmts][e] = 8;
parse_table[moreStmts][CBRACE] = 8;
parse_table[Stmt][COMMENT] = 9;
parse_table[Stmt][ID] = 10;
parse_table[Stmt][PRINT] = 11;
parse_table[Stmt][WHILE] = 12;
parse_table[Stmt][OPAREN] = 13;
parse_table[Stmt][LET] = 14;
parse_table[Stmt][SCAN] = 15;
parse_table[Stmt][IF] = 16;
parse_table[Stmt][RETURN] = 17;
parse_table[Stmt][Break] = 18;
parse_table[ReturnStmt][RETURN] = 19;
parse_table[BreakStmt][Break] = 20;
parse_table[DeclarationStmt][LET] = 21;
parse_table[moreDeclarations][OPAREN] = 22;
parse_table[moreDeclarations][ID] = 23;
parse_table[mutMod][MUT] = 24;
parse_table[mutMod][e] = 25;
parse_table[mutMod][ID] = 25;
parse_table[mutMod][OPAREN] = 25;
parse_table[Declaration][ID] = 26;
parse_table[moreTypes][COMMA] = 27;
parse_table[moreTypes][e] = 28;
parse_table[moreTypes][SEMICOLON] = 28;
parse_table[AssignStmtType2][OPAREN] = 29;
parse_table[listTypes][OPAREN] = 30;
parse_table[typeList][ID] = 31;
parse_table[moreList][COMMA] = 32;
parse_table[moreList][e] = 33;
parse_table[moreList][CPAREN] = 33;
parse_table[singleAssn][TRUE] = 34;
parse_table[singleAssn][OBRACE] = 34;
parse_table[singleAssn][NUM] = 34;
parse_table[singleAssn][ID] = 34;
parse_table[singleAssn][OSQUARE] = 34;
parse_table[singleAssn][STRINGL] = 34;
parse_table[singleAssn][OPAREN] = 34;
parse_table[singleAssn][CHARL] = 34;
parse_table[singleAssn][FALSE] = 34;
parse_table[multAssn][TRUE] = 35;
parse_table[multAssn][OBRACE] = 35;
parse_table[multAssn][NUM] = 35;
parse_table[multAssn][ID] = 35;
parse_table[multAssn][OSQUARE] = 35;
parse_table[multAssn][STRINGL] = 35;
parse_table[multAssn][OPAREN] = 35;
parse_table[multAssn][CHARL] = 35;
parse_table[multAssn][FALSE] = 35;
parse_table[moreAssn][COMMA] = 36;
parse_table[moreAssn][COLON] = 37;
parse_table[moreAssn][e] = 38;
parse_table[moreAssn][CSQUARE] = 38;
parse_table[moreAssn][CPAREN] = 38;
parse_table[IDStmts][OSQUARE] = 39;
parse_table[IDStmts][DOT] = 40;
parse_table[IDStmts][ASSIGNOP] = 41;
parse_table[IDStmts][OPAREN] = 42;
parse_table[IDStmts2][NUM] = 43;
parse_table[IDStmts2][ID] = 43;
parse_table[IDStmts2][OSQUARE] = 43;
parse_table[IDStmts2][DOT] = 44;
parse_table[IDStmts2][OPAREN] = 45;
parse_table[IDStmts2][e] = 46;
parse_table[IDStmts2][PLUS] = 46;
parse_table[IDStmts2][GTE] = 46;
parse_table[IDStmts2][DIV] = 46;
parse_table[IDStmts2][LTE] = 46;
parse_table[IDStmts2][CSQUARE] = 46;
parse_table[IDStmts2][MUL] = 46;
parse_table[IDStmts2][CPAREN] = 46;
parse_table[IDStmts2][NE] = 46;
parse_table[IDStmts2][EQ] = 46;
parse_table[IDStmts2][GT] = 46;
parse_table[IDStmts2][LT] = 46;
parse_table[IDStmts2][COMMA] = 46;
parse_table[IDStmts2][COLON] = 46;
parse_table[IDStmts2][SEMICOLON] = 46;
parse_table[IDStmts2][MINUS] = 46;
parse_table[Index][OSQUARE] = 47;
parse_table[Index][ID] = 48;
parse_table[Index][NUM] = 49;
parse_table[moreIndex][OSQUARE] = 50;
parse_table[moreIndex][e] = 51;
parse_table[moreIndex][PLUS] = 51;
parse_table[moreIndex][GTE] = 51;
parse_table[moreIndex][DIV] = 51;
parse_table[moreIndex][LTE] = 51;
parse_table[moreIndex][CSQUARE] = 51;
parse_table[moreIndex][MUL] = 51;
parse_table[moreIndex][CPAREN] = 51;
parse_table[moreIndex][NE] = 51;
parse_table[moreIndex][EQ] = 51;
parse_table[moreIndex][GT] = 51;
parse_table[moreIndex][LT] = 51;
parse_table[moreIndex][COMMA] = 51;
parse_table[moreIndex][COLON] = 51;
parse_table[moreIndex][SEMICOLON] = 51;
parse_table[moreIndex][MINUS] = 51;
parse_table[AssignStmtType1][ASSIGNOP] = 52;
parse_table[FunCall][OPAREN] = 53;
parse_table[MethodCall][DOT] = 54;
parse_table[FunCallStmt][OPAREN] = 55;
parse_table[MethodStmt][DOT] = 56;
parse_table[Type][BOOL] = 57;
parse_table[Type][F32] = 58;
parse_table[Type][I32] = 59;
parse_table[Type][string] = 60;
parse_table[parameterList][ID] = 61;
parse_table[remainingList][COMMA] = 62;
parse_table[remainingList][e] = 63;
parse_table[remainingList][CPAREN] = 63;
parse_table[IfStmt][IF] = 64;
parse_table[ElseStmt][ELSEIF] = 65;
parse_table[ElseStmt][ELSE] = 66;
parse_table[ElseStmt][e] = 67;
parse_table[ElseStmt][CBRACE] = 67;
parse_table[ElseStmt][LET] = 67;
parse_table[ElseStmt][COMMENT] = 67;
parse_table[ElseStmt][ID] = 67;
parse_table[ElseStmt][IF] = 67;
parse_table[ElseStmt][OPAREN] = 67;
parse_table[ElseStmt][WHILE] = 67;
parse_table[ElseStmt][RETURN] = 67;
parse_table[ElseStmt][Break] = 67;
parse_table[ElseStmt][SCAN] = 67;
parse_table[ElseStmt][PRINT] = 67;
parse_table[IStmt][SCAN] = 68;
parse_table[OStmt][PRINT] = 69;
parse_table[value][CHARL] = 70;
parse_table[value][NUM] = 71;
parse_table[value][STRINGL] = 72;
parse_table[value][OBRACE] = 73;
parse_table[value][OSQUARE] = 74;
parse_table[value][TRUE] = 75;
parse_table[value][FALSE] = 75;
parse_table[array][OSQUARE] = 76;
parse_table[IDList][ID] = 77;
parse_table[IDList][e] = 78;
parse_table[IDList][CPAREN] = 78;
parse_table[moreIds][COMMA] = 79;
parse_table[moreIds][e] = 80;
parse_table[moreIds][CPAREN] = 80;
parse_table[arithExpn][TRUE] = 81;
parse_table[arithExpn][OBRACE] = 81;
parse_table[arithExpn][NUM] = 81;
parse_table[arithExpn][ID] = 81;
parse_table[arithExpn][OSQUARE] = 81;
parse_table[arithExpn][STRINGL] = 81;
parse_table[arithExpn][OPAREN] = 81;
parse_table[arithExpn][CHARL] = 81;
parse_table[arithExpn][FALSE] = 81;
parse_table[moreTerms][PLUS] = 82;
parse_table[moreTerms][MINUS] = 82;
parse_table[moreTerms][e] = 83;
parse_table[moreTerms][CSQUARE] = 83;
parse_table[moreTerms][CPAREN] = 83;
parse_table[moreTerms][COMMA] = 83;
parse_table[moreTerms][COLON] = 83;
parse_table[moreTerms][SEMICOLON] = 83;
parse_table[arithTerm][TRUE] = 84;
parse_table[arithTerm][OBRACE] = 84;
parse_table[arithTerm][NUM] = 84;
parse_table[arithTerm][ID] = 84;
parse_table[arithTerm][OSQUARE] = 84;
parse_table[arithTerm][STRINGL] = 84;
parse_table[arithTerm][OPAREN] = 84;
parse_table[arithTerm][CHARL] = 84;
parse_table[arithTerm][FALSE] = 84;
parse_table[moreFactors][DIV] = 85;
parse_table[moreFactors][MUL] = 85;
parse_table[moreFactors][e] = 86;
parse_table[moreFactors][PLUS] = 86;
parse_table[moreFactors][CSQUARE] = 86;
parse_table[moreFactors][CPAREN] = 86;
parse_table[moreFactors][COMMA] = 86;
parse_table[moreFactors][COLON] = 86;
parse_table[moreFactors][SEMICOLON] = 86;
parse_table[moreFactors][MINUS] = 86;
parse_table[factor][OPAREN] = 87;
parse_table[factor][TRUE] = 88;
parse_table[factor][OBRACE] = 88;
parse_table[factor][NUM] = 88;
parse_table[factor][ID] = 88;
parse_table[factor][OSQUARE] = 88;
parse_table[factor][STRINGL] = 88;
parse_table[factor][CHARL] = 88;
parse_table[factor][FALSE] = 88;
parse_table[opLow][MINUS] = 89;
parse_table[opLow][PLUS] = 90;
parse_table[relType][ID] = 91;
parse_table[relType][TRUE] = 92;
parse_table[relType][OBRACE] = 92;
parse_table[relType][NUM] = 92;
parse_table[relType][OSQUARE] = 92;
parse_table[relType][STRINGL] = 92;
parse_table[relType][CHARL] = 92;
parse_table[relType][FALSE] = 92;
parse_table[opHigh][DIV] = 93;
parse_table[opHigh][MUL] = 94;
parse_table[boolExpn][OPAREN] = 95;
parse_table[boolExpn][TRUE] = 96;
parse_table[boolExpn][OBRACE] = 96;
parse_table[boolExpn][NUM] = 96;
parse_table[boolExpn][ID] = 96;
parse_table[boolExpn][OSQUARE] = 96;
parse_table[boolExpn][STRINGL] = 96;
parse_table[boolExpn][CHARL] = 96;
parse_table[boolExpn][FALSE] = 96;
parse_table[logicalOp][AND] = 97;
parse_table[logicalOp][NOT] = 98;
parse_table[logicalOp][OR] = 99;
parse_table[relationalOp][EQ] = 100;
parse_table[relationalOp][GT] = 101;
parse_table[relationalOp][GTE] = 102;
parse_table[relationalOp][LT] = 103;
parse_table[relationalOp][LTE] = 104;
parse_table[relationalOp][NE] = 105;
parse_table[LoopStmt][WHILE] = 106;
parse_table[grid][OBRACE] = 107;
parse_table[rows][NUM] = 108;
parse_table[moreRows][COLON] = 109;
parse_table[moreRows][e] = 110;
parse_table[moreRows][CBRACE] = 110;
parse_table[row][NUM] = 111;
parse_table[moreNums][COMMA] = 112;
parse_table[moreNums][e] = 113;
parse_table[moreNums][CBRACE] = 113;
parse_table[moreNums][COLON] = 113;
parse_table[boolean][FALSE] = 114;
parse_table[boolean][TRUE] = 115;

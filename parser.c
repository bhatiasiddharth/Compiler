#include "parser.h"
#include "stack.h"


int parse_table[NONTERM_COUNT][TERM_COUNT];

const char* nonterm_names[] = {"Program", "Functions", "FunctionDef", "fnReturn", "Statements", "moreStmts", "Stmt", "ReturnStmt", "BreakStmt", "DeclarationStmt", "moreDeclarations", "mutMod", "Declaration", "moreTypes", "AssignStmtType2", "listTypes", "typeList", "moreList", "singleAssn", "multAssn", "moreAssn", "IDStmts", "IDStmts2", "Index", "moreIndex", "AssignStmtType1", "FunCall", "MethodCall", "FunCallStmt", "MethodStmt", "Type", "parameterList", "remainingList", "IfStmt", "ElseStmt", "IStmt", "OStmt", "value", "array", "IDList", "moreIds", "arithExpn", "moreTerms", "arithTerm", "moreFactors", "factor", "opLow", "relType", "opHigh", "boolExpn", "logicalOp", "relationalOp", "LoopStmt", "grid", "rows", "moreRows", "row", "moreNums", "boolean", "DOLLAR","EPSILON"};


int rules[RULE_COUNT][RULE_MAX_SYMBOLS] = {
    { Functions, MAIN, OPAREN, CPAREN, OBRACE, Statements, CBRACE, STOP },
    { FunctionDef, Functions, STOP },
    { EPSILON, STOP },
    { FN, ID, OPAREN, parameterList, CPAREN, fnReturn, OBRACE, Statements, CBRACE, STOP },
    { RARROW, Type, STOP },
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
    { BREAK, SEMICOLON, STOP },
    { LET, mutMod, moreDeclarations, STOP },
    { AssignStmtType2, STOP },
    { Declaration, STOP },
    { MUT, STOP },
    { EPSILON, STOP },
    { ID, ASSIGNOP, arithExpn, moreTypes, SEMICOLON, STOP },
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
    { OSQUARE, relType, CSQUARE, moreIndex, STOP },
    { relType, STOP },
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
    { STRING, STOP },
    { ID, COLON, Type, remainingList, STOP },
    { COMMA, parameterList, STOP },
    { EPSILON, STOP },
    { IF, OPAREN, boolExpn, CPAREN, OBRACE, Statements, CBRACE, ElseStmt, STOP },
    { ELSEIF, OPAREN, boolExpn, CPAREN, OBRACE, Statements, CBRACE, ElseStmt, STOP },
    { ELSE, OBRACE , Statements, CBRACE, STOP },
    { EPSILON, STOP },
    { SCAN, OPAREN, ID, CPAREN, SEMICOLON, STOP },
    { PRINT, OPAREN, relType, CPAREN, SEMICOLON, STOP },
    { CHARL, STOP },
    { NUM, STOP },
    { STRL, STOP },
    { grid, STOP },
    { array, STOP },
    { boolean, STOP },
    { OSQUARE, multAssn, CSQUARE, STOP },
    { relType, moreIds, STOP },
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
    { GE, STOP },
    { LT, STOP },
    { LE, STOP },
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

void init_parse_table(){
    // initialize undefined positions with -1
    for(int i = 0; i < NONTERM_COUNT; i++)
        for(int j = 0; j < TERM_COUNT; j++)
            parse_table[i][j] = -1;

    parse_table[Program][FN] = 0;
    parse_table[Program][MAIN] = 0;
    parse_table[Functions][FN] = 1;
    parse_table[Functions][EPSILON] = 2;
    parse_table[Functions][MAIN] = 2;
    parse_table[FunctionDef][FN] = 3;
    parse_table[fnReturn][RARROW] = 4;
    parse_table[fnReturn][EPSILON] = 5;
    parse_table[fnReturn][OBRACE] = 5;
    parse_table[Statements][LET] = 6;
    parse_table[Statements][COMMENT] = 6;
    parse_table[Statements][ID] = 6;
    parse_table[Statements][IF] = 6;
    parse_table[Statements][OPAREN] = 6;
    parse_table[Statements][WHILE] = 6;
    parse_table[Statements][RETURN] = 6;
    parse_table[Statements][BREAK] = 6;
    parse_table[Statements][SCAN] = 6;
    parse_table[Statements][PRINT] = 6;
    parse_table[moreStmts][LET] = 7;
    parse_table[moreStmts][COMMENT] = 7;
    parse_table[moreStmts][ID] = 7;
    parse_table[moreStmts][IF] = 7;
    parse_table[moreStmts][OPAREN] = 7;
    parse_table[moreStmts][WHILE] = 7;
    parse_table[moreStmts][RETURN] = 7;
    parse_table[moreStmts][BREAK] = 7;
    parse_table[moreStmts][SCAN] = 7;
    parse_table[moreStmts][PRINT] = 7;
    parse_table[moreStmts][EPSILON] = 8;
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
    parse_table[Stmt][BREAK] = 18;
    parse_table[ReturnStmt][RETURN] = 19;
    parse_table[BreakStmt][BREAK] = 20;
    parse_table[DeclarationStmt][LET] = 21;
    parse_table[moreDeclarations][OPAREN] = 22;
    parse_table[moreDeclarations][ID] = 23;
    parse_table[mutMod][MUT] = 24;
    parse_table[mutMod][EPSILON] = 25;
    parse_table[mutMod][ID] = 25;
    parse_table[mutMod][OPAREN] = 25;
    parse_table[Declaration][ID] = 26;
    parse_table[moreTypes][COMMA] = 27;
    parse_table[moreTypes][EPSILON] = 28;
    parse_table[moreTypes][SEMICOLON] = 28;
    parse_table[AssignStmtType2][OPAREN] = 29;
    parse_table[listTypes][OPAREN] = 30;
    parse_table[typeList][ID] = 31;
    parse_table[moreList][COMMA] = 32;
    parse_table[moreList][EPSILON] = 33;
    parse_table[moreList][CPAREN] = 33;
    parse_table[singleAssn][TRUE] = 34;
    parse_table[singleAssn][OBRACE] = 34;
    parse_table[singleAssn][NUM] = 34;
    parse_table[singleAssn][ID] = 34;
    parse_table[singleAssn][OSQUARE] = 34;
    parse_table[singleAssn][STRL] = 34;
    parse_table[singleAssn][OPAREN] = 34;
    parse_table[singleAssn][CHARL] = 34;
    parse_table[singleAssn][FALSE] = 34;
    parse_table[multAssn][TRUE] = 35;
    parse_table[multAssn][OBRACE] = 35;
    parse_table[multAssn][NUM] = 35;
    parse_table[multAssn][ID] = 35;
    parse_table[multAssn][OSQUARE] = 35;
    parse_table[multAssn][STRL] = 35;
    parse_table[multAssn][OPAREN] = 35;
    parse_table[multAssn][CHARL] = 35;
    parse_table[multAssn][FALSE] = 35;
    parse_table[moreAssn][COMMA] = 36;
    parse_table[moreAssn][COLON] = 37;
    parse_table[moreAssn][EPSILON] = 38;
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
    parse_table[IDStmts2][EPSILON] = 46;
    parse_table[IDStmts2][PLUS] = 46;
    parse_table[IDStmts2][GE] = 46;
    parse_table[IDStmts2][DIV] = 46;
    parse_table[IDStmts2][LE] = 46;
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
    parse_table[moreIndex][EPSILON] = 51;
    parse_table[moreIndex][PLUS] = 51;
    parse_table[moreIndex][GE] = 51;
    parse_table[moreIndex][DIV] = 51;
    parse_table[moreIndex][LE] = 51;
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
    parse_table[Type][STRING] = 60;
    parse_table[parameterList][ID] = 61;
    parse_table[remainingList][COMMA] = 62;
    parse_table[remainingList][EPSILON] = 63;
    parse_table[remainingList][CPAREN] = 63;
    parse_table[IfStmt][IF] = 64;
    parse_table[ElseStmt][ELSEIF] = 65;
    parse_table[ElseStmt][ELSE] = 66;
    parse_table[ElseStmt][EPSILON] = 67;
    parse_table[ElseStmt][CBRACE] = 67;
    parse_table[ElseStmt][LET] = 67;
    parse_table[ElseStmt][COMMENT] = 67;
    parse_table[ElseStmt][ID] = 67;
    parse_table[ElseStmt][IF] = 67;
    parse_table[ElseStmt][OPAREN] = 67;
    parse_table[ElseStmt][WHILE] = 67;
    parse_table[ElseStmt][RETURN] = 67;
    parse_table[ElseStmt][BREAK] = 67;
    parse_table[ElseStmt][SCAN] = 67;
    parse_table[ElseStmt][PRINT] = 67;
    parse_table[IStmt][SCAN] = 68;
    parse_table[OStmt][PRINT] = 69;
    parse_table[value][CHARL] = 70;
    parse_table[value][NUM] = 71;
    parse_table[value][STRL] = 72;
    parse_table[value][OBRACE] = 73;
    parse_table[value][OSQUARE] = 74;
    parse_table[value][TRUE] = 75;
    parse_table[value][FALSE] = 75;
    parse_table[array][OSQUARE] = 76;
    parse_table[IDList][ID] = 77;
    parse_table[IDList][NUM] = 77;
    parse_table[IDList][CHARL] = 77;
    parse_table[IDList][STRL] = 77;
    parse_table[IDList][OBRACE] = 77;
    parse_table[IDList][OSQUARE] = 77;
    parse_table[IDList][TRUE] = 77;
    parse_table[IDList][FALSE] = 77;
    parse_table[IDList][EPSILON] = 78;
    parse_table[IDList][CPAREN] = 78;
    parse_table[moreIds][COMMA] = 79;
    parse_table[moreIds][EPSILON] = 80;
    parse_table[moreIds][CPAREN] = 80;
    parse_table[arithExpn][TRUE] = 81;
    parse_table[arithExpn][OBRACE] = 81;
    parse_table[arithExpn][NUM] = 81;
    parse_table[arithExpn][ID] = 81;
    parse_table[arithExpn][OSQUARE] = 81;
    parse_table[arithExpn][STRL] = 81;
    parse_table[arithExpn][OPAREN] = 81;
    parse_table[arithExpn][CHARL] = 81;
    parse_table[arithExpn][FALSE] = 81;
    parse_table[moreTerms][PLUS] = 82;
    parse_table[moreTerms][MINUS] = 82;
    parse_table[moreTerms][EPSILON] = 83;
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
    parse_table[arithTerm][STRL] = 84;
    parse_table[arithTerm][OPAREN] = 84;
    parse_table[arithTerm][CHARL] = 84;
    parse_table[arithTerm][FALSE] = 84;
    parse_table[moreFactors][DIV] = 85;
    parse_table[moreFactors][MUL] = 85;
    parse_table[moreFactors][EPSILON] = 86;
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
    parse_table[factor][STRL] = 88;
    parse_table[factor][CHARL] = 88;
    parse_table[factor][FALSE] = 88;
    parse_table[opLow][MINUS] = 89;
    parse_table[opLow][PLUS] = 90;
    parse_table[relType][ID] = 91;
    parse_table[relType][TRUE] = 92;
    parse_table[relType][OBRACE] = 92;
    parse_table[relType][NUM] = 92;
    parse_table[relType][OSQUARE] = 92;
    parse_table[relType][STRL] = 92;
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
    parse_table[boolExpn][STRL] = 96;
    parse_table[boolExpn][CHARL] = 96;
    parse_table[boolExpn][FALSE] = 96;
    parse_table[logicalOp][AND] = 97;
    parse_table[logicalOp][NOT] = 98;
    parse_table[logicalOp][OR] = 99;
    parse_table[relationalOp][EQ] = 100;
    parse_table[relationalOp][GT] = 101;
    parse_table[relationalOp][GE] = 102;
    parse_table[relationalOp][LT] = 103;
    parse_table[relationalOp][LE] = 104;
    parse_table[relationalOp][NE] = 105;
    parse_table[LoopStmt][WHILE] = 106;
    parse_table[grid][OBRACE] = 107;
    parse_table[rows][NUM] = 108;
    parse_table[moreRows][COLON] = 109;
    parse_table[moreRows][EPSILON] = 110;
    parse_table[moreRows][CBRACE] = 110;
    parse_table[row][NUM] = 111;
    parse_table[moreNums][COMMA] = 112;
    parse_table[moreNums][EPSILON] = 113;
    parse_table[moreNums][CBRACE] = 113;
    parse_table[moreNums][COLON] = 113;
    parse_table[boolean][FALSE] = 114;
    parse_table[boolean][TRUE] = 115;
}

int parse()
{
    stack stack = createStack();
    struct token token;
    int status = gettok(&token);//need to push dollar have done in stack.c

    push(100, &stack);
    while(status!=-1)
    {
        display(stack);
        if(status == 0 && top(stack) == dollar)   //change to dollar
        {
            printf("SUCCESSFUL\n");
            return 1;
        }

        if(top(stack) == EPSILON)
        {
            pop(&stack);
        }

        else if(top(stack) < 100)
        {
            //printf("%s %s\n", token_names[top(stack)], token_names[token.type ]);
            if(top(stack) == token.type)
            {
                
                printf("Popped term %s\n",token_names[top(stack)] );   
                pop(&stack);
                memset(&token, 0, sizeof(token));
                status = gettok(&token);
                

            }
            else
            {
                printf("Error in Terminal for Token %s. Expected Token %s %d:%d\n",token_names[token.type],token_names[top(stack)],token.linenum,token.colnum);
                return 0;
            }
        }

        else    //Non terminal
        {
            int rule = parse_table[top(stack)][token.type];
            
            //printf("Rule %d\n",rule );
            int i=0;
            if(rule==-1)
            {
               printf("Error in Nonterminal for Token %s. Expected Token %s %d:%d\n",token_names[token.type],nonterm_names[top(stack)-100],token.linenum,token.colnum);
                return 0;
            }

            printf("Popped nonterm %s\n",nonterm_names[top(stack)-100] );   
            pop(&stack);
            while(rules[rule][i]!=STOP)
                i++;
            i--;
            while(i!=-1)
            {
                push(rules[rule][i],&stack);
                //printf("%d:%s ", rules[rule][i],token_names[rules[rule][i]]);
                i--;
            }
            //printf("\n");
            //printf("Top of stack %s\n",nonterm_names[top(stack)-100]);
        }
    }
    if(status==-1)
    {

        if(token.type>=100)
            printf("%s %d %d",nonterm_names[token.type-100],token.linenum,token.colnum);

      else
        printf("%s %d %d", token_names[token.type],token.linenum,token.colnum);
    }
}


int main(int argc, char *argv[]) {
   init_parse_table();
    parse();
}

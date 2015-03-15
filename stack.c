#include <stdio.h>
#include <stdlib.h>
#include "stackDataType.h"


static const char* token_names[] = {"COMMENT", "ID", "NUM", "FLOAT", "STRL", "CHARL", "LE", "EQ", "GE" , "NE", "RARROW", "ASSIGNOP", "OSQUARE", "CSQUARE", "OPAREN", "CPAREN", "OBRACE", "CBRACE", "SEMICOLON", "COLON", "COMMA", "PLUS", "MINUS", "MUL", "DIV", "LT", "GT", "DOT","RETURN", "CHAR", "I32", "F32", "BOOL", "STRING", "MAIN", "FN", "LET", "WHILE", "BREAK", "IF", "ELSE", "ELSEIF", "SCAN", "PRINT", "AND", "OR", "NOT", "TRUE", "FALSE", "MUT"};

static const char* nonterm_names[] = {"Program", "Functions", "FunctionDef", "fnReturn", "Statements", "moreStmts", "Stmt", "ReturnStmt", "BreakStmt", "DeclarationStmt", "moreDeclarations", "mutMod", "Declaration", "moreTypes", "AssignStmtType2", "listTypes", "typeList", "moreList", "singleAssn", "multAssn", "moreAssn", "IDStmts", "IDStmts2", "Index", "moreIndex", "AssignStmtType1", "FunCall", "MethodCall", "FunCallStmt", "MethodStmt", "Type", "parameterList", "remainingList", "IfStmt", "ElseStmt", "IStmt", "OStmt", "value", "array", "IDList", "moreIds", "arithExpn", "moreTerms", "arithTerm", "moreFactors", "factor", "opLow", "relType", "opHigh", "boolExpn", "logicalOp", "relationalOp", "LoopStmt", "grid", "rows", "moreRows", "row", "moreNums", "boolean","DOLLAR","EPSILON"};

stack createStack()
{ 
   stack Top;
   Top = (stack)malloc(sizeof(nodeS));
   (Top)->next = NULL;
   (Top)->value = 159;
   return Top;
}

void push(int item, stack* Top)
{
  if((*Top)->value==-1)
  {
   (*Top)->value = item; ((*Top)->next) = NULL;
   return;
  }
   stack New;
   New = (stack)malloc(sizeof(nodeS));
   New->value = item;
   New->next = *Top;
   *Top = New;
}

int pop(stack* Top)
{
   if((*Top)==NULL) {printf("Cannot Pop!\n");return -1;};
   int item;
   stack temp;
   item = (*Top)->value;
   temp = *Top;
   *Top = (*Top)->next;
   free(temp);
   return item;
}

int top(stack st)
{
   return st->value;
}

void display(stack Top)
{
  printf("Display: ");
  stack temp = Top;
  while(temp!=NULL)
  {
   //printf("%d ",temp->value);
   if(temp->value>=100)
    printf("%s ",nonterm_names[temp->value-100]);

  else
    printf("%s ", token_names[temp->value]);

   temp = temp->next;
  }

  printf("\n");
}

#ifndef _LEXER_H
#define _LEXER_H
#include <stdio.h>
#include <ctype.h>
#include <string.h>

enum token_type { COMMENT, ID, NUM, FLOAT, STRL, CHARL, LE, EQ, GE , NE, RARROW, ASSIGNOP, OSQUARE, CSQUARE, OPAREN, CPAREN, OBRACE, CBRACE, SEMICOLON, COLON, COMMA, PLUS, MINUS, MUL, DIV, LT, GT, DOT , RETURN, CHAR, I32, F32, BOOL, STRING, MAIN, FN, LET, WHILE, BREAK, IF, ELSE, ELSEIF, SCAN, PRINT, AND, OR, NOT, TRUE, FALSE, MUT};

#define MAX_LEN 80
#define KWRD_CNT 22
#define SINGLE_TKNS 12
#define KWRD_BEGIN RETURN

struct token {
  unsigned int linenum, colnum;
  enum token_type type;
  char lexeme[MAX_LEN];
  union {
    char ch;
    int inum;
    float fnum;
  } value;
};

extern int gettok(struct token* token);

#endif
#ifndef _LEXER_H
#define _LEXER_H
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>

enum token_type {DOLLAR, EPSILON, RARROW, OSQUARE, CSQUARE, OPAREN, CPAREN, OBRACE, CBRACE, SEMICOLON, COLON, COMMA, DOT, COMMENT, ID, NUM, FLOAT, STRL, CHARL, LE, EQ, GE , NE, ASSIGNOP, PLUS, MINUS, MUL, DIV, LT, GT, RETURN, CHAR, I32, F32, BOOL, STRING, MAIN, FN, LET, WHILE, BREAK, IF, ELSE, ELSEIF, SCAN, PRINT, AND, OR, NOT, TRUE, FALSE, MUT};

#define MAX_LEN 80
#define KWRD_CNT 22
#define SINGLE_TKNS 12
#define KWRD_BEGIN RETURN
#define USELESS_END COMMENT // <=
#define OP_BEGIN LE
#define OP_END GT

#define NONTERM_COUNT 59
#define TERM_COUNT 52
#define RULE_COUNT 116
#define RULE_MAX_SYMBOLS 20

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

extern int gettok(FILE *fp, struct token* token);
extern const char* token_names[];
#endif
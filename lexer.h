#ifndef _LEXER_H
#define _LEXER_H
#include <unistd.h>
#include <sys/types.h> 
#include <sys/stat.h> 
#include <fcntl.h> 
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
extern const char* token_names[];
#endif
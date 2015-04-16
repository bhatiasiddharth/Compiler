#ifndef _LEXER_H
#define _LEXER_H
#define _GNU_SOURCE
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>

enum token_type {DOLLAR, EPSILON, RARROW, CSQUARE, OPAREN, CPAREN, OBRACE, CBRACE, SEMICOLON, COLON, COMMA, DOT, COMMENT, OSQUARE, ID, NUM, FLOAT, STRL, CHARL, LE, EQ, GE , NE, ASSIGNOP, PLUS, MINUS, MUL, DIV, LT, GT, TRUE, FALSE, RETURN, CHAR, I32, F32, BOOL, STRING, MAIN, FN, LET, WHILE, BREAK, IF, ELSE, ELSEIF, SCAN, PRINT, AND, OR, NOT, MUT};

#define MAX_LEN 80
#define KWRD_CNT MUT - RETURN + 1
#define SINGLE_TKNS 12
#define KWRD_BEGIN RETURN
#define USELESS_END COMMENT // <=
#define OP_BEGIN LE
#define OP_END GT

#define NONTERM_COUNT 59
#define TERM_COUNT 52
#define RULE_COUNT 117
#define RULE_MAX_SYMBOLS 20
#define BUFFER_SIZE 1024

union value{
	char ch;
	int inum;
	unsigned int bool:1;
	float fnum;
	char string[MAX_LEN];
};

struct token {
  unsigned int linenum, colnum;
  int type;
  char lexeme[MAX_LEN];
  union value value;
};

extern const char* token_names[];
extern int token_hasvalue(int symbol);
extern int gettok(FILE *fp, struct token* token);
extern void print_value(FILE* fp, int symbol, union value value);
extern void write_token(FILE *fp, struct token* token);


#endif

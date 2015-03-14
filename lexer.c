#include <stdio.h>
#include <ctype.h>
#include <string.h>
enum token_type { COMMENT, ID, NUM, FLOAT, STRL, CHARL, LE, EQ, GE , NE, RARROW, ASSIGNOP, OSQUARE, CSQUARE, OPAREN, CPAREN, OBRACE, CBRACE, SEMICOLON, COLON, COMMA, PLUS, MINUS, MUL, DIV, LT, GT, DOT , RETURN, CHAR, I32, F32, BOOL, STRING, MAIN, FN, LET, WHILE, BREAK, IF, ELSE, ELSEIF, SCAN, PRINT, AND, OR, NOT};

const char* token_names[] = {"COMMENT", "ID", "NUM", "FLOAT", "STRL", "CHARL", "LE", "EQ", "GE" , "NE", "RARROW", "ASSIGNOP", "OSQUARE", "CSQUARE", "OPAREN", "CPAREN", "OBRACE", "CBRACE", "SEMICOLON", "COLON", "COMMA", "PLUS", "MINUS", "MUL", "DIV", "LT", "GT", "DOT","RETURN", "CHAR", "I32", "F32", "STRING", "MAIN", "FN", "LET", "WHILE", "BREAK", "IF", "ELSE", "ELSEIF", "SCAN", "PRINT", "AND", "OR", "NOT"};

int single_token_map[][2] = {
  { '[', OSQUARE },
  { ']', CSQUARE },
  { '(', OPAREN },
  { ')', CPAREN  },
  { '{', OBRACE },
  { '}', CBRACE  },
  { ';', SEMICOLON },
  { ':', COLON },
  { ',', COMMA },
  { '+', PLUS },
  { '*', MUL },
  { '.', DOT  }
};

#define MAX_LEN 80
#define KWRD_CNT 18
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

void build_lexeme(char* str, char c) {
  char ch[2] = {c, 0};
  strcat(str, ch);
}

static int gettok(struct token* token) {
  static int current_char = ' ';
  static int linenum = 1, colnum = 1;
  bzero(token->lexeme, MAX_LEN);
  
  // Skip any whitespace.
  while (isspace(current_char)) {
    if(current_char == '\n') {
      linenum++;
      colnum = 1;
    }else if(current_char == ' ') {
      colnum++;
    }
    current_char = getchar();
  }

  token->linenum = linenum;
  token->colnum = colnum;
  if (isalpha(current_char)) { // identifier: [a-zA-Z][a-zA-Z0-9]*
    token->type = ID;
    build_lexeme(token->lexeme, current_char);
    while (isalnum((current_char = getchar()))){
      build_lexeme(token->lexeme, current_char);
    }

    for(int i = KWRD_BEGIN; i < KWRD_BEGIN + KWRD_CNT; i++) {
      if(strcasecmp(token->lexeme, token_names[i]) == 0)
        token->type = i;
    }
    colnum += strlen(token->lexeme);
    return 1;
  }

  if (isdigit(current_char)) {   // Number: [0-9.]+
    token->type = NUM;
    do {
      if(current_char == '.') token->type = FLOAT;
      build_lexeme(token->lexeme, current_char);
      current_char = getchar();
    } while (isdigit(current_char) || (current_char == '.' && token->type == NUM));

    if(token->type == NUM)
      token->value.inum = 0;
    else
      token->value.fnum = 0.0;

    colnum += strlen(token->lexeme);
    return 1;
  }

  if (current_char == '/') {
    if((current_char = getchar()) == '/') {
      token->type = COMMENT;
      // Comment until end of line.
      do {
        current_char = getchar();
      }while (current_char != EOF && current_char != '\n' && current_char != '\r');
      return 1;
    }else {
      token->type = DIV;
      colnum +=1;
      return 1;
    }
 }

  if (current_char == '=') {
    if((current_char = getchar()) == '=') {
      token->type = EQ;
      strcpy(token->lexeme, "==");
      current_char = getchar();
      colnum += 2;
      return 1;
  }else {
      token->type = ASSIGNOP;
      build_lexeme(token->lexeme, '=');
      colnum += 1;
      return 1;
   }
 }

  if (current_char == '-') {
    if((current_char = getchar()) == '>'){
      token->type = RARROW;
      strcpy(token->lexeme, "->");
      current_char = getchar();
      colnum += 2;
      return 1;
  }else {
      token->type = MINUS;
      build_lexeme(token->lexeme, '-');
      colnum += 1;
      return 1;
   }
}
   if (current_char == '<') {

    if((current_char = getchar()) == '=') {
      token->type = LE;
      strcpy(token->lexeme, "<=");
      current_char = getchar();
      colnum += 2;
      return 1;
  }else if(current_char == '>'){
      token->type = NE;
      strcpy(token->lexeme, "<>");
      current_char = getchar();
      colnum += 2;
      return 1;
   }else{
      token->type = LT;
      build_lexeme(token->lexeme, '<');
      colnum += 1;
      return 1;
   }
}
   if (current_char == '>') {

    if((current_char = getchar()) == '=') {
      token->type = GE;
      strcpy(token->lexeme, ">=");
      current_char = getchar();
      colnum += 2;
      return 1;
  }else{
      token->type = GT;
      build_lexeme(token->lexeme, '>');
      colnum += 1;
      return 1;
   }
}
  if (current_char == '"') {
    token->type = STRL;
    while (isalnum((current_char = getchar()))){
      build_lexeme(token->lexeme, current_char);
    }
    
    if(current_char == '"'){
      colnum += strlen(token->lexeme) + 2;
      return 1;
    }else {
      return -1;
    }
  }

  if (current_char == '\'') {
    token->type = CHARL;
    isalnum(current_char = getchar());

    build_lexeme(token->lexeme, current_char);
    token->value.ch = current_char;
    if((current_char = getchar())== '\''){
      colnum += strlen(token->lexeme) + 2;
      return 1;
    }else{
      return -1;
    }
  }

  for(int i = 0; i < SINGLE_TKNS; i++) {
    if(current_char == single_token_map[i][0]){          
      token->type = single_token_map[i][1];
      build_lexeme(token->lexeme, current_char);
      current_char = getchar();
      colnum += 1;      
      return 1;
    }
  }
    
  if(!feof(stdin)){
    // Error - Invalid token
    printf("(%d, %d) Error - Invalid token\n", linenum, colnum);
    return -1;
  }
  return 0;
}

int main(int argc, char *argv[]) {
  int status;
  struct token token;
  while((status = gettok(&token))) {
    if(status == -1) {
      //printf("Error - Invalid token\n");
      break;
    }
    printf("(%2d, %2d)\t %10s\t %s%s \n", token.linenum, token.colnum, token_names[token.type], (token.type >= KWRD_BEGIN ? "Keyword - " : ""), token.lexeme);
    bzero(&token, sizeof(token));
  }
  return 0;
}
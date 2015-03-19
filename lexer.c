#include "lexer.h"

const char* token_names[] = {"DOLLAR", "EPSILON", "RARROW", "OSQUARE", "CSQUARE", "OPAREN", "CPAREN", "OBRACE", "CBRACE", "SEMICOLON", "COLON", "COMMA", "DOT", "COMMENT", "ID", "NUM", "FLOAT", "STRL", "CHARL", "LE", "EQ", "GE ", "NE", "ASSIGNOP", "PLUS", "MINUS", "MUL", "DIV", "LT", "GT", "TRUE", "FALSE", "RETURN", "CHAR", "I32", "F32", "BOOL", "STRING", "MAIN", "FN", "LET", "WHILE", "BREAK", "IF", "ELSE", "ELSEIF", "SCAN", "PRINT", "AND", "OR", "NOT", "MUT"};

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

void build_lexeme(char* str, char c) {
  char ch[2] = {c, 0};
  strcat(str, ch);
}

void print_value(FILE* fp, int symbol, union value value){
    if(symbol == NUM) {
        fprintf(fp, "%d ", value.inum);
    }else if(symbol == FLOAT) {
        fprintf(fp, "%f ", value.fnum);
    }else if(symbol == CHARL) {
        fprintf(fp, "\'%c\' ", value.ch);
    }else if(symbol == TRUE || symbol == FALSE) {
        fprintf(fp, "%d ", value.bool);
    }else if(symbol == ID) {
        fprintf(fp, "%s ", value.string);
    }else if(symbol == STRL) {
        fprintf(fp, "\"%s\" ", value.string);
    }else {
        fprintf(fp, "Empty ");
    }
}

int token_hasvalue(int symbol) {
  return (symbol ==ID || symbol == NUM || symbol == FLOAT || symbol == STRL || symbol == BOOL || symbol == CHARL);
}

int gettok(FILE *fp, struct token* token) {
  int i;
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
    current_char = getc(fp);
  }

  token->linenum = linenum;
  token->colnum = colnum;
  if (isalpha(current_char)) { // identifier: [a-zA-Z][a-zA-Z0-9]*
    token->type = ID;
    build_lexeme(token->lexeme, current_char);
    while (isalnum((current_char = getc(fp)))){
      build_lexeme(token->lexeme, current_char);
    }

    for(i = KWRD_BEGIN; i < KWRD_BEGIN + KWRD_CNT; i++) {
      if(strcasecmp(token->lexeme, token_names[i]) == 0)
        token->type = i;
    }

    if(strcasecmp(token->lexeme, "true") == 0) {
      token->type = TRUE;
      token->value.bool = 1;
    }

    if(strcasecmp(token->lexeme, "false") == 0) {
      token->type = FALSE;
      token->value.bool = 0;
    }
    strcpy(token->value.string, token->lexeme);
    colnum += strlen(token->lexeme);
    return 1;
  }

  if (isdigit(current_char)) {   // Number: [0-9.]+
    token->type = NUM;
    do {
      if(current_char == '.') token->type = FLOAT;
      build_lexeme(token->lexeme, current_char);
      current_char = getc(fp);
    } while (isdigit(current_char) || (current_char == '.' && token->type == NUM));

    if(token->type == NUM)
      token->value.inum = atoi(token->lexeme);
    else
      token->value.fnum = atof(token->lexeme);

    colnum += strlen(token->lexeme);
    return 1;
  }

  if (current_char == '/') {
    if((current_char = getc(fp)) == '/') {
      token->type = COMMENT;
      // Comment until end of line.
      do {
        current_char = getc(fp);
      }while (current_char != EOF && current_char != '\n' && current_char != '\r');
      return 1;
    }else {
      token->type = DIV;
      colnum +=1;
      return 1;
    }
 }

  if (current_char == '=') {
    if((current_char = getc(fp)) == '=') {
      token->type = EQ;
      strcpy(token->lexeme, "==");
      current_char = getc(fp);
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
    if((current_char = getc(fp)) == '>'){
      token->type = RARROW;
      strcpy(token->lexeme, "->");
      current_char = getc(fp);
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

    if((current_char = getc(fp)) == '=') {
      token->type = LE;
      strcpy(token->lexeme, "<=");
      current_char = getc(fp);
      colnum += 2;
      return 1;
  }else if(current_char == '>'){
      token->type = NE;
      strcpy(token->lexeme, "<>");
      current_char = getc(fp);
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

    if((current_char = getc(fp)) == '=') {
      token->type = GE;
      strcpy(token->lexeme, ">=");
      current_char = getc(fp);
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
    while ((current_char = getc(fp)) && (isalnum(current_char) || isspace(current_char))) {
      build_lexeme(token->lexeme, current_char);
    }
    
    strcpy(token->value.string, token->lexeme);
    if(current_char == '"'){
      current_char = getc(fp);
      colnum += strlen(token->lexeme) + 2;
      return 1;
    }else {
      current_char = getc(fp);
      return -1;
    }
  }

  if (current_char == '\'') {
    token->type = CHARL;
    current_char = getc(fp);

    build_lexeme(token->lexeme, current_char);
    token->value.ch = current_char;
    if((current_char = getc(fp))== '\''){
      current_char = getc(fp);
      colnum += strlen(token->lexeme) + 2;
      return 1;
    }else{
      return -1;
    }
  }

  for(i = 0; i < SINGLE_TKNS; i++) {
    if(current_char == single_token_map[i][0]){          
      token->type = single_token_map[i][1];
      build_lexeme(token->lexeme, current_char);
      current_char = getc(fp);
      colnum += 1;      
      return 1;
    }
  }
    
  if(!feof(fp)){
    // Error - Invalid token
    printf("(%d, %d) Error - Invalid token\n", linenum, colnum);
    return -1;
  }
  return 0;
}

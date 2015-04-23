#include "lexer.h"

const char* token_names[] = {"DOLLAR", "EPSILON", "RARROW", "CSQUARE", "OPAREN", "CPAREN", "OBRACE", "CBRACE", "SEMICOLON", "COMMA", "DOT", "COMMENT", "COLON", "OSQUARE", "ID", "NUM", "FLOAT", "STRL", "CHARL", "LE", "EQ", "GE ", "NE", "ASSIGNOP", "PLUS", "MINUS", "MUL", "DIV", "LT", "GT", "TRUE", "FALSE", "RETURN", "CHAR", "I32", "F32", "BOOL", "STRING", "MAIN", "FN", "LET", "WHILE", "BREAK", "IF", "ELSE", "ELSEIF", "SCAN", "PRINT", "AND", "OR", "NOT", "MUT"};
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
        fprintf(fp, "%c ", value.ch);
    }else if(symbol == TRUE || symbol == FALSE) {
        fprintf(fp, "%d ", value.bool);
    }else if(symbol == STRL) {
        fprintf(fp, "%s ", value.string);
    }else if(symbol == ID) {
        fprintf(fp, "%s ", value.string);
    }else {
        fprintf(fp, "Empty ");
    }
}

int token_hasvalue(int symbol) {
  return (symbol == ID || symbol == NUM || symbol == FLOAT || symbol == STRL || symbol == TRUE || symbol == FALSE || symbol == CHARL);
}

int gettok(FILE *fp, struct token* token) {
  int i;
  static int count = 0;
  static int linenum = 1, colnum = 1;
  size_t startlen = 2*BUFFER_SIZE;
  size_t len = BUFFER_SIZE;
  static char* token_buffer;

  // BUFFERED I/O
  if(count == 0) {
    token_buffer = (char*) malloc(2 * BUFFER_SIZE * sizeof(char));
    getdelim(&token_buffer, &startlen, 0, fp);
  }else if(count > BUFFER_SIZE) {
    token_buffer = token_buffer + BUFFER_SIZE;
    count = count % BUFFER_SIZE;
    getdelim(&token_buffer, &len, 0, fp);
  }
  bzero(token->lexeme, MAX_LEN);

  // Skip any whitespace.
  while (isspace(token_buffer[count])) {
    if(token_buffer[count] == '\n') {
      linenum++;
      colnum = 1;
    }else if(token_buffer[count] == ' ' || token_buffer[count] == '\t') {
      colnum++;
    }
    count++;
  }

  token->linenum = linenum;
  token->colnum = colnum;
  if (isalpha(token_buffer[count])) { // identifier: [a-zA-Z][a-zA-Z0-9]*
    token->type = ID;
    build_lexeme(token->lexeme, token_buffer[count]);
    while (isalnum(token_buffer[++count])){
      build_lexeme(token->lexeme, token_buffer[count]);
    }
    strcpy(token->value.string, token->lexeme);
    if(strlen(token->lexeme) > 10) {
      fprintf(stderr, "(%d, %d) Error - Identifier length should be less than 10\n", linenum, colnum);
      exit(1);
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

    colnum += strlen(token->lexeme);

    // printf("count - %d %c \n", count, token_buffer[count]);
    return 1;
  }

  if (isdigit(token_buffer[count])) {   // Number: [0-9.]+
    token->type = NUM;
    do {
      if(token_buffer[count] == '.') token->type = FLOAT;
      build_lexeme(token->lexeme, token_buffer[count]);
      count++;
    } while (isdigit(token_buffer[count]) || (token_buffer[count] == '.' && token->type == NUM));

    if(token->type == NUM)
      token->value.inum = atoi(token->lexeme);
    else
      token->value.fnum = atof(token->lexeme);

    colnum += strlen(token->lexeme);
    return 1;
  }

  if (token_buffer[count] == '/') {
    if(token_buffer[++count] == '/') {
      token->type = COMMENT;
      // Comment until end of line.
      do {
        count++;
      }while (token_buffer[count] != EOF && token_buffer[count] != '\n' && token_buffer[count] != '\r');
      return 1;
    }else {
      token->type = DIV;
      colnum +=1;
      return 1;
    }
 }

  if (token_buffer[count] == '=') {
    if(token_buffer[++count] == '=') {
      token->type = EQ;
      strcpy(token->lexeme, "==");
      count++;
      colnum += 2;
      return 1;
  }else {
      token->type = ASSIGNOP;
      build_lexeme(token->lexeme, '=');
      colnum += 1;
      return 1;
   }
 }

  if (token_buffer[count] == '-') {
    if(token_buffer[++count] == '>'){
      token->type = RARROW;
      strcpy(token->lexeme, "->");
      count++;
      colnum += 2;
      return 1;
  }else {
      token->type = MINUS;
      build_lexeme(token->lexeme, '-');
      colnum += 1;
      return 1;
   }
}
   if (token_buffer[count] == '<') {

    if(token_buffer[++count] == '=') {
      token->type = LE;
      strcpy(token->lexeme, "<=");
      count++;
      colnum += 2;
      return 1;
  }else if(token_buffer[count] == '>'){
      token->type = NE;
      strcpy(token->lexeme, "<>");
      count++;
      colnum += 2;
      return 1;
   }else{
      token->type = LT;
      build_lexeme(token->lexeme, '<');
      colnum += 1;
      return 1;
   }
}
   if (token_buffer[count] == '>') {

    if(token_buffer[++count] == '=') {
      token->type = GE;
      strcpy(token->lexeme, ">=");
      count++;
      colnum += 2;
      return 1;
  }else{
      token->type = GT;
      build_lexeme(token->lexeme, '>');
      colnum += 1;
      return 1;
   }
}
  if (token_buffer[count] == '"') {
    token->type = STRL;
    while (token_buffer[++count] && (isalnum(token_buffer[count]) || isspace(token_buffer[count]))) {
      build_lexeme(token->lexeme, token_buffer[count]);
    }

    strcpy(token->value.string, token->lexeme);
    if(token_buffer[count] == '"'){
      count++;
      colnum += strlen(token->lexeme) + 2;
      return 1;
    }else {
      count++;
      return -1;
    }
  }

  if (token_buffer[count] == '\'') {
    token->type = CHARL;
    count++;

    build_lexeme(token->lexeme, token_buffer[count]);
    token->value.ch = token_buffer[count];
    if(token_buffer[++count]== '\''){
      count++;
      colnum += strlen(token->lexeme) + 2;
      return 1;
    }else{
      return -1;
    }
  }

  for(i = 0; i < SINGLE_TKNS; i++) {
    if(token_buffer[count] == single_token_map[i][0]){
      token->type = single_token_map[i][1];
      build_lexeme(token->lexeme, token_buffer[count]);
      count++;
      colnum += 1;
      return 1;
    }
  }

  if(token_buffer[count] != 0){
    // Error - Invalid token
    printf("(%d, %d) Error - Invalid token\n", linenum, colnum);
    return -1;
  }
  return 0;
}

void write_token(FILE *fp, struct token* token) {
    fprintf(fp, "(%2d, %2d)\t", token->linenum, token->colnum);
    fprintf(fp, "%s", (token->type >= KWRD_BEGIN ? "Keyword - " : ""));
    fprintf(fp, "%-20s\t", token_names[token->type]);
    if(token_hasvalue(token->type))
        print_value(fp, token->type, token->value);
    fprintf(fp, "\n");
}

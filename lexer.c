#include <stdio.h>
#include <ctype.h>
enum Tokens { COMMENT, ID, NUM, FLOAT, STRL, CHARL, LE, EQ, GE , NE, RARROW, ASSIGNOP = '=', OSQUARE = '[', CSQUARE = ']', OPAREN = '(', CPAREN = ')', OBRACE = '{', CBRACE = '}', SEMICOLON = ';', COLON = ':', COMMA = ',', PLUS = '+', MINUS = '-', MUL = '*', DIV = '/', LT = '<', GT = '>', DOT  = '.'};

// COMMENT, ID, NUM, FLOAT, STRL, CHARL
enum Keywords {RETURN, CHAR, I32, F32, BOOL, STRING, MAIN, FN, LET, WHILE, BREAK, IF, ELSE, ELSEIF, SCAN, PRINT,AND, OR, NOT};

const char* kwd_list[] = {"return", "char", "i32", "f32", "string", "main", "fn", "let", "while", "break", "if", "else", "elseif", "scan", "print", "and", "or", "not"};

enum Tokens token_list[] = { COMMENT, ID, NUM, FLOAT, STRL, CHARL, LE, EQ, GE , NE, RARROW, ASSIGNOP, OSQUARE , CSQUARE , OPAREN , CPAREN , OBRACE , CBRACE , SEMICOLON , COLON , COMMA , PLUS , MINUS , MUL , DIV , LT , GT , DOT  };
/*
valid_symbols = 0-9 + A-Z + a-z + _
				=, {, }, (, ), [, ], ;, :, +, -. *, /, <, >, ->, .
struct Token {
	unsigned int linenum, col;
	type Tokens;
	union {
		char el;
		char str[80];
		int integer;
		float real;
	}lexeme;
};
*/

/// gettok - Return the next token from standard input.

/*int readchar(int *linenum, int *colnum) {
	int c = getchar();
	if(c == '\n') {
		*linenum++;
		readchar();
	}else if(c == ' ') {
		*colnum++;
		readchar();
	}
	return c;
}*/
static int gettok() {
  static int current_char = ' ';
  static int linenum, colnum;
  char num_str[80];
  // Skip any whitespace.
  while (isspace(current_char)) {
  	// last_char = current_char;
    current_char = getchar();
    }

  if (isalpha(current_char)) { // identifier: [a-zA-Z][a-zA-Z0-9]*
    //strcpy(identifier_str, current_char);
    while (isalnum((current_char = getchar()))){
      //strcat(identifier_str, current_char);
    }

    printf("ID \n");
  	return ID;
  }

  int real_flag = 0;
  if (isdigit(current_char)) {   // Number: [0-9.]+
    do {
    	if(current_char == '.')	real_flag = 1;
      //strcat(num_str, current_char);
      current_char = getchar();
    } while (isdigit(current_char) || (current_char == '.' && !real_flag));

    //NumVal = strtod(num_str.c_str(), 0);
    if(!real_flag) printf("number \n"); else printf("float\n");
    return 1;
  }

  if (current_char == '/') {
  	current_char = getchar();
	 if(current_char == '/') {
	    // Comment until end of line.
	    do current_char = getchar();
	    while (current_char != EOF && current_char != '\n' && current_char != '\r');
	    printf("Comment \n");
	    return 1;
	}else {
		printf("Div \n");
		gettok();
	   	return 1;
   }
  }

  if (current_char == '"') {
    
    while (isalnum((current_char = getchar()))){
      //strcat(identifier_str, current_char);
    }
    
    if(current_char == '"'){
    	printf("String \n");
   		return 1;
    }
  }

  if (current_char == '\'') {
    
    isalnum(current_char = getchar());
    
    if((current_char = getchar())== '\''){
    	printf("Character \n");
   		return 1;
    }
  }
    if(current_char == '=' || current_char == '{' || current_char == '}' || current_char == '(' || current_char == ')' || current_char == '[' || current_char == ']' || current_char == ';' || current_char == ':' || current_char == '+' || current_char == '-' || current_char ==  '.' || current_char == '*' || current_char == '/' || current_char == '<' || current_char == '>' || current_char == ','){
    	printf("single char - %c\n", current_char);
    	current_char = getchar();
    	return 1;
    }
    if(!feof(stdin))
     	printf("mismatch - %c", current_char);
    return 0;
}

int main(int argc, char *argv[]) {
	int t;
	do{
		t = gettok();
	} while(t !=  0);

	for(int i= 0; i < 28; i++) {
		printf("Token - %d\n", token_list[i]);
	}
	return 0;
}
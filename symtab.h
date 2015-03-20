/*build symbol tables
 * and function tables
 */

#ifndef SYMTAB_H
#define SYMTAB_H
#include "parser.h"

typedef enum{GLOBAL, LOCAL, PARAM} Scope;
typedef struct var_symbol VarSymbol;

enum var_type {T_STR, T_ARRAY, T_INT, T_CHAR, T_FUN, T_GRID, T_FLOAT, T_BOOL};
 /* SIZE is the size of the hash table */
#define SIZE 211
/* SHIFT is the power of two used as multiplier in hash function  */
#define SHIFT 4

//st_node
struct var_symbol {
     char* name;
     Scope scope;
     enum var_type type;
     int offset;
     // for string and array
     unsigned int size;

     unsigned int mutable:1;
     unsigned int initialized:1;
     union value* value;
     struct var_symbol* next;/*used in hashTable*/
     struct var_symbol* next_FIFO;/*used in FIFO*/
};


typedef struct symbol_table SymbolTable;
struct symbol_table {
     int size;
     Scope scope;
     VarSymbol* hashTable[SIZE];
     /* FIFO queue used to record orders of variables*/
     VarSymbol* varList;
     struct symbol_table* next;
};

typedef struct fun_symbol FunSymbol;
struct fun_symbol {
     char* name;
     int type;
     int offset;
     int paramNum;
     SymbolTable* symbolTable;
     struct fun_symbol* next;
};

// for function call
struct function_attr {
     int param_count;
     int return_type;
     int* param_types;
     union value* value;
};

extern SymbolTable* tables;
/* the hash function */
int hash ( char * key );

/* initialize symbol tables*/
void initTable();

/* create a new symbol table of certain scope */
SymbolTable* newSymbolTable(Scope s);

/* manipulate the symbol table stack*/
SymbolTable* topTable();
SymbolTable* popTable();
void pushTable(SymbolTable* st);

/* look up for a symbol entry*/
VarSymbol* lookup_var_top(char* name);
VarSymbol* lookup_var(char * name);
FunSymbol* lookup_fun(char * name);

/* insert symbol entries */
int insert_var(char * name, Scope s, int offset, enum var_type type, union value* value,int size);
int insert_fun(char* name, SymbolTable* st, int num, enum var_type type);



/* prints a formatted listing of the symbol table */
void printSymTab(SymbolTable* st, FILE *fp);
void printFunTab(FILE *fp);

#endif

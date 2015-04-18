#ifndef _SYMTAB_H
#define _SYMTAB_H
#include "parser.h"

typedef enum{GLOBAL, LOCAL, PARAM} Scope;
extern char *strdup(const char *s);

enum var_type {T_STR, T_INT, T_CHAR, T_FLOAT, T_BOOL, T_FUN, T_ARRAY, T_GRID};
 /* SIZE is the size of the hash table */
#define SIZE 211
/* SHIFT is the power of two used as multiplier in hash function  */
#define SHIFT 4

struct var_symbol {
     char* name;
     int scope;
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

struct symbol_table {
     int size;
     int scope;
     struct var_symbol* hashTable[SIZE];
     struct var_symbol* varList;
     struct symbol_table* next;
};

struct fun_symbol {
     char* name;
     int type;
     int offset;
     int param_num;
     struct symbol_table* symbolTable;
     struct fun_symbol* next;
};

extern struct symbol_table* tables;
/* the hash function */
int hash ( char * key );

/* initialize symbol tables*/
void init_table();

/* create a new symbol table of certain scope */
struct symbol_table* new_symtable(Scope s);

/* manipulate the symbol table stack*/
struct symbol_table* top_table();
struct symbol_table* pop_table();
void push_table(struct symbol_table* st);

/* look up for a symbol entry*/
struct var_symbol* lookup_var_top(char* name);
struct var_symbol* lookup_var(char * name);
struct fun_symbol* lookup_fun(char * name);

/* insert symbol entries */
int insert_var(char * name, Scope s, int offset, enum var_type type, union value* value,int size);
int insert_fun(char* name, struct symbol_table* st, int num, enum var_type type);



/* prints a formatted listing of the symbol table */
void print_symtab(struct symbol_table* st, FILE *fp,int headerFlag);
void print_funtab(FILE *fp);
struct var_symbol* lookup_var_offset (struct symbol_table* st,int offset);
void symtab_write(const char* symbols_file, struct tree_node* syntax_tree);
#endif

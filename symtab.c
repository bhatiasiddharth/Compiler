#include "symtab.h"
#include "analyzer.h"
#include "parser.h"

struct symbol_table* tables = NULL;
/* funs store all function symbols */
struct fun_symbol* funs = NULL;

int hash ( char * key ) {
     int temp = 0;
     int i = 0;
     while (key[i] != '\0')
     { temp = ((temp << SHIFT) + key[i]) % SIZE;
          ++i;
     }
     return temp;
}
void init_table()
{
     tables = new_symtable(GLOBAL);
}

/* create a new symbol table of certain scope */
struct symbol_table* new_symtable(Scope scope) {
     int i;
     struct symbol_table* st = (struct symbol_table*)malloc(sizeof(struct symbol_table));
     if(st == NULL){
        fprintf(stderr, "Failed to malloc for symbal table.\n");
     }
     st->scope = scope;
     st->size = 0;
     st->next = NULL;
     st->varList = NULL;
     for(i = 0;i<SIZE;i++)
          st->hashTable[i] = NULL;
     return st;
}


struct symbol_table* top_table() {
     return tables;
}

struct symbol_table* pop_table() {
     if(tables == NULL){
        fprintf(stderr, "Pop an empty table list.\n");
     }
     struct symbol_table* st = tables;
     tables = tables->next;
     return st;
}

void push_table(struct symbol_table* st) {
     if(st == NULL){
      fprintf(stderr, "Push an null table.\n");
     }
     st->next = tables;
     tables = st;
}


/* look up for variables in top symbol table*/
struct var_symbol* lookup_var_top(char* name) {
    if(tables == NULL)
      return NULL;

    struct var_symbol* l;
    int h = hash(name);
    for(l = tables->hashTable[h]; l!=NULL; l=l->next){
        if(strcmp(l->name, name) == 0 )
            break;
    }

    return l;
}

/* lookup for variable with offset this and in symboltable st*/
struct var_symbol* lookup_var_offset (struct symbol_table* st,int offset) {
     if(st == NULL)
        return NULL;


     struct var_symbol* vs;

     for (int i=0;i<SIZE;++i)
     {
          for(vs = st->hashTable[i]; vs != NULL; vs=vs->next)
          {
               if(vs->offset==offset)
                return vs;
          }
     }


     return NULL;                  /* may be NULL */
}

/* lookup for all tables in the stack */
struct var_symbol* lookup_var (char * name) {
     if(tables == NULL)
        return NULL;


     int h = hash(name);
     struct symbol_table* st;
     struct var_symbol* l;

     for(st = tables; st!=NULL; st=st->next) /* iteration of all symbol tables in stack */
     {
          for(l = st->hashTable[h]; l!=NULL; l=l->next) /* iteration of all linkedlist in a symboltable */
          {
               if(strcmp(l->name, name)==0)
                    return l;
          }
     }
     return NULL;                  /* may be NULL */
}

/* look up for a function symbol*/
struct fun_symbol* lookup_fun(char * name) {
     if(funs == NULL)
        return NULL;
     struct fun_symbol* fs;
     for(fs=funs;fs!=NULL; fs = fs->next)
     {
          if(strcmp(fs->name, name)==0)
               break;
     }
     return fs;
}

/* Always insert var into the top symbol table*/
int insert_var(char * name, Scope scope, int offset, enum var_type type, union value* value,int size,int mutflag) {
     struct var_symbol* l, *tmp;
     int h = hash(name);

     /*Check duplication*/
     if(tables == NULL){
        l = NULL;
      }
     else{
        l =  tables->hashTable[h];
        while ((l != NULL) && (strcmp(name,l->name) != 0))
          l = l->next;
     }


     if (l != NULL){
        fprintf(stderr, "Duplicate declarations of variable: %s.\n", name);
        //exit(1);
     }

      l = (struct var_symbol*) malloc(sizeof(struct var_symbol));
      if(l == NULL){
        fprintf(stderr, "Failed to malloc for struct var_symbol.\n" );
      }
      l->name = strdup(name);
      l->scope = scope;
      l->type = type;
      l->offset = offset;
      l->size=size;
      l->mutable=mutflag;
      // copy value
      //l->value = (union value*) malloc(sizeof(union value));
      //*(l->value) = value;

      l->value = value;
      l->next = tables->hashTable[h];
      tables->hashTable[h] = l;
      l->next_FIFO = NULL;
      if(tables->varList == NULL){ /*First insertion*/
          tables->varList = l;
      }
      else{
          for(tmp=tables->varList; tmp->next_FIFO != NULL; tmp = tmp->next_FIFO);
          tmp->next_FIFO = l;
      }

      return 0;

}

int insert_fun(char* name, struct symbol_table* st, int num, enum var_type type) {
     struct fun_symbol* fs;

     /*Check duplication*/
     if(lookup_fun(name) != NULL){
        fprintf(stderr, "Duplicate declarations of function: %s\n", name);
        //exit(1);
     }
     fs = (struct fun_symbol*)malloc(sizeof(struct fun_symbol));
     if(fs == NULL){
        fprintf(stderr, "Failed to malloc for struct fun_symbol.\n");
     }
     fs->name = strdup(name);
     fs->type = type;
     fs->param_num = num;
     fs->symbolTable = st;
     fs->next = funs;
     funs = fs;

     return 0;
}



void print_var(struct var_symbol* vs, FILE* fp) {

    if(vs->size>0)
    for (int i = 0; i < vs->size; ++i)
    {
        switch(vs->type) {
        case T_STR : fprintf(fp, "%s ", vs->value[i].string);
                break;
        case T_INT : fprintf(fp, "%d ", vs->value[i].inum);
                break;
        case T_CHAR:fprintf(fp, "%c ", vs->value[i].ch);
                break;
        case T_FUN : // fprintf(fp, "%s ", vs->value[i]->inum);
                break;
        case T_FLOAT: fprintf(fp, "%f ", vs->value[i].fnum);
                break;
        case T_BOOL: fprintf(fp, "%d ", vs->value[i].bool);
                break;
      }
    }

}


void print_symtab(struct symbol_table* st, FILE *fp,int headerFlag) {
     int i;
     if(headerFlag)
     {
      fprintf(fp,"Variable Name Scope Offset Value \n");
      fprintf(fp,"------------- ----- ------ -----\n");
    }
     struct var_symbol* vs = NULL;
     for (i=0;i<SIZE;++i)
     {
          for(vs = st->hashTable[i]; vs != NULL; vs=vs->next)
          {
               fprintf(fp, "%-14s", vs->name);
               fprintf(fp, "%-10d", vs->scope);
               fprintf(fp, "%-7d", vs->offset);
               // fprintf(fp, "%-8d");
               print_var(vs, fp);
               fprintf(fp, "\n");
          }
     }

     //fprintf(fp, "\n");
}

void print_funtab(FILE *fp) {
      fprintf(fp, "Function: main()\n");
      print_symtab(tables,fp,1);
      fprintf(fp,"\n");
      struct fun_symbol* fs=funs;
      while(fs!=NULL)
      {
        fprintf(fp, "Function: %s(",fs->name);
        for (int i = 0; i < fs->param_num; ++i)
        {
          struct var_symbol* vs=lookup_var_offset(fs->symbolTable,i);
          if(vs!=NULL)
          {
            if(i!=0)fprintf(fp, ",");
            fprintf(fp, "%s", vs->name );
          }
        }
        fprintf(fp,")\n");

        print_symtab(fs->symbolTable,fp,1);
        fprintf(fp,"\n\n");
        fs=fs->next;
      }
}

void symtab_write(const char* symbols_file, struct tree_node* syntax_tree) {
  FILE *fp = fopen(symbols_file, "w+");
  init_table();
  st_fill(syntax_tree,GLOBAL,tables, "main",fp);
  print_funtab(fp);
  fclose(fp);
}

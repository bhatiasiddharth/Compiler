/*build symbol tables
 * and function tables
 */
#include "symtab.h"
/* tables store different symbol tables,
 * incluidng global, local and param.
 */
SymbolTable* tables = NULL;
/* funs store all function symbols */
FunSymbol* funs = NULL;



/* temporary table used to allocate small symtabs for Compound & Param */
SymbolTable* CompoundST;
SymbolTable* ParamST;



/* the hash function */
int hash ( char * key ) {
     int temp = 0;
     int i = 0;
     while (key[i] != '\0')
     { temp = ((temp << SHIFT) + key[i]) % SIZE;
          ++i;
     }
     return temp;
}

/* Note: we insert for input()&output() here*/
void initTable()
{
     CompoundST = newSymbolTable(LOCAL);
     ParamST = newSymbolTable(PARAM);
     tables = newSymbolTable(GLOBAL);

     //insert_fun("input", ParamST, 0, TYPE_INTEGER);
     //ParamST = newSymbolTable(PARAM);

     // pushTable(ParamST);
     // insert_var("i", PARAM, ParamST->size++, TYPE_INTEGER);
     // popTable();
     // insert_fun("output", ParamST, ParamST->size, TYPE_VOID);
     // ParamST = newSymbolTable(PARAM);
}

/* create a new symbol table of certain scope */
SymbolTable* newSymbolTable(Scope scope)
{
     int i;
     SymbolTable* st = (SymbolTable*)malloc(sizeof(SymbolTable));
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


/* manipulate the symbol table stack*/
SymbolTable* topTable()
{
     return tables;
}

SymbolTable* popTable()
{
     if(tables == NULL){
        fprintf(stderr, "Pop an empty table list.\n");
     }
     SymbolTable* st = tables;
     tables = tables->next;
     return st;
}

void pushTable(SymbolTable* st)
{
     if(st == NULL){
      fprintf(stderr, "Push an null table.\n");
     }
     st->next = tables;
     tables = st;
}


/* look up for variables in top symbol table*/
VarSymbol* lookup_var_top(char* name)
{
    if(tables == NULL)
      return NULL;

    VarSymbol* l;
    int h = hash(name);
    for(l = tables->hashTable[h]; l!=NULL; l=l->next){
        if(strcmp(l->name, name) == 0 )
            break;
    }

    return l;
}

/* lookup for variable with offset this and in symboltable st*/
VarSymbol* lookup_var_offset (SymbolTable* st,int offset)
{
     if(st == NULL)
        return NULL;

       
     VarSymbol* vs;
     
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
VarSymbol* lookup_var (char * name)
{
     if(tables == NULL)
        return NULL;

       
     int h = hash(name);
     SymbolTable* st;
     VarSymbol* l;
     
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
FunSymbol* lookup_fun(char * name)
{
     if(funs == NULL)
        return NULL;
     FunSymbol* fs;
     for(fs=funs;fs!=NULL; fs = fs->next)
     {
          if(strcmp(fs->name, name)==0)
               break;
     }
     return fs;
}

/* Always insert var into the top symbol table*/
int insert_var(char * name, Scope scope, int offset, enum var_type type, union value* value,int size)
{
     VarSymbol* l, *tmp;
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
        return 1;
     }

      l = (VarSymbol*) malloc(sizeof(VarSymbol));
      if(l == NULL){
        fprintf(stderr, "Failed to malloc for VarSymbol.\n" );
      }
      l->name = strdup(name);
      l->scope = scope;
      l->type = type;
      l->offset = offset;
      l->size=size;
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

int insert_fun(char* name, SymbolTable* st, int num, enum var_type type)
{
     FunSymbol* fs;

     /*Check duplication*/
     if(lookup_fun(name) != NULL){
        fprintf(stderr, "Duplicate declarations of function: %s\n", name);
        return 1;
     }
     fs = (FunSymbol*)malloc(sizeof(FunSymbol));
     if(fs == NULL){
        fprintf(stderr, "Failed to malloc for FunSymbol.\n");
     }
     fs->name = strdup(name);
     fs->type = type;
     fs->paramNum = num;
     fs->symbolTable = st;
     fs->next = funs;
     funs = fs;

     return 0;
}
                   


void print_var(VarSymbol* vs, FILE* fp) {

    if(vs->size>0)
    for (int i = 0; i < vs->size; ++i)
    {
          switch(vs->type) {
        case T_STR : fprintf(fp, "%s ", vs->value[i].string);
                break;
        // T_ARRAY fprintf(fp, "%s ", vs->value[i]->inum);
        //         break;
        case T_INT : fprintf(fp, "%d ", vs->value[i].inum);
                break;
        case T_CHAR:fprintf(fp, "%c ", vs->value[i].ch); 
                break;
        // T_FUN : fprintf(fp, "%s ", vs->value[i]->inum);
        //         break;
        // T_GRID :fprintf(fp, "%s ", vs->value[i]->inum); 
        //         break;
        case T_FLOAT: fprintf(fp, "%f ", vs->value[i].fnum);
                break;
        case T_BOOL: fprintf(fp, "%d ", vs->value[i].bool); 
                break;
      }
    }
    
}


void printSymTab(SymbolTable* st, FILE *fp,int headerFlag)
{
     int i;
     if(headerFlag)
     {
      fprintf(fp,"Variable Name Scope Offset Value \n");
      fprintf(fp,"------------- ----- ------ -----\n");
    }
     VarSymbol* vs = NULL;
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

void printFunTab(FILE *fp)
{ 
      fprintf(fp, "Function: main()\n");
      printSymTab(tables,fp,1);
      fprintf(fp,"\n");
      FunSymbol* fs=funs;
      while(fs!=NULL)
      {
        fprintf(fp, "Function: %s(",fs->name);
        for (int i = 0; i < fs->paramNum; ++i)
        {
          VarSymbol* vs=lookup_var_offset(fs->symbolTable,i);
          if(vs!=NULL)
          {
            if(i!=0)fprintf(fp, ",");
            fprintf(fp, "%s", vs->name );
          }
        }
        fprintf(fp,")\n");
        
        printSymTab(fs->symbolTable,fp,1);
        fprintf(fp,"\n\n");
        fs=fs->next;
      }
}

void write_table(const char* symbols_file, struct tree_node* syntax_tree) {
  FILE *fp = fopen(symbols_file, "w+");
  initTable();
  st_fill(syntax_tree,GLOBAL,tables, "main",fp);
  printFunTab(fp);
  fclose(fp);
}
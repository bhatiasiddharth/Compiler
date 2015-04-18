#include "analyzer.h"
#include "codegen.h"

//static Scope current_scope = GLOBAL; /* record current scope for variable declaration */
static struct fun_symbol* current_fun = NULL; /* which function's body are we in ?*/
static int current_scope=2;

int get_type(int symbol) {
    if(symbol == NUM || symbol == I32) return T_INT;
    if(symbol == STRL || symbol == STRING) return T_STR;
    if(symbol == CHARL || symbol == CHAR ) return T_CHAR;
    if(symbol == TRUE || symbol == FALSE || symbol == BOOL) return T_BOOL;
    if(symbol == FLOAT || symbol == F32) return T_FLOAT;
    // if(symbol == NUM) return T_GRID;
    // if(symbol == NUM) return T_ARRAY;
    return T_INT;
}

enum var_type getvar_type(char* var) {
  struct var_symbol* vs = lookup_var (var);
  return vs->type;
}

int is_arithop(int symbol) {
  if(symbol == MUL || symbol == PLUS || symbol == MINUS || symbol == DIV) return 1;
  return 0;
}

char* get_relop(int symbol) {
  static char op[4];
  if(symbol == LE) {
    strcpy(op, "jle");
    return op;
  }
  if(symbol == LT) {
    strcpy(op, "jl");
    return op;
  }
  if(symbol == GE) {
    strcpy(op, "jge");
    return op;
  }
  if(symbol == GT) {
    strcpy(op, "ja");
    return op;
  }
  if(symbol == EQ) {
    strcpy(op, "jae");
    return op;
  }
  if(symbol == NE) {
    strcpy(op, "jne");
    return op;
  }
  return NULL;
}
void eval_expn(struct tree_node* tr, int scope) {
    static int rel_count = 0;
    if(tr->children_count > 0) {
      eval_expn(tr->children[0], scope);
      eval_expn(tr->children[1], scope);
    }
    if(tr->symbol == ID) {
      codeseg_add("push %s_%d", tr->value.string, scope);
    }else if(tr->symbol == NUM) {
      codeseg_add("push dword ptr %d", tr->value.inum);
    }else if(tr->symbol == PLUS) {
      codeseg_add("pop eax  ; add");
      codeseg_add("pop ebx");
      codeseg_add("add eax, ebx");
      codeseg_add("push eax");
    }else if(tr->symbol == MINUS) {
      codeseg_add("pop ebx  ; subtract");
      codeseg_add("pop eax");
      codeseg_add("sub eax, ebx");
      codeseg_add("push eax");
    }else if(tr->symbol == MUL) {
      codeseg_add("pop eax  ; multiply");
      codeseg_add("pop ebx");
      codeseg_add("mul ebx");
      codeseg_add("push eax");
    }else if(tr->symbol == DIV) {
      codeseg_add("pop ebx  ; division");
      codeseg_add("pop eax");
      codeseg_add("div ebx");
      codeseg_add("push eax");
    }else if(tr->symbol == AND) {
      codeseg_add("pop ebx  ; and");
      codeseg_add("pop eax");
      codeseg_add("and eax, ebx");
      codeseg_add("push eax");
    }else if(tr->symbol == OR) {
      codeseg_add("pop ebx  ; or");
      codeseg_add("pop eax");
      codeseg_add("or eax, ebx");
      codeseg_add("push eax");
    }else if(tr->symbol == NOT) {
      codeseg_add("pop eax  ; not");
      codeseg_add("not eax");
      codeseg_add("push eax");
    }else if(tr->symbol == LT || tr->symbol == LE || tr->symbol == GT ||
             tr->symbol == GE || tr->symbol == EQ || tr->symbol == NE) {
     int relnum = rel_count;
      codeseg_add("pop ebx  ; LT");
      codeseg_add("pop eax");
      codeseg_add("cmp eax, ebx");
      codeseg_add("%s rel_true%d", get_relop(tr->symbol), relnum);
      codeseg_add("mov eax, 0");
      codeseg_add("jmp rel_end%d", relnum);
      codeseg_add("rel_true%d:", relnum);
      codeseg_add("mov eax,1");
      codeseg_add("rel_end%d:", relnum);
      codeseg_add("push eax");
      rel_count++;
    }
    //LE, EQ, GE , NE
    //LT GT
}

void declaration_stmt(struct tree_node* tr, int scope) {
  struct tree_node *assignop, *temp;
  enum var_type type;
  if(tr->children[1]->symbol == MUT){
      // id in 3rd branch
      assignop = tr->children[2];
  }else {
      // id in 2nd branch
      assignop = tr->children[1];
  }

  temp = assignop->children[0];
  if (temp->symbol == ID) {
      if(assignop->children[1]->symbol != array){
          // single declaration
          union value* value = (union value*) malloc(sizeof(union value));
          *value= assignop->children[1]->value;
          // codegen
          type = get_type(assignop->children[1]->symbol);

          if(is_arithop(assignop->children[1]->symbol)) {
            // print_symbol(stdout, assignop->children[1]->symbol, *value);
            eval_expn(assignop->children[1], scope);
            codeseg_add("pop eax");
            codeseg_add("mov %s_%d, eax", temp->value.string, scope);
            type = T_INT;
          }

          dataseg_add(temp->value.string, scope, type, value, 1);
          insert_var(temp->value.string, scope, tables->size++, type,value,1);
      }
      else {
          // array declaration
          temp = assignop->children[1];
          int size = temp->children_count;
          union value* value = (union value*) malloc(sizeof(union value) * (size - 1));
          type = get_type(temp->children[1]->symbol);
          for (int i = 1; i < size; ++i) {
              value[i-1] = temp->children[i]->value;
          }
          dataseg_add(assignop->children[0]->value.string, scope, type, value, size-1);
          insert_var(assignop->children[0]->value.string, scope, tables->size++, type, value, size-1);

      }
  }



  else {
      // multiple declarations
      if(temp->symbol == typeList)
      {
          for (int i = 0; i < temp->children_count; ++i)
          {
              union value* tvalue = (union value*) malloc(sizeof(union value));
              *tvalue = assignop->children[i+1]->value;
              type = get_type(assignop->children[i + 1]->symbol);
              dataseg_add(temp->children[i]->value.string, scope, type, tvalue, 1);
              insert_var(temp->children[i]->value.string, scope, tables->size++, type, tvalue,1);
          }
      }
  }
}

void single_assign_stmt(struct tree_node* tr, int scope) {
  struct tree_node* temp;
  if(tr->children[1]->symbol == ASSIGNOP)
  {
      struct tree_node* assignop = tr->children[1];
      temp = tr->children[0];
      if (temp->symbol == ID) {
          if(assignop->children[0]->symbol != array){
              // single assignment
              struct var_symbol* vs = lookup_var(temp->value.string);
              // todo: print error if symbol does not exist

              // code generate for single assignment
              // todo: check for types, lhs should not be array
              if(is_arithop(assignop->children[1]->symbol)) {
                eval_expn(assignop->children[1], scope);
                codeseg_add("mov %s_%d, eax", vs->name, vs->scope);
              }else if(assignop->children[1]->symbol == ID) {
                struct var_symbol* vs2 = lookup_var(assignop->children[1]->value.string);
                // todo: mov for string; check for types on both lhs and rhs
                codeseg_add("mov eax,  %s_%d", vs2->name, vs2->scope);
                codeseg_add("mov %s_%d, eax", vs->name, vs->scope);
              }else if(assignop->children[1]->symbol == NUM) {
                codeseg_add("push dword ptr %d", assignop->children[1]->value.inum);
                codeseg_add("pop eax");
                codeseg_add("mov %s_%d, eax", vs->name, vs->scope);
              }

              // function call case
              if(vs!=NULL)
                *(vs->value) = assignop->children[0]->value;

              else//function call with and without parameters
              {
                  struct fun_symbol* tempfun=lookup_fun(assignop->children[0]->value.string);
                  if(tempfun!=NULL)
                  {
                      //call function
                  }
                  else if(assignop->children[0]->symbol==relType) //for reltype
                  {

                      tempfun=lookup_fun(assignop->children[0]->children[0]->value.string);
                      if(tempfun!=NULL)
                      {
                          //call function in code gen
                          for (int i = 1; i < assignop->children[0]->children_count; ++i)
                          {
                              struct var_symbol* vs1=lookup_var_offset(tempfun->symbolTable,i-1);
                              *(vs1->value)=assignop->children[0]->children[i]->value;
                          }



                      }
                  }
              }
          }
          else {
              // array assignment

              //TODO: Check for size and type
              temp = assignop->children[0];
              int size = temp->children_count;

              struct var_symbol* vs=lookup_var (tr->children[0]->value.string);
              if(vs!=NULL)
              for (int i = 1; i < size; ++i)
              {

                  vs->value[i-1] = temp->children[i]->value;

              }

          }
      }

  }

  else//function call with no return type
  {

          struct fun_symbol* tempfunc=lookup_fun(tr->children[0]->value.string);
          if(tempfunc!=NULL)
          {
              //call function
              for (int i = 1; i < tr->children_count; ++i)
                  {
                      struct var_symbol* vs1=lookup_var_offset(tempfunc->symbolTable,i-1);
                      *(vs1->value)=tr->children[i]->value;
                  }

          }
  }

}

void multi_assign_stmt(struct tree_node* tr, int scope) {
  struct tree_node* temp;
  temp = tr->children[0];
   if(temp->symbol == typeList)
      {
          for (int i = 0; i < temp->children_count; ++i)
          {
              struct var_symbol* vs=lookup_var (temp->children[i]->value.string);
              //TODO: Check if already declared
              if(vs!=NULL)
              *(vs->value)=tr->children[i+1]->value;
          }
      }
}

void print_stmt(struct tree_node* tr, int scope) {
  static int strcount = 0;
  int type;
  int symbol = tr->children[1]->symbol;
  // if number,  float, charl, strl print directly
  if (symbol == NUM ) {
    codeseg_add("printnum %d", tr->children[1]->value.inum);
  }else if(symbol == STRL) {
    strcount++;
    char strname[MAX_LEN];
    sprintf(strname, "_printstr%d", strcount);
    dataseg_add(strname, scope, T_STR, &(tr->children[1]->value), 1);

    codeseg_add("printstr _printstr%d_%d", strcount, scope);
  }
  // if identifier - lookup in table and print value
  else if (symbol == ID) {
    //todo - check variable exists
    type = getvar_type(tr->children[1]->value.string);
    if (type == T_STR) {
      codeseg_add("printstr %s_%d", tr->children[1]->value.string, scope);
    }
    else if (type == T_INT) {
      codeseg_add("printnum %s_%d", tr->children[1]->value.string, scope);
    }
    else if (type == T_FLOAT) {}
    else if (type == T_BOOL) {
      codeseg_add("printbool %s_%d", tr->children[1]->value.string, scope);
    }
  }
}

void loop_stmt(struct tree_node* tr, int scope, struct symbol_table* tables, char* func_name, FILE* fp) {
  static int while_count = 0;
  int whilenum = while_count;
  while_count++;
  codeseg_add("while_begin%d:", whilenum);
  eval_expn(tr->children[1], scope);
  codeseg_add("cmp eax, 1");
  codeseg_add("jnz while_end%d", whilenum);
  st_fill(tr->children[2], scope, tables, func_name, fp);
  codeseg_add("jmp while_begin%d", whilenum);
  codeseg_add("while_end%d:", whilenum);


  // struct symbol_table* temp_table = new_symtable(++current_scope);
  // push_table(temp_table);
  // for (int i = 0; i < tr->children_count; i++) {
  //     st_fill(tr->children[i],current_scope,temp_table,func_name,fp);
  // }
  // struct symbol_table* copy_table=temp_table;
  // fprintf(fp, "While of %s()\n",func_name);
  // print_symtab(copy_table, fp, 1);
  // copy_table=copy_table->next;
  // while(copy_table!=NULL) {
  //   print_symtab(copy_table, fp,0);
  //   copy_table=copy_table->next;
  // }
  // fprintf(fp, "\n\n");
  // pop_table(temp_table);
}

void func_defn_stmt(struct tree_node* tr, int scope, FILE* fp) {
  struct symbol_table* temp_table = new_symtable(++current_scope);
  struct tree_node* temp;
  int type;
  insert_fun(tr->children[1]->value.string, temp_table, (tr->children[2]->children_count)/2 , T_FUN);

  push_table(temp_table);

  //for parameter list
  temp = tr->children[2];
   for (int i = 0; i < temp->children_count/2; i++) {
      union value* value = (union value*) malloc(sizeof(union value));
      //*value= 0;
      bzero(value, sizeof(union value));
      type = get_type(temp->children[2*i+1]->symbol);
      //later send value
      insert_var(temp->children[2*i]->value.string, current_scope, temp_table->size++, type,value,1);
  }

  for (int i = 0; i < tr->children_count; i++) {
      st_fill(tr->children[i],current_scope,temp_table,tr->children[1]->value.string,fp);
  }
  pop_table(temp_table);
}

void if_stmt(struct tree_node* tr, int scope, struct symbol_table* tables, char* func_name, FILE* fp) {
  static int if_count = 0;
  // codegen for boolean expn
  eval_expn(tr->children[0], scope);
  codeseg_add("cmp eax, 1");
  int labelnum = if_count;
  if_count++;
  codeseg_add("jnz if_begin%d", labelnum);
  // codegen for if block
  st_fill(tr->children[1], scope, tables, func_name, fp);
  codeseg_add("jmp if_end%d", labelnum);
  codeseg_add("if_begin%d:", labelnum);
  // else segment code
  if(tr->children[2] != NULL)
  st_fill(tr->children[2], scope, tables, func_name, fp);
  codeseg_add("if_end%d:", labelnum);

  // for (int i = 1; i < tr->children_count; i++) {
  //     struct symbol_table* temp_table = new_symtable(++current_scope);
  //     struct symbol_table* copy_table = temp_table;
  //     push_table(temp_table);
  //     st_fill(tr->children[i], current_scope, temp_table, func_name, fp);
  //
  //     fprintf(fp, "If Else of %s()\n",func_name);
  //     print_symtab(copy_table, fp,1);
  //     copy_table = copy_table->next;
  //
  //     while(copy_table != NULL) {
  //       print_symtab(copy_table, fp,0);
  //       copy_table = copy_table->next;
  //     }
  //     fprintf(fp, "\n\n");
  //     pop_table(temp_table);
  // }
}

void else_stmt(struct tree_node* tr, int scope, struct symbol_table* tables, char* func_name, FILE* fp) {
  st_fill(tr->children[0],scope,tables,func_name,fp);
  st_fill(tr->children[1],scope,tables,func_name,fp);

  struct symbol_table* temp_table= new_symtable(++current_scope);
  push_table(temp_table);
  st_fill(tr->children[2],current_scope,temp_table,func_name,fp);
  struct symbol_table* copy_table=temp_table;
  fprintf(fp, "Elseif of %s()\n",func_name);
  print_symtab(copy_table, fp,1);
  copy_table=copy_table->next;
  while(copy_table!=NULL)
  {
    print_symtab(copy_table, fp,0);
    copy_table=copy_table->next;
  }
  fprintf(fp, "\n\n");
  pop_table(temp_table);

}
void st_fill(struct tree_node* tr, int scope, struct symbol_table* tables,char* func_name,FILE* fp) {

    // single and multiple declaration statements
    if(tr->symbol == DeclarationStmt) {
      declaration_stmt(tr, scope);
      return;
    }

    // single assignment statement
    if(tr->symbol == Stmt) {
      single_assign_stmt(tr, scope);
      return;
    }

    // multiple assignment statemnt
    if(tr->symbol == ASSIGNOP) {
      multi_assign_stmt(tr, scope);
      return;
    }

    // loop stmt
    if(tr->symbol == LoopStmt) {
      loop_stmt(tr, scope, tables, func_name, fp);
      return;
    }

    // funcn defn stmt
    if(tr->symbol == FunctionDef) {
      func_defn_stmt(tr, scope, fp);
      return;
    }

    // if stmt
    if(tr->symbol == IfStmt) {
        if_stmt(tr, scope, tables, func_name, fp);
        return;
    }

    // else stmt
    // need only 1 more new scope
    if(tr->symbol == ElseStmt){
      if_stmt(tr, scope, tables, func_name, fp);
      return;
    }

    // print statements
    if(tr->symbol == OStmt) {
      print_stmt(tr, scope);
      return;
    }

    // no match
    for (int i = 0; i < tr->children_count; i++) {
        st_fill(tr->children[i],scope,tables,func_name,fp);
    }
}

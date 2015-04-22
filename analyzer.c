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
int eval_expn(struct tree_node* tr, int scope) {
    static int rel_count = 0;
    int type1,type2;
    if(tr->children_count > 0) {
      type1=eval_expn(tr->children[0], scope);
      type2=eval_expn(tr->children[1], scope);
      if(type1!=type2)
        {
            fprintf(stderr, "Types don't match\n");
            exit(1);
        }
    }
    if(tr->symbol == ID) {
      struct var_symbol* vs = lookup_var(tr->value.string);
      if(vs==NULL)
      {
        fprintf(stderr, "Variable: %s should be declared before use.\n", tr->value.string);
        exit(1);
      }
      codeseg_add("push %s_%d", tr->value.string, vs->scope);
      return vs->type;
    }else if(tr->symbol == NUM) {
      codeseg_add("push dword ptr %d", tr->value.inum);
      return T_INT;
    }else if(tr->symbol == FLOAT) {
      return T_FLOAT;
    }
    else if(tr->symbol == BOOL || tr->symbol == TRUE || tr->symbol == FALSE) {
      codeseg_add("push dword ptr %d", tr->value.bool);
      return T_BOOL;
    }


    else if(tr->symbol == PLUS) {
      codeseg_add("pop eax  ; add");
      codeseg_add("pop ebx");
      codeseg_add("add eax, ebx");
      codeseg_add("push eax");
      return type1;
    }else if(tr->symbol == MINUS) {
      codeseg_add("pop ebx  ; subtract");
      codeseg_add("pop eax");
      codeseg_add("sub eax, ebx");
      codeseg_add("push eax");
      return type1;
    }else if(tr->symbol == MUL) {
      codeseg_add("pop eax  ; multiply");
      codeseg_add("pop ebx");
      codeseg_add("mul ebx");
      codeseg_add("push eax");
      return type1;
    }else if(tr->symbol == DIV) {
      codeseg_add("pop ebx  ; division");
      codeseg_add("pop eax");
      codeseg_add("div ebx");
      codeseg_add("push eax");
      return type1;
    }

    


    else if(tr->symbol == AND) {
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
      codeseg_add("pop ebx  ; %s", token_names[tr->symbol]);
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
    return T_BOOL;
}

/*
  let x =5
  let x = [1,2, 3, 4];
  let x = "foo";

*/

void declaration_stmt(struct tree_node* tr, int scope) {
  struct tree_node *assignop, *temp;
  enum var_type type;
  int mutflag=0;
  if(tr->children[1]->symbol == MUT){
      // id in 3rd branch
    mutflag=1;
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
          // codegen
          type = get_type(assignop->children[1]->symbol);


          if(is_arithop(assignop->children[1]->symbol)) {
            // print_symbol(stdout, assignop->children[1]->symbol, *value);
            type = eval_expn(assignop->children[1], scope);
            value->inum = 0;
            // for T_INT
            codeseg_add("pop eax");
            codeseg_add("mov %s_%d, eax", temp->value.string, scope);
          }else if(assignop->children[1]->symbol == NUM) {
            value->inum = assignop->children[1]->value.inum;
           // data segment initialized
          }else if(assignop->children[1]->symbol == STRL) {
            strcpy(value->string, assignop->children[1]->value.string);
           // data segment initialized
          }else if(assignop->children[1]->symbol == TRUE || assignop->children[1]->symbol == FALSE) {
            value->bool = assignop->children[1]->value.bool;
           // data segment initialized
          }else if(assignop->children[1]->symbol == ID) {
            struct var_symbol* vs = lookup_var(assignop->children[1]->value.string);
             if(vs==NULL)
              {
                fprintf(stderr, "Variable: %s should be declared before use.\n", assignop->children[1]->value.string);
                exit(1);
              }
            type = vs->type;
            if (vs->type == T_INT) {
              value->inum = 0;
              codeseg_add("mov eax, %s_%d", vs->name, vs->scope);
              codeseg_add("mov %s_%d, eax", temp->value.string, scope);
            }if (vs->type == T_BOOL) {
              value->bool = 0;
              codeseg_add("mov eax, %s_%d", vs->name, vs->scope);
              codeseg_add("mov %s_%d, eax", temp->value.string, scope);
            }else if(vs->type == T_STR) {
              strcpy(value->string, "$");
              codeseg_add("strcpy %s_%d, %s_%d", temp->value.string, scope, vs->name, vs->scope);
            }
          }

          dataseg_add(temp->value.string, scope, type, value, 1);

          insert_var(temp->value.string, scope, tables->size++, type,value,1,mutflag);
      }
      else {
          // array declaration
          temp = assignop->children[1];

          if(temp->children[temp->children_count-1]->symbol!=moreAssn) 
          {
                  int size = temp->children_count;
                  union value* value = (union value*) malloc(sizeof(union value) * (size - 1));
                  type = get_type(temp->children[1]->symbol);

                for (int i = 1; i < size; ++i)
                {
                    if(get_type(temp->children[i]->symbol)!=type)
                    {
                        fprintf(stderr, "Array %s should have variables of same type.\n", assignop->children[0]->value.string);
                        exit(1);
                    }

                     value[i-1] = temp->children[i]->value;
                }
                dataseg_add(assignop->children[0]->value.string, scope, type, value, size-1);
                insert_var(assignop->children[0]->value.string, scope, tables->size++,type, value, size,mutflag); //size 1 more to indicate array or grid
          }
 
          else   //grid
          {
                  int size = temp->children_count-2 + temp->children[temp->children_count-1]->children_count-1;
                  union value* value = (union value*) malloc(sizeof(union value) * (size));
                  type = get_type(temp->children[1]->symbol);

                  int i;
                for (i = 1; i < temp->children_count-1; ++i)
                {
                    if(temp->children[i]->symbol == ID)
                      {
                            struct var_symbol* vs4 = lookup_var(temp->children[i]->value.string);
                            if(vs4==NULL)
                              {
                                fprintf(stderr, "Variable: %s should be declared before use.\n", temp->children[i]->value.string);
                                exit(1);
                              }
          
                      }

                    if(get_type(temp->children[i]->symbol)!=type)
                    {
                        fprintf(stderr, "Grid %s should have variables of same type.\n", assignop->children[0]->value.string);
                        exit(1);
                    }
                     value[i-1] = temp->children[i]->value;
                }
                int j=1;
                for (;i<=size;++i)
                {
                    if(temp->children[temp->children_count-1]->children[j]->symbol == ID)
                      {
                            struct var_symbol* vs4 = lookup_var(temp->children[temp->children_count-1]->children[j]->value.string);
                            if(vs4==NULL)
                              {
                                fprintf(stderr, "Variable: %s should be declared before use.\n", temp->children[temp->children_count-1]->children[j]->value.string);
                                exit(1);
                              }
          
                      }

                    if(get_type(temp->children[temp->children_count-1]->children[j]->symbol)!=type)
                    {
                        fprintf(stderr, "Grid %s should have variables of same type.\n", assignop->children[0]->value.string);
                        exit(1);
                    }
                    value[i-1] = temp->children[temp->children_count-1]->children[j++]->value;
                }


                dataseg_add(assignop->children[0]->value.string, scope, type, value, size);
                insert_var(assignop->children[0]->value.string, scope, tables->size++, type, value, size+1,mutflag);    //size 1 more to indicate array or grid
          }

      }
  }



  else {
      // multiple declarations
      if(temp->symbol == typeList) {
          for (int i = 0; i < temp->children_count; ++i) {
              union value* tvalue = (union value*) malloc(sizeof(union value));
              *tvalue = assignop->children[i+1]->value;
              type = get_type(assignop->children[i + 1]->symbol);

              if(assignop->children[i+1]->symbol == ID)
              {
                    struct var_symbol* vs4 = lookup_var(assignop->children[i+1]->value.string);
                    if(vs4==NULL)
                      {
                        fprintf(stderr, "Variable: %s should be declared before use.\n", assignop->children[i+1]->value.string);
                        exit(1);
                      }
  
              }
              
              dataseg_add(temp->children[i]->value.string, scope, type, tvalue, 1);
              insert_var(temp->children[i]->value.string, scope, tables->size++, type, tvalue,1,mutflag);
          }
      }
  }
}

void single_assign_stmt(struct tree_node* tr, int scope) {
  struct tree_node* temp;
  if(tr->children[1]->symbol == ASSIGNOP)
  {
      struct tree_node* assignop = tr->children[1];
      struct tree_node* returnvar = tr->children[0];
      temp = tr->children[0];
      if (temp->symbol == ID)
      {
           

            if(assignop->children[0]->symbol != array) //not array assignment
            {

                // single assignment
                struct var_symbol* vs = lookup_var(temp->value.string);
                if(vs==NULL)
                {
                    fprintf(stderr, "Variable: %s should be declared before use.\n", temp->value.string);
                    exit(1);
                }

                if(!vs->mutable)
                {
                     fprintf(stderr, "Variable: %s must be mutable.\n",temp->value.string);
                    exit(1); 
                }

                if(vs->size>1)
                {
                    fprintf(stderr, "Variable: %s should be assigned to an array.\n", temp->value.string);
                    exit(1);
                }
                  
                  // code generate for single assignment
                if(is_arithop(assignop->children[1]->symbol))
                {

                    int type=eval_expn(assignop->children[1], scope);
                    if(vs->type!=type)
                    {
                        fprintf(stderr, "Type of %s does not match type of RHS.\n", vs->name);
                            exit(1);
                    }
                    codeseg_add("push eax");
                }

                else if(assignop->children[1]->symbol == ID)
                {
                    struct var_symbol* vs2 = lookup_var(assignop->children[1]->value.string);
                    if(vs2==NULL)
                    {
                      struct fun_symbol* tempfun=lookup_fun(assignop->children[1]->value.string);
                      if(tempfun==NULL)
                      {
                        //of form x= add(); or x=y;
                         fprintf(stderr, "Variable: %s should be declared before use.\n", assignop->children[1]->value.string);
                          exit(1);
                      }

                    }

                    if(vs2!=NULL)
                    {
                        if(vs2->type!=vs->type)
                        {
                            fprintf(stderr, "Type of %s does not match type of %s.\n", vs->name, vs2->name);
                            exit(1);
                        }
                    }

                    if(vs->type == T_INT || vs->type == T_BOOL) {
                      codeseg_add("push dword ptr %s_%d", vs2->name, vs2->scope);
                    }else if(vs->type == T_STR) {
                      codeseg_add("strcpy %s_%d, %s_%d", vs->name, vs->scope, vs2->name, vs2->scope);
                    }
                }

                else if(assignop->children[1]->symbol == NUM)
                {
                    if(vs->type!=get_type(assignop->children[1]->symbol))
                    {
                        fprintf(stderr, "Type of %s does not match type of RHS.\n", vs->name);
                        exit(1);
                    }
                    codeseg_add("push dword ptr %d", assignop->children[1]->value.inum);
                }

                else if(assignop->children[1]->symbol == FLOAT || assignop->children[1]->symbol == CHARL || assignop->children[1]->symbol == STRL || assignop->children[1]->symbol == TRUE || assignop->children[1]->symbol == FALSE || assignop->children[1]->symbol == BOOL)
                {
                    if(vs->type!=get_type(assignop->children[1]->symbol))
                    {
                        fprintf(stderr, "Type of %s does not match type of RHS.\n", vs->name);
                        exit(1);
                    }
                }
                  
                  // function call case
                else if(assignop->children[0]->symbol==relType)
                {
                    *(vs->value) = assignop->children[0]->value;

                    struct fun_symbol* tempfun=lookup_fun(assignop->children[0]->children[0]->value.string);
                    if(assignop->children[0]->children[1]->symbol==MethodCall) //of form x= a.add(); or x= a.add(2,4);
                    {
                      struct fun_symbol* tempfun=lookup_fun(assignop->children[0]->children[1]->children[0]->value.string);
                      if(tempfun==NULL)
                      {
                          fprintf(stderr, "Function: %s should be declared before use.\n",assignop->children[0]->children[1]->children[0]->value.string);
                          exit(1);
                      }

                      if(tempfun->param_num!=assignop->children[0]->children[1]->children_count-1)
                      {
                        fprintf(stderr, "Function: %s mismatch parameter count.\n",assignop->children[0]->children[1]->children[0]->value.string);
                        exit(1);
                      }


                      for (int i = 0; i < tempfun->param_num; ++i)
                      {
                          struct var_symbol* vs=lookup_var_offset(tempfun->symbolTable,i);
                          if(vs!=NULL)
                          {

                            struct var_symbol* vs2 = lookup_var (assignop->children[0]->children[1]->children[i+1]->value.string);
                            if(vs2==NULL)
                            {
                                if( get_type(assignop->children[0]->children[1]->children[i+1]->symbol) != vs->type)
                                {
                                  fprintf(stderr, "Function: %s type mismatch for parameter %d.\n",assignop->children[0]->children[1]->children[0]->value.string,i+1);
                                   exit(1);
                                }
                            }
                            else
                            {
                                if( vs2->type != vs->type)
                                {
                                  fprintf(stderr, "Function: %s type mismatch for parameter %d.\n",assignop->children[0]->children[1]->children[0]->value.string,i+1);
                                   exit(1);
                                }
                            }
                          }
                      }

                       struct var_symbol* vs3 = lookup_var (returnvar->value.string);
                      if(vs3->type != tempfun->type)
                      {
                        fprintf(stderr, "Function: %s return type mismatch.\n",assignop->children[0]->children[1]->children[0]->value.string);
                          exit(1);
                      }
                    }

                    else if(assignop->children[0]->children[1]->symbol == OSQUARE) //min = arr[0]
                    {
                          // get array identifier
                          struct var_symbol* vs2 = lookup_var(assignop->children[1]->children[0]->value.string);
                          if(vs2 != NULL){
                            // only for numbers right now
                            codeseg_add("lea si,  %s_%d ; get array index", vs2->name, vs2->scope);
                            codeseg_add("add si,  %d", 4*assignop->children[1]->children[2]->value.inum);
                            codeseg_add("push dword ptr [si]", vs2->name, vs2->scope);
                          }
                    }

                    else   //of form x= add(3,4);
                    {
                      struct fun_symbol* tempfun=lookup_fun(assignop->children[0]->children[0]->value.string);
                      if(tempfun==NULL)
                      {
                          fprintf(stderr, "Function: %s should be declared before use.\n",assignop->children[0]->children[0]->value.string);
                          exit(1);
                      }
                      if(tempfun->param_num!=assignop->children[0]->children_count-1)
                      {
                        fprintf(stderr, "Function: %s mismatch parameter count.\n",assignop->children[0]->children[0]->value.string);
                        exit(1);
                      }



                      for (int i = 0; i < tempfun->param_num; ++i)
                      {
                          struct var_symbol* vs=lookup_var_offset(tempfun->symbolTable,i);
                          if(vs!=NULL)
                          {

                            struct var_symbol* vs2 = lookup_var (assignop->children[0]->children[i+1]->value.string);
                            if(vs2==NULL)
                            {
                                if( get_type(assignop->children[0]->children[i+1]->symbol) != vs->type)
                                {
                                  fprintf(stderr, "Function: %s type mismatch for parameter %d.\n",assignop->children[0]->children[0]->value.string,i+1);
                                   exit(1);
                                }
                            }
                            else
                            {
                                if( vs2->type != vs->type)
                                {
                                  fprintf(stderr, "Function: %s type mismatch for parameter %d.\n",assignop->children[0]->children[0]->value.string,i+1);
                                   exit(1);
                                }
                            }
                          }
                      }

                       struct var_symbol* vs3 = lookup_var (returnvar->value.string);
                      if(vs3->type != tempfun->type)
                      {
                        fprintf(stderr, "Function: %s return type mismatch.\n",assignop->children[0]->children[0]->value.string);
                          exit(1);
                      }
                    }
                }

                if(temp->symbol == ID)
                {
                    if(vs->type == T_INT)
                    {
                      codeseg_add("pop eax");
                      codeseg_add("mov %s_%d, eax", vs->name, vs->scope);
                    }
                } 
            }

            else //array assignment x=[1,2,3] 
            {


                struct var_symbol* vs2 = lookup_var(temp->value.string);

                if(!vs2->mutable)
                {
                     fprintf(stderr, "Variable: %s must be mutable.\n",temp->value.string);
                    exit(1); 
                }

                if(vs2->size==1)
                {
                    fprintf(stderr, "Variable: %s cannot be assigned to array.\n", temp->value.string);
                    exit(1);
                }

              temp = assignop->children[0];
              int size = temp->children_count;

              struct var_symbol* vs=lookup_var (tr->children[0]->value.string);
              if(vs==NULL)
              {
                fprintf(stderr, "Variable: %s should be declared before use.\n", tr->children[0]->value.string);
                exit(1);
              }
              if(vs!=NULL)
              for (int i = 1; i < size; ++i)
              {

                  vs->value[i-1] = temp->children[i]->value;

              }

            }
      }

  }


  // of the form a.add()
  else
  {
          //of form a.add() or a.add(2,3)
          if(tr->children[1]->symbol==MethodCall)
          {
              struct fun_symbol* tempfunc=lookup_fun(tr->children[1]->children[0]->value.string);
              if(tempfunc==NULL)
              {
                fprintf(stderr, "Function: %s should be declared before use.\n",tr->children[1]->children[0]->value.string);
                exit(1);
              }
              if(tempfunc->param_num!=tr->children[1]->children_count-1)
              {
                fprintf(stderr, "Function: %s mismatch parameter count.\n",tr->children[1]->children[0]->value.string);
                exit(1);
              }

            for (int i = 0; i < tempfunc->param_num; ++i)
              {
                  struct var_symbol* vs=lookup_var_offset(tempfunc->symbolTable,i);
                  if(vs!=NULL)
                  {

                    struct var_symbol* vs2 = lookup_var (tr->children[1]->children[i+1]->value.string);
                    if(vs2==NULL)
                    {
                        if( get_type(tr->children[1]->children[i+1]->symbol) != vs->type)
                        {
                          fprintf(stderr, "Function: %s type mismatch for parameter %d.\n",tr->children[1]->children[0]->value.string,i+1);
                           exit(1);
                        }
                    }
                    else
                    {
                        if( vs2->type != vs->type)
                        {
                          fprintf(stderr, "Function: %s type mismatch for parameter %d.\n",tr->children[1]->children[0]->value.string,i+1);
                           exit(1);
                        }
                    }
                  }
              }




          }


          //of form add(2,3)
          else
          {
              struct fun_symbol* tempfunc=lookup_fun(tr->children[0]->value.string);
              if(tempfunc==NULL)
              {
                fprintf(stderr, "Function: %s should be declared before use.\n",tr->children[0]->value.string);
                exit(1);
              }

              if(tempfunc->param_num!=tr->children_count-1)
              {
                fprintf(stderr, "Function: %s mismatch parameter count.\n",tr->children[0]->value.string);
                exit(1);
              }

              for (int i = 0; i < tempfunc->param_num; ++i)
              {
                  struct var_symbol* vs=lookup_var_offset(tempfunc->symbolTable,i);
                  if(vs!=NULL)
                  {

                    struct var_symbol* vs2 = lookup_var (tr->children[i+1]->value.string);
                    if(vs2==NULL)
                    {
                        if( get_type(tr->children[i+1]->symbol) != vs->type)
                        {
                          fprintf(stderr, "Function: %s type mismatch for parameter %d.\n",tr->children[0]->value.string,i+1);
                           exit(1);
                        }
                    }
                    else
                    {
                        if( vs2->type != vs->type)
                        {
                          fprintf(stderr, "Function: %s type mismatch for parameter %d.\n",tr->children[0]->value.string,i+1);
                           exit(1);
                        }
                    }
                  }
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
              if(vs==NULL)
              {
                fprintf(stderr, "Variable: %s should be declared before use.\n", temp->children[i]->value.string);
                exit(1);
              }
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
    struct var_symbol* vs = lookup_var (tr->children[1]->value.string);
    if(vs==NULL)
    {
      fprintf(stderr, "Variable: %s should be declared before use.\n", tr->children[1]->value.string);
      exit(1);
    }
    if (vs->type == T_STR) {
      codeseg_add("printstr %s_%d", vs->name, vs->scope);
    }
    else if (vs->type == T_INT) {
      codeseg_add("printnum %s_%d", vs->name, vs->scope);
    }
    else if (vs->type == T_FLOAT) {}
    else if (vs->type == T_BOOL) {
      codeseg_add("printbool %s_%d", vs->name, vs->scope);
    }
  }
  // print array index
  else if (symbol == relType && tr->children[1]->children[1]->symbol == OSQUARE) {
      struct var_symbol* vs = lookup_var (tr->children[1]->children[0]->value.string);
      if(vs==NULL)
    {
      fprintf(stderr, "Variable: %s should be declared before use.\n", tr->children[1]->children[0]->value.string);
      exit(1);
    }
      struct tree_node* index = tr->children[1]->children[2];
      int offset;
      codeseg_add("lea si, %s_%d", vs->name, vs->scope);
      if(index->symbol == NUM) {
        codeseg_add("mov eax, %d", index->value.inum);
      }else if(index->symbol == ID) {
          struct var_symbol* vs2 = lookup_var (index->value.string);
          if(vs2->type != T_INT) {
              fprintf(stderr, "Array index %s must be of type integer\n", index->value.string);
              exit(1);
          }
          codeseg_add("mov eax, %s_%d", vs2->name, vs2->scope);
      }
      if(vs->type == T_INT || vs->type == T_BOOL) {
          codeseg_add("mov cx, 4");
          codeseg_add("mul cx");
          codeseg_add("add si, ax");
      } else if(vs->type == T_STR) {
          codeseg_add("mov cx, 80");
          codeseg_add("mul cx");
          codeseg_add("add si, ax");
      }
      if (vs->type == T_STR) {
        codeseg_add("printstr [si]");
      }
      else if (vs->type == T_INT) {
        codeseg_add("printnum [si]");
      }
      else if (vs->type == T_FLOAT) {}
      else if (vs->type == T_BOOL) {
        codeseg_add("printbool [si]");
      }
    }
}

void scan_stmt(struct tree_node* tr, int scope) {
  static int strcount = 0;
  int type;
  int symbol = tr->children[1]->symbol;

  if(symbol == ID) {
    struct var_symbol* vs = lookup_var(tr->children[1]->value.string);
    if(vs != NULL) {
      if(vs->type == T_INT) {
        // scan int
        codeseg_add("scannum %s_%d", tr->children[1]->value.string, vs->scope);
      }else if(vs->type == T_STR) {
        // scan num
        codeseg_add("scanstr %s_%d", tr->children[1]->value.string, vs->scope);
      }

    }else {
      fprintf(stderr, "Variable: %s should be declared before use.\n", tr->children[1]->value.string);
      exit(1);
    }

  }else {
    // todo: scan only ID
  }
}
void loop_stmt(struct tree_node* tr, int scope, struct symbol_table* tables, char* func_name, FILE* fp) {
  static int while_count = 0;
  int whilenum = while_count;
  while_count++;
  codeseg_add("while_begin%d:", whilenum);
  int type=eval_expn(tr->children[1], scope);
  if(type!=T_BOOL)
  {
    fprintf(stderr, "Boolean Expression for loop condition.\n");
    exit(1);
  }
  codeseg_add("cmp eax, 1");
  codeseg_add("jnz while_end%d", whilenum);

  struct symbol_table* temp_table = new_symtable(++current_scope);
  push_table(temp_table);

  st_fill(tr->children[2], current_scope, temp_table, func_name,fp);

  struct symbol_table* copy_table=temp_table;
  fprintf(fp, "While of %s()\n",func_name);
  print_symtab(copy_table, fp, 1);
  copy_table=copy_table->next;
  while(copy_table!=NULL) {
    print_symtab(copy_table, fp,0);
    copy_table=copy_table->next;
  }
  fprintf(fp, "\n\n");
  pop_table(temp_table);

  codeseg_add("jmp while_begin%d", whilenum);
  codeseg_add("while_end%d:", whilenum);
}

void func_defn_stmt(struct tree_node* tr, int scope, FILE* fp) {
  struct symbol_table* temp_table = new_symtable(++current_scope);
  struct tree_node* temp;
  int type;

if(tr->children[3]->symbol==Statements)
  type = -1;

else
  type=get_type(tr->children[3]->symbol);

  insert_fun(tr->children[1]->value.string, temp_table, (tr->children[2]->children_count)/2 , type);

  push_table(temp_table);

  //for parameter list
  temp = tr->children[2];
   for (int i = 0; i < temp->children_count/2; i++) {
      union value* value = (union value*) malloc(sizeof(union value));
      //*value= 0;
      bzero(value, sizeof(union value));
      type = get_type(temp->children[2*i+1]->symbol);
      //later send value
      insert_var(temp->children[2*i]->value.string, current_scope, temp_table->size++, type,value,1,0);
  }

  for (int i = 0; i < tr->children_count; i++) {
      st_fill(tr->children[i],current_scope,temp_table,tr->children[1]->value.string,fp);
  }
  pop_table(temp_table);
}

void ifelse_stmt(struct tree_node* tr, int scope, struct symbol_table* tables, char* func_name, FILE* fp) {
  static int if_count = 0;
  // codegen for boolean expn
  int type=eval_expn(tr->children[0], scope);
  if(type!=T_BOOL)
  {
    fprintf(stderr, "Boolean Expression for if condition.\n");
    exit(1);
  }

  codeseg_add("cmp eax, 1");
  int labelnum = if_count;
  if_count++;
  codeseg_add("jnz if_begin%d", labelnum);

  for (int i = 1; i < tr->children_count; i++) {
      struct symbol_table* temp_table = new_symtable(++current_scope);
      struct symbol_table* copy_table = temp_table;
      push_table(temp_table);

      st_fill(tr->children[i], current_scope, temp_table, func_name, fp);
      if(i == 1) {
        // codegen for if block
        codeseg_add("jmp if_end%d", labelnum);
        codeseg_add("if_begin%d:", labelnum);
      }
      fprintf(fp, "If Else of %s()\n",func_name);
      print_symtab(copy_table, fp,1);
      copy_table = copy_table->next;

      while(copy_table != NULL) {
        print_symtab(copy_table, fp,0);
        copy_table = copy_table->next;
      }
      fprintf(fp, "\n\n");
      pop_table(temp_table);
  }
  codeseg_add("if_end%d:", labelnum);
}

void else_stmt(struct tree_node* tr, int scope, struct symbol_table* tables, char* func_name, FILE* fp) {
  st_fill(tr->children[0],scope,tables,func_name,fp);
  st_fill(tr->children[1],scope,tables,func_name,fp);

  struct symbol_table* temp_table = new_symtable(++current_scope);
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

        ifelse_stmt(tr, scope, tables, func_name, fp);
        return;
    }

    // else stmt
    // need only 1 more new scope
    if(tr->symbol == ElseStmt){

      ifelse_stmt(tr, scope, tables, func_name, fp);
      return;
    }

    // print statements
    if(tr->symbol == OStmt) {

      print_stmt(tr, scope);
      return;
    }

    // print statements
    if(tr->symbol == IStmt) {

      scan_stmt(tr, scope);
      return;
    }


    // no match
    for (int i = 0; i < tr->children_count; i++) {
        st_fill(tr->children[i],scope,tables,func_name,fp);
    }
}

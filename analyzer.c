#include "analyzer.h"
#include "codegen.h"

//static Scope current_scope = GLOBAL; /* record current scope for variable declaration */
static struct fun_symbol* current_fun = NULL; /* which function's body are we in ?*/
static int current_scope=2;

enum var_type get_type(int symbol) {
    if(symbol == NUM || symbol ==I32) return T_INT;
    if(symbol == STRL || symbol == STRING) return T_STR;
    if(symbol == CHARL || symbol == CHAR ) return T_CHAR;
    if(symbol == TRUE || symbol == FALSE || symbol == BOOL) return T_BOOL;
    if(symbol == FLOAT || symbol ==F32) return T_FLOAT;
    // if(symbol == NUM) return T_GRID;
    // if(symbol == NUM) return T_ARRAY;


}

enum var_type getvar_type(char* var) {
  struct var_symbol* vs = lookup_var (var);
  return vs->type;
}

int is_arithop(int symbol) {
  if(symbol == MUL || symbol == PLUS || symbol == MINUS || symbol == DIV) return 1;
  return 0;
}

void eval_expn(struct tree_node* tr, int scope) {
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
    }
}

void st_fill(struct tree_node* tr, int scope, struct symbol_table* tables,char* func_name,FILE* fp) {
    int i;
    int n = tr->children_count;
    struct tree_node* temp;
    union value tvalue;
    enum var_type type;
    if(tr->symbol == DeclarationStmt)   //declaration statement
    {
        struct tree_node* assignop;
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

                dataseg_add(temp->value.string, scope, type, value);
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
                //dataseg_add(temp->value.string, scope, type, value);
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
                    dataseg_add(temp->children[i]->value.string, scope, type, tvalue);

                    printf("> %s %d \n", temp->children[i]->value.string, scope);
                    insert_var(temp->children[i]->value.string, scope, tables->size++, type, tvalue,1);
                }
            }
        }
    }


    else if(tr->symbol == Stmt) { // assignment statement
        if(tr->children[1]->symbol==ASSIGNOP)
        {

            struct tree_node* assignop=tr->children[1];
            temp = tr->children[0];
            if (temp->symbol == ID) {
                if(assignop->children[0]->symbol != array){
                    // single assignment

                    struct var_symbol* vs=lookup_var (temp->value.string);
                    if(vs!=NULL)
                    *(vs->value)=assignop->children[0]->value;

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


    else if(tr->symbol == ASSIGNOP) {   //multiple assignment
        temp=tr->children[0];
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



    else if(tr->symbol == LoopStmt)
    {
        struct symbol_table* temp_table= new_symtable(++current_scope);
        push_table(temp_table);
        for (int i = 0; i < tr->children_count; ++i)
        {
            st_fill(tr->children[i],current_scope,temp_table,func_name,fp);
        }
        struct symbol_table* copy_table=temp_table;
        fprintf(fp, "While of %s()\n",func_name);
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




    else if(tr->symbol == FunctionDef)
    {
        struct symbol_table* temp_table= new_symtable(++current_scope);

        insert_fun(tr->children[1]->value.string, temp_table, (tr->children[2]->children_count)/2 , T_FUN);

        push_table(temp_table);


        //for parameter list
        temp=tr->children[2];
         for (int i = 0; i < temp->children_count/2; ++i)
        {
            union value* value = (union value*) malloc(sizeof(union value));
                //*value= 0;
                bzero(value, sizeof(union value));
                type = get_type(temp->children[2*i+1]->symbol);
                //later send value
                insert_var(temp->children[2*i]->value.string, current_scope, temp_table->size++, type,value,1);

        }


        for (int i = 0; i < tr->children_count; ++i)
        {
            st_fill(tr->children[i],current_scope,temp_table,tr->children[1]->value.string,fp);
        }
        pop_table(temp_table);
    }




    else if(tr->symbol == IfStmt)
    {
        st_fill(tr->children[0],scope,tables,func_name,fp);

        for (int i = 1; i < tr->children_count; ++i)
        {
            struct symbol_table* temp_table= new_symtable(++current_scope);
            push_table(temp_table);
            st_fill(tr->children[i],current_scope,temp_table,func_name,fp);

            struct symbol_table* copy_table=temp_table;
            fprintf(fp, "If Else of %s()\n",func_name);
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
    }


    else if(tr->symbol == ElseStmt) // need only 1 more new scope
    {
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
    else if(tr->symbol == OStmt) {
      int symbol = tr->children[1]->symbol;
      // if number,  float, charl, strl print directly
      if (symbol == NUM || symbol == FLOAT || symbol == STRL || symbol == TRUE || symbol == FALSE) {
        // dataseg_add
        // codeseg_add
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
  }else if(is_arithop(tr->symbol) || tr->symbol == NUM) {
    printf("nothing\n");
  } else {
        for (int i = 0; i < tr->children_count; ++i)
        {
            st_fill(tr->children[i],scope,tables,func_name,fp);
        }
    }
}

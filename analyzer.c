#include "analyzer.h"


//static Scope current_scope = GLOBAL; /* record current scope for variable declaration */
static FunSymbol* current_fun = NULL; /* which function's body are we in ?*/
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
void st_fill(struct tree_node* tr, int scope, SymbolTable* tables) {
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
                type = get_type(assignop->children[1]->symbol);
                insert_var(temp->value.string, scope, tables->size++, type,value,1);
            }
            else {
                // array declaration
                temp = assignop->children[1];
                int size = temp->children_count;
                union value* value = (union value*) malloc(sizeof(union value) * (size - 1));
                type = get_type(temp->children[1]->symbol);
                for (int i = 1; i < size; ++i)
                {
                    
                    value[i-1] = temp->children[i]->value;
                    
                }
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
                    insert_var(temp->children[i]->value.string, scope, tables->size++, type, tvalue,1);
                }
            }
        }
    }


    else if(tr->symbol == Stmt) {
        if(tr->children[1]->symbol==ASSIGNOP)
        {

            struct tree_node* assignop=tr->children[1];
            temp = tr->children[0];
            if (temp->symbol == ID) {
                if(assignop->children[0]->symbol != array){
                    // single assignment

                    VarSymbol* vs=lookup_var (temp->value.string);
                    *(vs->value)=assignop->children[0]->value;
                }
                else {
                    // array assignment

                    //TODO: Check for size and type
                    temp = assignop->children[0];
                    int size = temp->children_count;

                    VarSymbol* vs=lookup_var (tr->children[0]->value.string);
                    for (int i = 1; i < size; ++i)
                    {
                        
                        //vs->value[i-1] = temp->children[i]->value;
                        
                    }

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
                    VarSymbol* vs=lookup_var (temp->children[i]->value.string);
                    //TODO: Check if already declared
                    if(vs!=NULL)
                    *(vs->value)=tr->children[i+1]->value;
                }
            }
    }


    
    else if(tr->symbol == LoopStmt)
    {
        SymbolTable* temp_table= newSymbolTable(++current_scope);
        pushTable(temp_table);
        for (int i = 0; i < tr->children_count; ++i)
        {
            st_fill(tr->children[i],current_scope,temp_table);
        }
        printSymTab(temp_table, stdout);
        popTable(temp_table);
    }




    else if(tr->symbol == FunctionDef)
    {
        SymbolTable* temp_table= newSymbolTable(++current_scope);

        insert_fun(tr->children[1]->value.string, temp_table, (tr->children[2]->children_count)/2 , T_FUN);

        pushTable(temp_table);

        /*
        //for parameter list
        temp=tr->children[2];
         for (int i = 0; i < temp->children_count/2; ++i)
        {
            union value* value = (union value*) malloc(sizeof(union value));
                *value= assignop->children[1]->value;
                type = get_type(temp->children[2*i+1]->symbol);
                //later send value
                insert_var(temp->children[2*i]->value.string, current_scope, temp_table->size++, type,NULL,1);

        }
        */

        for (int i = 0; i < tr->children_count; ++i)
        {
            st_fill(tr->children[i],current_scope,temp_table);
        }
        popTable(temp_table);
    } 




    else if(tr->symbol == IfStmt)
    {
        st_fill(tr->children[0],scope,tables);
        
        for (int i = 1; i < tr->children_count; ++i)
        {
            SymbolTable* temp_table= newSymbolTable(++current_scope);
            pushTable(temp_table);
            st_fill(tr->children[i],current_scope,temp_table);
            printSymTab(temp_table, stdout);
            popTable(temp_table);
        }
    }



    else {
        for (int i = 0; i < tr->children_count; ++i)
        {
            st_fill(tr->children[i],scope,tables);
        }
    }
}
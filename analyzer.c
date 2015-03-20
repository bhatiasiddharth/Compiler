#include "analyzer.h"


static Scope current_scope = GLOBAL; /* record current scope for variable declaration */
static FunSymbol* current_fun = NULL; /* which function's body are we in ?*/

enum var_type get_type(int symbol) {
    if(symbol == NUM) return T_INT;
    if(symbol == STRL) return T_STR; 
    if(symbol == CHARL) return T_CHAR;
    if(symbol == TRUE || symbol == FALSE) return T_BOOL; 
    if(symbol == FLOAT) return T_FLOAT; 
    // if(symbol == NUM) return T_GRID;
    // if(symbol == NUM) return T_ARRAY;


}
void st_fill(struct tree_node* tr) {
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
                // single assignment
                type = get_type(assignop->children[1]->symbol);
                insert_var(temp->value.string, GLOBAL, tables->size++, type, assignop->children[1]->value);
            }else {
                // array init
                temp = assignop->children[1];
                int size = temp->children_count;
                union value value = (union value*) malloc(sizeof(union value) * (size - 1));
                for (int i = 1; i < size; ++i)
                {
                    type = get_type(temp->children[i]->symbol);
                    value[i - 1] = temp->children[i]->value;
                    insert_var(assignop->children[0]->value.string, GLOBAL, tables->size++, type, temp->children[i]->value);
                }

            }
        }



        else {
            // multiple assignments
            if(temp->symbol == typeList)
            {
                for (int i = 0; i < temp->children_count; ++i)
                {
                    tvalue = assignop->children[i+1]->value;
                    type = get_type(assignop->children[i + 1]->symbol);
                    insert_var(temp->children[i]->value.string, GLOBAL, tables->size++, type, tvalue);
                }
            }
        }
    }else if(tr->symbol == Stmt) {
        // left child is ID
        
    }else {
        for (int i = 0; i < tr->children_count; ++i)
        {
            st_fill(tr->children[i]);
        }
    }
}
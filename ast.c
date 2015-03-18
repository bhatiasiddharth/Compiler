#include "ast.h"

struct tree_node* removeTerm(struct tree_node* tr) 
{
     int i=0;
     int newchildren_count = tr->children_count;

     while(i < newchildren_count)
     {  
           int v = tr->children[i]->symbol;
           int c = tr->children[i]->children_count;
       
        if((v < 100)||(v>=100 && c==0) ) 
        { 

           if( v<=USELESS_END ||(v >= Program && c==0) || v==IF || v==ELSE || v==ELSEIF)
           {
            int j;
            for(j=i;j<newchildren_count-1;j++)
            {
                tr->children[j] = tr->children[j+1];
            }
            newchildren_count--;
            i--;
           }
           i++;
        }
       else
       {
            struct tree_node* tr1 = removeTerm(tr->children[i]);
            i++;
       }
     }
     tr->children_count = newchildren_count;
  return tr;
}

struct tree_node* remove_Chaining(struct tree_node* tr)
{
    if(tr->children_count ==1 && tr->symbol>=100)
    {
        struct tree_node* ptr = tr->children[0];

        tr->symbol = ptr->symbol;
        tr->children_count = ptr->children_count;
        int i =0;
        int c = ptr->children_count;

        for(i=0;i<c;i++)
        {
            tr->children[i] = ptr->children[i];
        }

        remove_Chaining(tr);
    }

        if(tr->children_count > 1 ) //not necassarily a non-terminal
        {
            int i =0;
            int n = tr->children_count;
            while(i < n)
            {
                if(tr->children[i]->children_count > 0) 
                    remove_Chaining(tr->children[i]);
                i++;
            }
        }
    return tr;
}



struct tree_node* removeExtra(struct tree_node* tr)
{
   int i=0;
   int newchildren_count = tr->children_count;
   int flag = 0;
   
   while(i < newchildren_count)
   {
   //printf("in remove extra : %d : %s\n", i, mapTo[tr->symbol]);
      int v = tr->children[i]->symbol;
      //printf("%d\n",v);

        int n = tr->children_count;
      if((v>=OP_BEGIN && v<=OP_END) || v==AND || v==OR || v==NOT)//21 -> ELSE
      {
        struct tree_node* ptr = tr->children[i];
        tr->symbol = v;
        int j;
        for(j=i; j<newchildren_count-1;j++)
        {
            tr->children[j] = tr->children[j+1];
        }

        newchildren_count -= 1;
        free(ptr);
      }
          /*
        else if(v==IF)
        {
            tr->children[0]->children_count = n-2;
            int j;
            for(j=1;j<=n-2;j++)
            {
                tr->children[0]->children[j-1] = tr->children[j];
            }
            tr->children[1]=tr->children[n-1];
            newchildren_count = 2;
            tr->children[1]->symbol = tr->children[1]->children[0]->symbol;
            
            tr->children[1]->children_count--;
            for(j=0;j<tr->children_count;j++)
            {
              printf("%d %d\n",j,tr->children_count);
                tr->children[1]->children[j]=tr->children[1]->children[j+1];
            }
            i=0;
        }*/
        /*
        else if( v==IF )//if elsepart not present // tr-> istmt-133
        {
            tr->children_count -= 1;
            tr->symbol = tr->children[0]->symbol;
            int j;
            for(j=1;j<=n-1;j++)
            {
                tr->children[j-1] = tr->children[j];
            }
            newchildren_count-=1;
        }
        */
      else 
      {
        if(v >= 100) removeExtra(tr->children[i]);
        i++;
      }
      
   }
   tr->children_count = newchildren_count;
   return tr;
}

struct tree_node* arithmeticPass(struct tree_node* tr)
{
    
        if(tr->symbol == arithExpn || tr->symbol == arithTerm)//ae,at
        {
            tr->symbol = tr->children[1]->symbol; //PLUS/MINUS, MUL/DIV
            tr->children[1] = tr->children[1]->children[0]; 
        }
        if(tr->children_count > 0 ) //not necassarily a non-terminal
        {
            int i =0;
            int n = tr->children_count;
            while(i < n)
            {
                if(tr->children[i]->children_count > 0) 
                    arithmeticPass(tr->children[i]);
                i++;
            }
        }
    return tr;
}

struct tree_node* create_ast(struct tree_node* root) {
	removeTerm(root);
    remove_Chaining(root);
    removeTerm(root);
    remove_Chaining(root);
    removeExtra(root);

    return root;
}
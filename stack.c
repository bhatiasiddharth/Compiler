#include <stdio.h>
#include <stdlib.h>
#include "stackDataType.h"
#include "parser.h"

stack createStack()
{ 
   stack Top;
   Top = (stack)malloc(sizeof(nodeS));
   (Top)->next = NULL;
   (Top)->value = 159;
   return Top;
}

void push(int item, stack* Top)
{
  if((*Top)->value==-1)
  {
   (*Top)->value = item; ((*Top)->next) = NULL;
   return;
  }
   stack New;
   New = (stack)malloc(sizeof(nodeS));
   New->value = item;
   New->next = *Top;
   *Top = New;
}

int pop(stack* Top)
{
   if((*Top)==NULL) {printf("Cannot Pop!\n");return -1;};
   int item;
   stack temp;
   item = (*Top)->value;
   temp = *Top;
   *Top = (*Top)->next;
   free(temp);
   return item;
}

int top(stack st)
{
   return st->value;
}

void display(stack Top)
{
  printf("Display: ");
  stack temp = Top;
  while(temp!=NULL)
  {
   //printf("%d ",temp->value);
   if(temp->value>=100)
    printf("%s ",nonterm_names[temp->value-100]);

  else
    printf("%s ", token_names[temp->value]);

   temp = temp->next;
  }

  printf("\n");
}

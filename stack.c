#include "stack.h"
#include "parser.h"

struct stack* stack_init() { 
   struct stack* stack;
   stack = (struct stack*) malloc(sizeof(struct stack));
   stack->top = NULL;
   return stack;
}

void stack_push(struct stack* stack, void* item) {
   struct stack_node* new_node = (struct stack_node*) malloc(sizeof(struct stack_node));
   // make sure item was allocated on heap
   new_node->value = item;
   new_node->next = stack->top;
   stack->top = new_node;
}

void* stack_pop(struct stack* stack) {
   if(stack->top == NULL){
      printf("Cannot pop, stack is empty.");
      return NULL;
   }
   struct stack_node* temp = stack->top;
   void* item = temp->value;
   stack->top = temp->next;
   free(item);
   free(temp);
   return item;
}

void* stack_top(struct stack* stack) {
   return stack->top->value;
}

int stack_empty(struct stack* stack) {
  return (stack->top == NULL);
}

void stack_print(struct stack* stack, FILE* fp) {
  //printf("Display: ");
  struct stack_node* temp = stack->top;
  while(temp != NULL){
    //print_symbol(fp, *(int *)(temp->value));
    temp = temp->next;
  }

  printf("\n");
}

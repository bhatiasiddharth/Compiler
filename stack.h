#ifndef _STACK_H
#include "parser.h"
struct stack {
	struct stack_node* top;
};

struct stack_node {
	int value;
	struct stack_node* next;
};

/* Initialise stack */
extern struct stack* stack_init();

/* Top element of the stack */
extern int stack_top(struct stack* stack);

/* Check if stack is empty */
extern int stack_empty(struct stack* stack);

/* Dispose the stack */
//extern int stack_dispose(struct stack* stack);

/* Push node to stack */
extern void stack_push(struct stack* stack, int value);

/* Pop node from stack */
extern int stack_pop(struct stack* stack);

/* Print elements in stack */
extern void stack_print(struct stack* stack, FILE* fp);

#endif
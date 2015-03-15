#ifndef _STACK_H
struct stack {
	struct stack_node* top;
};

struct stack_node {
	int value;
	struct stack_node* next;
};

/* Initialise stack */
extern struct stack* stack_init();

/* Check if stack is empty */
extern int stack_isempty(struct stack*);

/* Empty the stack */
extern int stack_clear(struct stack*);

/* Push node to stack */
extern int stack_push(struct stack stack*, int value);

/* Pop node from stack */
extern int stack_pop(struct stack stack*);

/* Print elements in stack */
extern void stack_print(struct stack stack*);

#endif _STACK_H
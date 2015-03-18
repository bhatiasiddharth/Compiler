#ifndef _SYMBOLTABLE_H
#define _SYMBOLTABLE_H
#include "parser.h"

#define TABLE_SIZE 256

#define MAX_ARR_SIZE 80

struct st_node {
	int type;	// token type
	char identifier[MAX_LEN];
	int scope;

	void* attr;
	struct st_node* next;
};

// for boolean, char, i32, f32
struct raw_attr {
	unsigned int mutable:1;
	unsigned int initialized:1;
	union value value;
};

// for strings
struct string_attr {
	unsigned int length;
	unsigned int mutable:1;
	unsigned int initialized:1;
	union value value;
};

// for arrays
struct array_attr {
	int size;
	union value* value;
};

// for function call
struct function_attr {
	int param_count;
	int return_type;
	int* param_types;
	union value* value;
};

int st_hashfunction(char str[]);

struct st_node* st_init();

int equal(struct st_node node1, struct st_node node2);

int st_insert(struct st_node* symboltable);

int st_lookup(struct st_node* symboltable);

void st_print();
#endif

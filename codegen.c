#include "codegen.h"

// 0 - Empty, 1 = OCCUPIED
int reg[REG_COUNT];
char* reg_names[] = {"AX", "BX", "CX", "DX"};

char dataseg[DATA_SIZE], codeseg[DATA_SIZE];
char* getreg(int reg) {
	return reg_names[reg];
}

void value_tostr(int type, union value* value, char* strval) {
	if(type == T_INT) {
		sprintf(strval, "%d", value->inum);
	}if(type == T_FLOAT) {
		sprintf(strval, "%f", value->fnum);
	}else if(type == T_BOOL) {
		sprintf(strval, "%d", value->bool);
	}else if(type == T_STR) {
		sprintf(strval, "'%s', '$'", value->string);
	}
}
void dataseg_add(char *identifier, int scope, int type, union value* value) {
	char entry[MAX_LEN];
	char storage_type[3];
	char strval[MAX_LEN];
	if(type == T_INT) {
		strcpy(storage_type, "dd");
	}else if (type == T_CHAR || type == T_BOOL || type == T_STR)
		strcpy(storage_type, "db");
	value_tostr(type, value, strval);
	sprintf(entry, "%s_%d %s %s\n", identifier, scope, storage_type, strval);
	strcat(dataseg, entry);
}

void codeseg_add(char *fmt, ...) {
	va_list va;
  va_start(va, fmt);
  vsprintf(codeseg + strlen(codeseg), fmt, va);
	strcat(codeseg, "\n");
  va_end(va);
}

// init register, all available
void init_reg() {
	for (int i = 0; i < REG_COUNT; i++) {
		reg[i] = 0;
	}
}

// get empty register
int get_reg() {
	for (int i = 0; i < 4; i++) {
		if (reg[i] == 1) {
			reg[i] = 0;
			return i;
		}
	}
	return -1;
}

char* get_file_content(char* file) {
	FILE *fp = fopen(file, "r+");
	fseek(fp, 0, SEEK_END);
	long fsize = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	char *utils = malloc(fsize + 1);
	fread(utils, fsize, 1, fp);
	fclose(fp);
	return utils;
}

void codegen_write(char *codegen_file, struct tree_node* ast) {
	FILE *fp = fopen(codegen_file, "w+");
	char* macros = get_file_content(MACROS_FILE);
	init_reg();
	fprintf(fp, ".model tiny\n"
							".486\n"
							".data\n"
							"%s\n" // data segment
							"%s"	// utils
							".code\n"
							".startup\n"
							"%s\n"	// code segment
							".exit\n"
							"end\n", dataseg, macros, codeseg);
	fclose(fp);
	free(macros);
}

#include "codegen.h"

// 0 - Empty, 1 = OCCUPIED
int reg[REG_COUNT];
char* reg_names[] = {"AX", "BX", "CX", "DX"};

char dataseg[DATA_SIZE], codeseg[DATA_SIZE];
char* getreg(int reg) {
	return reg_names[reg];
}

void dataseg_add(char *identifier, int scope, int type) {
	char entry[MAX_LEN];
	char storage_type[3];
	if(type == T_INT) {
		strcpy(storage_type, "dd");
	}else if (type == T_CHAR || T_BOOL || T_STR)
		strcpy(storage_type, "db");
	sprintf(entry, "%s_%d %s\n", identifier, scope, storage_type);
	strcat(dataseg, entry);
}

void codeseg_add(char *stmt) {
	strcat(codeseg, stmt);
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

void codegen_write(char *codegen_file, struct tree_node* ast) {
	FILE *fp = fopen(codegen_file, "w+");

	init_reg();
	fprintf(fp, ".model tiny\n.data\n%s.code\n.startup\n%s\n.exit\nend\n", dataseg, codeseg);
	fclose(fp);
}

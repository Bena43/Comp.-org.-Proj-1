#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#define REG_SIZE 10
#define MEM_SIZE 65536

//Declarations
void print_trace(int reg[16], FILE *FP, int pc, char *str);
void print_regout(int reg[16], FILE *FP);
int function_executer(int reg[16], char * str, char ** MEM, int *pc, FILE * trace, FILE * regout);

void print_trace(int reg[16], FILE *fp, int pc, char *str) {
	// at the begining of each iteration, prints the trace in to the file
	int i;
	unsigned int INST; // inst so we can shift the immidiate to the MSB of INST
	fprintf(fp, "%.8X ", (pc));
	INST = strtol(str, NULL, 16);
	INST += reg[1] * pow(2, 16); // INST=str+imm<<16
	fprintf(fp, "%.8X ", INST);
	for (i = 0; i < 16; i++) {
		fprintf(fp, "%.8X ", reg[i]);
	} //print the registers
	fprintf(fp, "\n");
}

void print_regout(int reg[16], FILE *fp) {
	//print the regout at the end of the exection
	int i;
	for (i = 2; i < 16; i++) {
		fprintf(fp, "%.8X\n", reg[i]);
	}
}
int ctoi(char c) { //char to integer
	if (c <= '9') { return (c - '0'); }
	else if (c >= 'a') { return (c - 'a' + 10); }
	else {
		return (c - 'A' + 10);
	}
}

int function_executer(int reg[16], char * str, char **MEM, int * pc, FILE * trace, FILE * regout) {
	//this function is the core of the simulator, translating the hexadecimal to actual actions and values.
	signed int rd, rs, rt;
	char imm[REG_SIZE];
	rd = ctoi(str[1]);
	rs = ctoi(str[2]);
	rt = ctoi(str[3]);

	if ((rd == 1) || (rs == 1) || (rt == 1)) {
		strcpy(imm, MEM[*(pc)+1]);
		reg[1] = strtol(imm, NULL, 16);
		if (imm[0] > '7') { reg[1] -= MEM_SIZE; }
	}
	print_trace(reg, trace, *(pc), str);
	if ((rd == 1) || (rs == 1) || (rt == 1)) {
		(*(pc))++;
	}

	if (str[0] == '0') { reg[rd] = reg[rs] + reg[rt]; }
	else if (str[0] == '1') { reg[rd] = reg[rs] - reg[rt]; }
	else if (str[0] == '2') { reg[rd] = reg[rs] & reg[rt]; }
	else if (str[0] == '3') { reg[rd] = reg[rs] | reg[rt]; }
	else if (str[0] == '4') { reg[rd] = reg[rs] << reg[rt]; }
	else if (str[0] == '5') { reg[rd] = reg[rs] >> reg[rt]; }
	else if (str[0] == '6') { reg[rd] = 0x9AD1; strcpy(MEM[1022], "9AD1"); } //writes gadi in MEM[1022]
	else if (str[0] == '7') { if (reg[rs] == reg[rt]) { *(pc) = reg[rd] - 1; } }
	else if (str[0] == '8') { if (reg[rs] > reg[rt]) { *(pc) = reg[rd] - 1; } }
	else if (str[0] == '9') { if (reg[rs] <= reg[rt]) { *(pc) = reg[rd] - 1; } }
	else if (str[0] == 'A' || str[0] == 'a') { if (reg[rs] != reg[rt]) { *(pc) = reg[rd] - 1; } }
	else if (str[0] == 'B' || str[0] == 'b') { reg[15] = (*(pc)+1);  *(pc) = reg[rd] - 1; }
	else if (str[0] == 'C' || str[0] == 'c') {
		reg[rd] = strtol(MEM[(reg[rs] + reg[rt]) & 0x0000FFFF], NULL, 16);
		if (reg[rd] > 0x7FFF) reg[rd] -= MEM_SIZE;
	}
	else if (str[0] == 'D' || str[0] == 'd') {
		// making sure words that are stored in the memory are with size of 4 hexadecimal digits.
		sprintf(imm, "%.4X", reg[rd] & 0x0000FFFF);
		strcpy(MEM[(reg[rs] + reg[rt]) & 0x0000FFFF], imm);
	}
	else if (str[0] == 'E' || str[0] == 'e') {
		reg[rd] = (reg[rd] & 0x0000FFFF) + pow(2, 16)*(reg[rs]);
	}
	else if (str[0] == 'F' || str[0] == 'f') { return 1; }
	return 0;
}

int main(int argc, char * argv[]) {
	int pc = 0, token = 0, counter = 0;
	int reg[16];
	char temp[REG_SIZE];
	char * line;
	int i = 0;
	char ** MEM;
	FILE *fp, *memout, *trace, *regout, *count;
	for (i = 0; i < 16; i++) { reg[i] = 0; }
	line = (char*)malloc(sizeof(char)*REG_SIZE);
	MEM = (char**)malloc(sizeof(char*)*MEM_SIZE);
	fp = fopen(argv[1], "r");
	if (fp == NULL) { printf("MALLOC FAILED"); return NULL; }
	for (i = 0; i < MEM_SIZE; i++) {
		MEM[i] = (char*)malloc(sizeof(char)*REG_SIZE);
		if (fgets(temp, 6, fp)) { temp[4] = '\0'; strcpy(MEM[i], temp); }
		else { strcpy(MEM[i], "0000"); }
	}
	fclose(fp);
	trace = fopen(argv[4], "w");
	regout = fopen(argv[3], "w");
	if (trace == NULL) { printf("MALLOC FAILED"); return NULL; }
	for (pc = 0; (pc < MEM_SIZE) && (strcmp(MEM[pc], "0000")) && (token == 0); pc++) {
		reg[0] = 0; // making sure $zero is equal to zero
		reg[1] = 0; // reseting the $imm to 0
		token = function_executer(reg, MEM[pc], MEM, &pc, trace, regout);
		counter++;
	}
	fclose(trace);
	print_regout(reg, regout);
	fclose(regout);
	memout = fopen(argv[2], "w");
	if (memout == NULL) { printf("MALLOC FAILED"); return NULL; }
	for (i = 0; i < MEM_SIZE; i++) { fprintf(memout, "%.4s\n", MEM[i]); }
	fclose(memout);
	for (i = 0; i < MEM_SIZE; i++) { free(MEM[i]); }
	count = fopen(argv[5], "w");
	if (count == NULL) { printf("malloc failed"); return NULL; }
	fprintf(count, "%d", counter);
	fclose(count);
	exit(1);

}
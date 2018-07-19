#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define LINE_MAX 500
#define MEM_SIZE 65536
#define WORD_SIZE 50
#define LABEL_SIZE 50
#define REG_SIZE 6


//Structs
typedef struct Label {
	// we decided to store all the labels, with thier names and pc, in a linked list
	char *name;
	int pc;
	struct Label *next;
}label;

// Declarations
char function_decider(char * str);
char register_decider(char *str);
label *add_label(label* head, char* name, int * pc);
label *create_label(char *str, int * pc);
label *find_label(label *head, char * lab, int *val_pc);
char *beginning_remover(char * str);
char *get_word(char * str);
char *char_remover(char*str, char * r);
label *line_builder(char *line, label *head, int *pc, FILE *fp, char ** MEM);
void second_run(FILE *FP, char **MEM, label *label_head);
label *label_check(char*str, label *lab_init, int * pc);
int word_check(char **MEM, char *line, int *pc);
label *first_run(FILE * FP, char **MEM, label *label_head);


char function_decider(char * str) {
	// This function gets the opcode and returns it's hexadecimal code
	if (!strcmp(str, "add")) { return '0'; }
	if (!strcmp(str, "sub")) { return '1'; }
	if (!strcmp(str, "and")) { return '2'; }
	if (!strcmp(str, "or")) { return '3'; }
	if (!strcmp(str, "sll")) { return '4'; }
	if (!strcmp(str, "sra")) { return '5'; }
	if (!strcmp(str, "gadi")) { return '6'; }
	if (!strcmp(str, "beq")) { return '7'; }
	if (!strcmp(str, "bgt")) { return '8'; }
	if (!strcmp(str, "ble")) { return '9'; }
	if (!strcmp(str, "bne")) { return 'A'; }
	if (!strcmp(str, "jal")) { return 'B'; }
	if (!strcmp(str, "lw")) { return 'C'; }
	if (!strcmp(str, "sw")) { return 'D'; }
	if (!strcmp(str, "lhi")) { return 'E'; }
	if (!strcmp(str, "halt")) { return 'F'; }
	return NULL;
}

char register_decider(char * str)
{	// This function gets a register and returns it's hexadecimal code
	if (!strcmp(str, "$zero")) { return '0'; }
	if (!strcmp(str, "$imm")) { return '1'; }
	if (!strcmp(str, "$v0")) { return '2'; }
	if (!strcmp(str, "$a0")) { return '3'; }
	if (!strcmp(str, "$a1")) { return '4'; }
	if (!strcmp(str, "$t0")) { return '5'; }
	if (!strcmp(str, "$t1")) { return '6'; }
	if (!strcmp(str, "$t2")) { return '7'; }
	if (!strcmp(str, "$t3")) { return '8'; }
	if (!strcmp(str, "$s0")) { return '9'; }
	if (!strcmp(str, "$s1")) { return 'A'; }
	if (!strcmp(str, "$s2")) { return 'B'; }
	if (!strcmp(str, "$gp")) { return 'C'; }
	if (!strcmp(str, "$sp")) { return 'D'; }
	if (!strcmp(str, "$fp")) { return 'E'; }
	if (!strcmp(str, "$ra")) { return 'F'; }
	return NULL;
}

label *add_label(label* head, char* name, int * pc)
{	// adding a label to the label linked list
	label *iter, *l1;
	label *prev = NULL;
	l1 = create_label(name, pc);
	if (l1 == NULL) {
		printf("something went wrong");
		return NULL;
	}
	if (head == NULL) {
		return l1;
	}
	l1->next = head;
	return l1; }

label *create_label(char *str, int * pc) {
	//creates a label, storing his name and program counter.
	label *l1;
	l1 = (label*)malloc(sizeof(label));
	if (l1 == NULL) {
		printf("MALLOC FAILED");
		return NULL;
	}
	l1->name = (char*)malloc(sizeof(char)*LABEL_SIZE);
	strcpy(l1->name, str);
	l1->next = NULL;
	l1->pc = *(pc);
	return l1;
}

label *find_label(label *head, char * lab, int *val_pc) {
	// searching for a label in the listed link, returns the head, changes the value of val_pc
	label *link, *temp;
	link = head;
	if (!(strcmp(link->name, lab))) {
		*(val_pc) = link->pc;
		return head;
	}

	while (link->next != NULL)
	{
		if (!strcmp((link->next->name), lab)) {
			*(val_pc) = link->next->pc;
			break;
		}
		link = link->next;
	}
	return head;
}

char * beginning_remover(char * str)
{	//removes white spaces from the beginning of a string, returns the pointer to the same str,
	//at the first char which isnt a white space.
	int i = 0;
	while (str[i] == ' ' || str[i] == ',' || str[i] == '	') {
		i++;
	}
	return &str[i];
}

char *get_word(char * str)
{	//creates a new string, returning the first word, until encountering a white space.
	char *temp;
	int i = 0;
	temp = (char*)malloc(sizeof(char) * 500);
	strcpy(temp, str);
	while (temp[i] != ' ' && temp[i] != ',' && temp[i] != '\0' && temp[i] != '	' && temp[i]!='\n') {
		str[i] = ' ';
		i++;
	}
	temp[i] = '\0';
	return temp;
}

char * char_remover(char*str, char * r) {
	//deleting char 'r' and everything after it from str.
	char *temp = NULL;
	temp = strstr(str, r);
	if (temp != NULL) { *(temp) = '\0'; }
	return str;
}

label *line_builder(char *line, label *head, int *pc, FILE *fp, char ** MEM) {
	//get a line of assembly code, returns the hexadecimal presentation on simp ISA.
	char *temp, *reg, word[LABEL_SIZE], *semi_const;
	label *L1;
	int place, val;
	reg = (char*)malloc(REG_SIZE * sizeof(char));
	strcpy(word, get_word(line));
	reg[0] = function_decider(word);
	strcpy(line, beginning_remover(line));
	strcpy(word, get_word(line));
	reg[1] = register_decider(word);
	strcpy(line, beginning_remover(line));
	strcpy(word, get_word(line));
	reg[2] = register_decider(word);
	strcpy(line, beginning_remover(line));
	strcpy(word, get_word(line));
	reg[3] = register_decider(word);
	reg[4] = '\0';
	//reg is the hexadecimal string that is representing the line of assembly code
	MEM[*(pc)] = (char*)malloc(sizeof(char)*REG_SIZE);
	strcpy(MEM[*(pc)], reg);
	if (reg[1] == '1' || reg[2] == '1' || reg[3] == '1') {
		//if we have an immidiate, handle and save in MEM the 
		strcpy(line, beginning_remover(line));
		strcpy(word, get_word(line));
		if (word[0] > '9') {// label: (the ASCII value of '-' and numbers is lower then lower and upper cases)
			head = find_label(head, word, &place); // get the place(program counter) in which the label is located.
			sprintf(word, "%.4X", place); //turn place to string of hexadecimal presentation with length = 4
			MEM[*(pc)+1] = (char*)malloc(sizeof(char)*REG_SIZE);
			strcpy(MEM[*(pc)+1], word); //save the immidiate(the place, the program counter of the label) in the next line.
		}
		else //immidiate
		{
			if (word[1] == 'x' || word[1] == 'X') //hexadecimal number
				val = strtol(word, NULL, 16); //save the value of the immidiate in val
			else {
				val = strtol(word, NULL, 10); //save the value of the immidiate in val
				if (word[0] == '-') { // negative decimal
					val += 65536; //16^4, 10000 in unsigned hexadecimal.
				}
			}
			sprintf(word, "%.4X", val); // save the value calculated above in word
			MEM[*(pc)+1] = (char*)malloc(sizeof(char)*REG_SIZE);
			strcpy(MEM[*(pc)+1], word); // save the word in the memory array
		}
		(*(pc))++;
	}
	return head; //the function returns the head of the label list, in order to not "lose" it
}

void second_run(FILE *FP, char **MEM, label *label_head) {
	// gathers all the functions needed for the second run (translating and saving in to the memory array)
	int pc = 0;
	char line[LINE_MAX];
	char word[WORD_SIZE];
	while (fgets(line, LINE_MAX, FP)) {
		strcpy(line, char_remover(line, "#")); //removes the comment of the assembly code, no need for it
		strcpy(line, beginning_remover(line)); // delete white spaces from beginning of the assembly code line
		if (!strcmp(line, "\n") || !strcmp(line, "")) { // if the line is empty after deleting white spaces, go to next line without enlarging the program counter
			continue;
		}
		if (!(line[0] == '.' || strstr(line, ":")))
		{	// if its not a label or word, translate and save in Memory array
			label_head = line_builder(line, label_head, &pc, FP, MEM);
			pc++;
		}
	}
}

label * label_check(char*str, label *lab_head, int * pc)
{
	char *temp;
	temp = strstr(str, ":");
	if (temp) { // if we have : in our line (after removing the comment)
		strcpy(str, char_remover(str, ":")); // save labels name
		lab_head = add_label(lab_head, str, pc); // save the label in the label's linked list
		(*(pc))--; //dont go to the next pc, because label is not counted as a PC.
		return lab_head;
	}
	temp = strstr(str, "$imm");
	if (temp) { (*(pc))++; } // if the imm is not label, add one more to the pointer.

	return lab_head; //return the label's linked list head in order to not lose it.
}

int word_check(char **MEM, char *line, int *pc) {
	// checks if the line is .word function
	int place, num;
	char * word;
	word = (char*)malloc(sizeof(char)*LINE_MAX);
	if (line[0] == '.') {
		strcpy(line, beginning_remover(line));
		strcpy(word, get_word(line)); //delete the .word from the begining of the string
		strcpy(line, beginning_remover(line));
		strcpy(word, get_word(line)); // get the first argument of .word's function -> the location which the word will be stored at.
		if (word[1] == 'x' || word[1] == 'X')  //handling hexadecimal or decimal presentation
			place = strtol(word, NULL, 16);
		else
			place = strtol(word, NULL, 10);
		strcpy(line, beginning_remover(line));
		strcpy(word, get_word(line)); // get the word which we want to save
		if (word[1] == 'x' || word[1] == 'X') //handling hexadecimal or decimal presentation
			num = strtol(word, NULL, 16);
		else {
			num = strtol(word, NULL, 10);
			if (word[0] == '-')  // negative decimal
				num += 65536; //16^4, 10000 in unsigned hexadecimal.
		}
		sprintf(word, "%.4X", num); // translate the int to string
		strcpy(MEM[place], word); // save the word in it's intended location
		(*(pc))--; //.word is not counted as a function.
		return 1; //tell the calling function that the string is indeed a word function.
	}
	else
		return 0; //dont do anything, go back to called function, continue.
}

label * first_run(FILE * FP, char **MEM, label *label_head) {
	//first run saves the labels in a linked list and stores the words of .word functions
	int pc;
	char line[LINE_MAX], word[LABEL_SIZE];
	for (pc = 0; (pc < MEM_SIZE) && (fgets(line, LINE_MAX, FP)!=NULL); pc++) {
		strcpy(line, char_remover(line, "#")); // removes the comment
		strcpy(line, beginning_remover(line)); // removes the white spaces at the begining of the line
		if (!strcmp(line, "\n") || !strcmp(line, "")) {  pc--; continue; } // if the line is empty, go to next iteration without adding to the pc.
													 //	strcpy(word, get_word(line));
		if (!(word_check(MEM, line, &pc))) {// if its a word, saves the word inside if's condition
			label_head = label_check(line, label_head, &pc); // else, add the label in to the labels linked list
		}
	}
	return label_head; // returns the label head in order to not lose it.
}

int main(int argc, char * argv[])
{
	char *str1, temp[6], **MEM;
	FILE *mem_import, *FirstrunFP, *SecondrunFP, *mem_export;
	label *label_head = NULL, *temp_label = NULL;
	int i = 0;
	//mem_import = fopen(argv[2], "r");
	//if (mem_import == NULL) { printf("file could not be read"); return NULL; }
	MEM = (char**)malloc((sizeof(char*) * MEM_SIZE));
	if (MEM == NULL) {
		printf("MALLOC FAILED");
		return NULL;
	}
	for (i = 0; i < MEM_SIZE; i++) {
		//saves the mem in array
		MEM[i] = (char*)malloc(sizeof(char)*REG_SIZE);
		/*if (fgets(temp, 6, mem_import)) {
			temp[4] = '\0';
			strcpy(MEM[i], temp);*/
		
		//else 
		strcpy(MEM[i], "0000");  /*if nothing in memory, save 0000*/ }
	/*fclose(mem_import);*/
	FirstrunFP = fopen(argv[1], "r");
	if (FirstrunFP == NULL) { printf("file could not be read"); return NULL; }
	label_head = first_run(FirstrunFP, MEM, label_head);
	fclose(FirstrunFP);
	SecondrunFP = fopen(argv[1], "r");
	if (SecondrunFP == NULL) { printf("file could not be read"); return NULL; }
	second_run(SecondrunFP, MEM, label_head);
	fclose(SecondrunFP);
	mem_export = fopen(argv[2], "w");
	for (i = 0; i < MEM_SIZE - 1; i++) { fprintf(mem_export, "%s\n", MEM[i]); } //print the memory in memin text file
	fprintf(mem_export, "%s", MEM[i]); //prints the last register without \n for perfaction
	for (i = 0; i < MEM_SIZE; i++) { free(MEM[i]); } //free the memory array
	while (label_head != NULL) { temp_label = label_head; label_head = label_head->next; free(temp_label); } // free the labels saved
	fclose(mem_export);
	exit(1);
}
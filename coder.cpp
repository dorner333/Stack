#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int OPEN_ERR = 1;

enum {END = '0', PUSH, PUSHR, POP, POPR, ADD, SUB, DIV, MUL, FSQRT, IN, PRINT, OUT} CMDS;
enum {RAX = '0', RBX, RCX, RDX} REGS;

int read(char** symbols, char** f_asm);
void write(char* symbols, int SIZE);

int main(int argc, char* argv[]){

	char* symbols = NULL;
	
	int SIZE = read(&symbols, argv + 1);
	
	if(SIZE == OPEN_ERR){
		printf("OPEN_ERR\n");
		return OPEN_ERR;
	}

	write(symbols, SIZE);

	return 0;
}

int read(char** symbols, char** f_asm){

	FILE* file1 = fopen("unTest.asm", "r+");

	if(file1 == NULL){
		return OPEN_ERR;
	}

	fseek(file1, 0, SEEK_END);
	int SIZE = ftell(file1);
	rewind(file1);
	
	*symbols = (char*)calloc(SIZE, sizeof(char));
	fread(*symbols, sizeof(char), SIZE, file1);

	for(int i = 0; i < SIZE; i++){
		if((*symbols)[i] == ' ' || (*symbols)[i] == '\n'){
			(*symbols)[i] = '\0';
		}
	}

	fclose(file1);

	return SIZE;
}

void write(char* symbols, int SIZE){

	FILE* file2 = fopen("TEST.asm", "w");

	char* code = (char*)calloc(SIZE, sizeof(char));

	int j = 0, pnt = 0;

	for(int i = 0; i < SIZE; i++)
	{
		printf("stroka is (%s)\n", symbols + pnt );
		if(symbols[i] == '\0'){
			if(!strcmp(symbols + pnt, "push")){
				code[j++] = PUSH;
			} else if(!strcmp(symbols + pnt, "pushr")){
				code[j++] = PUSHR;
			} else if(!strcmp(symbols + pnt, "pop")){
				code[j++] = POP;
			} else if(!strcmp(symbols + pnt, "popr")){
				code[j++] = POPR;
			} else if(!strcmp(symbols + pnt, "add")){
				code[j++] = ADD;
			} else if(!strcmp(symbols + pnt, "sub")){
				code[j++] = SUB;
			} else if(!strcmp(symbols + pnt, "div")){
				code[j++] = DIV;
			} else
			printf("stroka is %s", symbols + pnt );
			 if(!strcmp(symbols + pnt, "mul")){
				code[j++] = MUL;
			} else if(!strcmp(symbols + pnt, "fsqrt")){
				code[j++] = FSQRT;
			} else if(!strcmp(symbols + pnt, "out")){
				code[j++] = OUT;
			} else if(!strcmp(symbols + pnt, "print")){
				code[j++] = PRINT;
			} else if(!strcmp(symbols + pnt, "in")){
				code[j++] = IN;
			} else {
				if(!strcmp(symbols + pnt, "rax")){
					code[j++] = RAX;
				} else if(!strcmp(symbols + pnt, "rbx")){
					code[j++] = RBX;
				} else if(!strcmp(symbols + pnt, "rcx")){
					code[j++] = RCX;
 				} else if(!strcmp(symbols + pnt, "rdx")){
					code[j++] = RDX;
				} else {	
					while(symbols[pnt] != '\0')
						code[j++] = symbols[pnt++];
				}
			}
			code[j++] = '\n';
			pnt = i + 1;
		}
	}
	fwrite(code, sizeof(char), j, file2);

	fclose(file2);
	free(symbols);
	free(code);
}
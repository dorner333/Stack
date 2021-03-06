
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int OPEN_ERR = 1;

//	   0          1     2    3    4    5    6   7      8     8   
enum { NOP = '0', PUSH, ADD, SUB, DIV, MUL, IN, PRINT, JUMP, OUT} CMDS; // standart comands
//     0          1     2        
enum { R_1 = '0', R_2, R_SIZE } RG;  // registers
//     A			B         
enum { RPUSH = 'A', RPOP }RCMDS; // registers commands
//     a 
enum { CMD = 'a' }UCMD; // user commands

typedef struct LABEL
{
char* name;
int adress;	
}label;

int open(char** symbols);
void asembler(char* symbols, int SIZE);
int lable_count(char* symbols, int SIZE);
int make_label(label** labels, char* symbols, int SIZE, int nof_labels);

int main(){

	char* symbols = NULL;
	
	int SIZE = open(&symbols);
	
	if(SIZE == OPEN_ERR)
    {
		printf("OPEN_ERR\n");
		return OPEN_ERR;
	}

	asembler(symbols, SIZE);

	return 0;
}

int open(char** symbols)
{

	FILE* file1 = fopen("unTest.txt", "r+");

	if(file1 == NULL)
    {
		return OPEN_ERR;
	}

	fseek(file1, 0, SEEK_END);
	int SIZE = ftell(file1);
	rewind(file1);
	
	char* symbols_win = (char*)calloc(SIZE, sizeof(char));
    *symbols          = (char*)calloc(SIZE, sizeof(char));
	fread(symbols_win, sizeof(char), SIZE, file1);
      
    printf("(%s)\n", symbols_win);

	for(int i = 0; i < SIZE; i++)
    {
		if((symbols_win)[i] == ' ' || (symbols_win)[i] == '\n' || (symbols_win)[i] == '\r' )
        {
			(symbols_win)[i] = '\0';
		}
	}

	int cnt = 0;
    for(int i = 0; i < SIZE; i++)
    {
        if (!((symbols_win)[i] == '\0' && (symbols_win)[i+1] == '\0'))
        {
            (*symbols)[cnt] = (symbols_win)[i];
            cnt++;
        }
    }

	fclose(file1);

	return SIZE;
}

void asembler(char* symbols, int SIZE)
{

    FILE* file2 = fopen("TEST.txt", "w");

	int nof_lable = lable_count(symbols, SIZE);
    char*  code   = (char*)  calloc(SIZE, sizeof(char));
	label* labels = (label*) calloc(nof_lable, sizeof(label));

	make_label(	&labels, symbols, SIZE, nof_lable);

    int j = 0, pnt = 0;
	for(int i = 0; i < SIZE; i++)
    {
		if(symbols[i] == '\0')
        {
			if(!strcmp(symbols + pnt, "nop")){
				code[j++] = NOP;
            } else if(!strcmp(symbols + pnt, "push")){
				code[j++] = PUSH;
			} else if(!strcmp(symbols + pnt, "rpush")){
				code[j++] = RPUSH;
			} else if(!strcmp(symbols + pnt, "popr")){
				code[j++] = RPOP;
			} else if(!strcmp(symbols + pnt, "add")){
				code[j++] = ADD;
			} else if(!strcmp(symbols + pnt, "sub")){
				code[j++] = SUB;
			} else if(!strcmp(symbols + pnt, "div")){
				code[j++] = DIV;
			} else if(!strcmp(symbols + pnt, "mul")){
				code[j++] = MUL;
			} else if(!strcmp(symbols + pnt, "out")){
				code[j++] = OUT;
			} else if(!strcmp(symbols + pnt, "print")){
				code[j++] = PRINT;
			} else if(!strcmp(symbols + pnt, "jump")){
				code[j++] = JUMP;
			} else if(!strcmp(symbols + pnt, "in")){
				code[j++] = IN;
			} else {
				if(!strcmp(symbols + pnt, "r_1")){
					code[j++] = R_1;
				} else if(!strcmp(symbols + pnt, "r_2")){
					code[j++] = R_2;
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

int lable_count(char* symbols, int SIZE)
{
	int count = 0;
	
	for(int i = 0; i < SIZE; i++)
	{
	if((symbols)[i] == ':') count++;	
	}

	return count;
}

int make_label(label** labels, char* symbols, int SIZE, int nof_labels)
{
	int j = 0;
	for (int i = 0; i < nof_labels; i++)
	{
		(*labels)[i].name = "ded";
		(*labels)[i].adress = -1 ;
	}
	for (int i = 0; i < nof_labels; i++)
	{
	 printf("stroka is %s\n",(*labels)[i].name);
	 printf("adress is %d\n",(*	labels)[i].adress);
	}
//char man[200] = "loh";
(*labels)[j].name = "ded";
printf("neupal %d\n",__LINE__);
	for(int i = 0; i < SIZE; i++)
	{
		if (symbols[i] == ':')
		{
			printf("neupal %d i =  %d\n",__LINE__, i);
			printf("s is: %s\n", symbols + i + 1);
			strcpy( ((*labels)[j].name), (symbols + i + 1));
			j++;
			printf("&&&&&&&&\n");
		}
	}
	//printf("%s\n",man);
	return j;
}


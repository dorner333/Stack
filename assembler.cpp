
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int OPEN_ERR = 1;

//	   0	1	  2	   3 	4    5    6      7   8      9     10   11
enum { end, PUSH, ADD, SUB, DIV, MUL, FSQRT, IN, PRINT, JUMP, OUT, NOP } CMDS;
//     0    1    2
enum { R_1, R_2, R_SIZE } RG;
//                 22          
enum { RPUSH = 21, RPOP };

int open(char** symbols);
void read(char* symbols, int SIZE);

int main(){

	char* symbols = NULL;
	
	int SIZE = open(&symbols);
	
	if(SIZE == OPEN_ERR)
    {
		printf("OPEN_ERR\n");
		return OPEN_ERR;
	}

	read(symbols, SIZE);

	return 0;
}

int open(char** symbols)
{

	FILE* file1 = fopen("unTest.asm", "r+");

	if(file1 == NULL)
    {
		return OPEN_ERR;
	}

	fseek(file1, 0, SEEK_END);
	int SIZE = ftell(file1);
	rewind(file1);
	
	*symbols = (char*)calloc(SIZE, sizeof(char));
	fread(*symbols, sizeof(char), SIZE, file1);
    
    printf("(%s)\n", *symbols);

    
	for(int i = 0; i < SIZE; i++)
    {
		if((*symbols)[i] == ' ' || (*symbols)[i] == '\n')
        {
			(*symbols)[i] = '#';
		}
	}

    printf("(%s)\n", *symbols);
    
    /*
    for(int i = 0; i < SIZE; i++)
    {
		if((*symbols)[i] == ' ')
        {
			(*symbols)[i] = '\0';
		}
        if((*symbols)[i] == '\n')
        {
			(*symbols)[i] = '\0';
		}
	}
    */
	fclose(file1);

	return SIZE;
}

    void read(char* symbols, int SIZE)
    {

        FILE* file2 = fopen("TEST.asm", "w");

        char* code = (char*)calloc(SIZE, sizeof(char));

        int j = 0, pnt = 0;

        for(int i = 0; i < SIZE; i++)
        {		
            if(symbols[i] == '\0')
            {
            //printf("stroka is (%s\n", symbols + pnt );
            if (!strcmp(symbols + pnt, "mul"))
            {
            printf("MUL\n");
            }
            //if(!strcmp(symbols + pnt, "push"))
            //{
            //    printf("push\n");
            //}
            pnt = i + 1;
            }
        
        }
    }


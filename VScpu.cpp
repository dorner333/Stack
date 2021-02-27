#define _CRT_SECURE_NO_WARNINGS
#include "stack.cpp"

#define FILE_ERROR 1
#define CMD_ERROR 2

typedef struct registers
{
const char* name;
int data;
} registers;

//	   0	1	  2	   3 	4    5    6      7   8      9     10
enum { eыnd, PUSH, ADD, SUB, DIV, MUL, FSQRT, IN, PRINT, JUMP, OUT } CMDS;
//     0    1    2
enum { R_1, R_2, R_SIZE } RG;
//                 22          
enum { RPUSH = 21, RPOP };


int file (int** cmd);
int proc(int* cmd, int SIZE);

registers rgs[R_SIZE] = {{"R_1", 5},{"R_2", 8}};

int main() 
{
	for (int i = 0; i < R_SIZE;i++)
	{
		printf("%s - ", rgs[i].name);
		printf("%d\n", rgs[i].data);	
	}

	int* cmd;
	int SIZE = file (&cmd);
	proc(cmd, SIZE);

	return 0;
}

int file(int** cmd) 
{
	FILE* comands = fopen("TEST.txt", "r");

	if (comands == NULL) 
	{
		fclose(comands);
		return FILE_ERROR;
	}

	fseek(comands, 0, SEEK_END);                          //kursor na posledniy simvol
	int SIZE = ftell(comands) + 1;                        //rasmer raven yacheyke pointera
	rewind(comands);                                      //kursor v nachalo

	char* char_cmd = (char*)calloc(SIZE, sizeof(char));
	*cmd = (int*)calloc(SIZE, sizeof(int));

	fread(char_cmd, sizeof(char), SIZE, comands);

	int j = 0, pnt = 0, command_counter = 0;
	for (int i = 0; i < SIZE; i++) 
	{
		if (char_cmd[i] == '\n' || char_cmd[i] == '\0' ) 
		{
			char_cmd[i] = '\0';
			(*cmd)[j++] = atoi(char_cmd + pnt);
			command_counter++;
			pnt = i + 1;
		}
	}
	
	fclose(comands);
	free(char_cmd);

	for (int i = 0; i < command_counter; i++)
	{
		printf("%d ", (*cmd)[i]);
	}
	printf("\n");


	return SIZE;

}


int proc(int* cmd, int SIZE)
{
	int mode = poison;
	STACK stack;

	construct(&stack, start_size);

	if (cmd[0] != PUSH)
	{
	printf("Bad input data\n");
	return CMD_ERROR;
	}
	else mode = 1; 

	int tmp1 = 0, tmp2 = 0, top = 0;

	int registers[R_SIZE] = {0};

		for(int i = 0; (i < stack.capacity) && (cmd[i] != OUT) ; i++)
		{
			//printf("ya rabotat %d\n", i);

		printf("command is %d\n", cmd[i]);
		
		if(mode == 1) //waiting for command
		{

			switch(cmd[i])
			{
				case(PUSH):
					mode = 0;
					break;
				case(ADD):				
					tmp1 = pop(&stack), tmp2 = pop(&stack);
					push(&stack, tmp2 + tmp1);
					break;
				case(SUB):			
					tmp1 = pop(&stack), tmp2 = pop(&stack);
					push(&stack, tmp2 - tmp1);
					break;
				case(DIV):
					tmp1 = pop(&stack), tmp2 = pop(&stack);
					push(&stack, (tmp2)/(tmp1));
					break;
				case(MUL):
					tmp1 = pop(&stack), tmp2 = pop(&stack);
					push(&stack, (tmp2) * (tmp1));
					break;
				case(IN):
					scanf("%d", &tmp1);
					push(&stack, tmp1);
					break;
				case(PRINT):
					top = pop(&stack);
					push(&stack, top);
					printf("TOP = %d\n", top);
					break;
				case(JUMP):
					mode = 0;
					break; 
				case(RPOP):
					mode = 3;
					break;
				case(RPUSH):
					mode = 3;
					break;  	
				}

		} 
		
		if(mode == 0) //waiting for number
		{
			switch(cmd[i-1])
			{
				case(PUSH):
						push(&stack, cmd[i]);		
					mode = 1;
					break;
				case(JUMP):
					i = i - cmd[i];
					mode = 1;
					break; 
			}
		}

		if(mode == 3) //waiting for register
		{
			switch(cmd[i-1])
			{
				case(RPUSH):
					push(&stack, rgs[cmd[i]].data);		
					mode = 1;
					break;
				case(RPOP):
					top = pop(&stack);
					rgs[cmd[i]].data = top; 
					mode = 1;
					break; 
			}
		}

		stack_show (&stack);
	}

	destructor(&stack);
	free(cmd);

	return 0;


}


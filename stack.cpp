
#include "stack.h"
#line 4
//#include <iostream>


#define DEBUG 0                        // �������� �� 1, ���� ����� ����� ���
#define DBG(expr) if (DEBUG) {expr}

/*
int main()
{
    STACK stack = {0, 0, NULL, 1};
    construct(&stack, start_size);

    for (int i = 0; i < 1000000; i++)
    {
        push(&stack, 3);
    }

    DBG(stack_dump(&stack);)

    printf("%d\n", pop(&stack));

    destructor(&stack);

    DBG(stack_dump(&stack);)

    return 0;
}
*/

//-----------------------------------------------------------------------------

void construct (STACK* stack, int in_size)
{
    (*stack).capacity    = in_size;
    (*stack).buf         = (int*) calloc(in_size, sizeof(int));
    (*stack).size        = 0;
    (*stack).destruction = non_destructed;

    for (int i = (*stack).size ; i < (*stack).capacity; i++)
    {
            (*stack).buf[i] = poison;
    }

stack_ok(stack);
}

//-----------------------------------------------------------------------------

void push (STACK* stack, int element)
{
stack_ok(stack);

    if ((*stack).capacity == (*stack).size) recalloc(stack);
    (*stack).buf[(*stack).size] = element;
    (*stack).size ++;

stack_ok(stack);
}

//-----------------------------------------------------------------------------

int pop (STACK* stack)
{
stack_ok(stack);

    int element = (*stack).buf[(*stack).size - 1];
    (*stack).buf[(*stack).size - 1] = poison;
    (*stack).size --;
    return element;

stack_ok(stack);
}
//-----------------------------------------------------------------------------

void recalloc (STACK* stack)
{
stack_ok(stack);

     STACK local_stack = {0, 0, NULL, 1};

    (*stack).buf = (int*) realloc((*stack).buf,((*stack).capacity + upsize)  * sizeof(int));
    (*stack).capacity += upsize ;

    for (int i = (*stack).size; i < (*stack).capacity; i++)
    (*stack).buf[i] = poison;

stack_ok(stack);
}

//-----------------------------------------------------------------------------

void stack_printf (STACK stack)
{
    printf("Stack - (");
    for(int i = 0; i < stack.size - 1; i++) printf("%d, ", stack.buf[i]);

    printf("%d)\n", stack.buf[stack.size - 1]);

}

//-----------------------------------------------------------------------------

void destructor (STACK* stack)
{
stack_ok(stack);

    (*stack).capacity    = poison;
    (*stack).size        = poison;
    (*stack).destruction = destructed;
    free ((*stack).buf);
}

//-----------------------------------------------------------------------------

int errortest(STACK stack)
{
int bag = no_error;
if ((stack.capacity > stack.size) && (stack.buf[stack.size] != poison )) bag = unpoisoned_element;
if (stack.capacity < stack.size)                                         bag = bad_capacity;
if (stack.buf == NULL)                                                   bag = bad_buff_adress;
if (&stack == NULL)                                                      bag = bad_stack_adress;
if (stack.destruction == destructed)                                     bag = destructed_stack;
return bag;
}

//-----------------------------------------------------------------------------

void stack_dump (STACK* stack)
{
int error = errortest (*stack);

                printf ("\n[][][][][][][][][][][][][][][][][][][][][][][][][][][]\n");
if (error == 0) printf("Stack (OK)\n");
else            
{
                printf("Stack (BAD), error - %d\n", error);
                print_error(error);
}
                //printf("adress - [%X]\n", (int)stack);
                    printf("{\n");
                    printf("\tcapacity    = %d\n", (*stack).capacity);
                    printf("\tsize        = %d\n", (*stack).size);
                    printf("\tdestruction = %d\n", (*stack).destruction);
                    //printf("\tbuf [%X]\n",(int)(*stack).buf);
                        printf("\t{\n");
                        for(int i = 0; i < (*stack).capacity;i++) printf("\t\t[%d]: %d\n", i, (*stack).buf[i]);
                        printf("\t}\n");
                    printf("}\n");
                printf ("[][][][][][][][][][][][][][][][][][][][][][][][][][][]\n\n");


}

void stack_show (STACK* stack)
{
    printf("Stack: ");

    for (int i = 0; i < (*stack).size; i++)
    {
    printf("[%d]",(*stack).buf[i]);   
    }

    printf("\n");

}



void stack_ok (STACK* stack)
{
if(errortest(*stack) != 0)
    {
    stack_dump(stack);
    }
}


void print_error(int error)
{
    switch (error)
    {
        case no_error:           printf("no_error\n");  break;
        case destructed_stack:   printf("destructed_stack\n");  break;
        case bad_stack_adress:   printf("bad_stack_adress\n");  break;
        case bad_buff_adress:    printf("bad_buff_adress\n");  break;
        case bad_capacity:       printf("bad_capacity\n");  break;
        case unpoisoned_element: printf("unpoisoned_element\n");  break;
    }
}
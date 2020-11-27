#include <stdio.h>
#include <stdlib.h>
#include <iostream>

//[*][*][*][*][*][*][*][*][*][*][*][*][*][*][*][*][*][*][*][*][*][*][*][*][*][*]
enum
{
    destructed = 1,
    non_destructed = 0,
    start_size = 8,
    upsize = 3,
    poison = -666
};
//[*][*][*][*][*][*][*][*][*][*][*][*][*][*][*][*][*][*][*][*][*][*][*][*][*][*]


typedef struct
{
    int* buf;
    int size;
    int capacity;
    int destruction;
} STACK;

//[*][*][*][*][*][*][*][*][*][*][*][*][*][*][*][*][*][*][*][*][*][*][*][*][*][*]

void construct    (STACK* stack, int in_size);

void push         (STACK* stack, int element);

int  pop          (STACK* stack             );

void recalloc     (STACK* stack             );

void stack_printf (STACK  stack             );

void destructor   (STACK* stack             );

int errortest     (STACK  stack             );

//[*][*][*][*][*][*][*][*][*][*][*][*][*][*][*][*][*][*][*][*][*][*][*][*][*][*]

int main()
{
    STACK stack = {NULL, 0, 0, destructed};
    construct(&stack, start_size);

    push (&stack, 10);
    push (&stack, 20);
    stack_printf(stack);

    printf("%d\n", pop(&stack));
    stack_printf(stack);

    printf("%d\n", errortest(stack));
    destructor(&stack);
    printf("%d\n", errortest(stack));

    return 0;
}

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
}

//-----------------------------------------------------------------------------

void push (STACK* stack, int element)
{
    if ((*stack).capacity == (*stack).size) recalloc(stack);
    (*stack).buf[(*stack).size] = element;
    (*stack).size ++;
}

//-----------------------------------------------------------------------------

int pop (STACK* stack)
{
    int element = (*stack).buf[(*stack).size - 1];
    (*stack).size --;
    return element;
}
//-----------------------------------------------------------------------------

void recalloc (STACK* stack)
{                                // когда умножаю на 2 реалок умирает на больших числах
    (*stack).buf = (int*) realloc((*stack).buf, ((*stack).capacity + upsize) * sizeof(int));
    (*stack).capacity += upsize ;

    for (int i = (*stack).size; i < (*stack).capacity; i++)
    (*stack).buf[i] = poison;

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
    (*stack).capacity    = poison;
    (*stack).size        = poison;
    (*stack).destruction = destructed;
    free ((*stack).buf);
}

//-----------------------------------------------------------------------------

int errortest (STACK stack)
{
int bag = 0;
if ((stack.capacity > stack.size) && (stack.buf[stack.size] != poison )) bag = 5;
if (stack.capacity < stack.size) bag = 4;
if (stack.buf == NULL) bag = 3;
if (&stack == NULL) bag = 2;
if (stack.destruction == destructed) bag = 1;
return bag;
}

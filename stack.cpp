#include <stdio.h>
#include <stdlib.h>
#include <iostream>
//[*][*][*][*][*][*][*][*][*][*][*][*][*][*][*][*][*][*][*][*][*][*][*][*][*][*]

#define DEBUG 1
                        // change to 1, if u want debug mod
#define DBG(expr) if (DEBUG) {expr;}

#define STACK_OK(stack)

//[*][*][*][*][*][*][*][*][*][*][*][*][*][*][*][*][*][*][*][*][*][*][*][*][*][*]
enum
{
    start = 1,
    end = 0,
    destructed = 1,
    non_destructed = 0,
    start_size = 10,
    upsize = 4,
    hash_error = 228,
    int_const = 1,
    poison = -666,
    buf_canary = 11223344,
    stack_canary = 1122334455667788 // if > 11223344 - (4 warnings)
};

//[*][*][*][*][*][*][*][*][*][*][*][*][*][*][*][*][*][*][*][*][*][*][*][*][*][*]


typedef struct
{
    unsigned long long start_canary;

    int size;
    int capacity;
    int* buf;
    int destruction;
    int hash;

    unsigned long long end_canary;
} STACK;

//[*][*][*][*][*][*][*][*][*][*][*][*][*][*][*][*][*][*][*][*][*][*][*][*][*][*]

void construct    (STACK* stack, int in_size);

void push         (STACK* stack, int element);

int  pop          (STACK* stack             );

void recalloc     (STACK* stack             );

void stack_printf (STACK  stack             );

void destructor   (STACK* stack             );

int hash_maker    (STACK* stack             );

int  errortest    (STACK  stack             );

void stack_dump   (STACK* stack,  int error );

void stack_ok     (STACK* stack, int place  );

//[*][*][*][*][*][*][*][*][*][*][*][*][*][*][*][*][*][*][*][*][*][*][*][*][*][*]

int main()
{
    STACK stack = {0, 0, 0, NULL, 1, 0, 0};
    construct(&stack, start_size);

    DBG(stack_dump(&stack, errortest(stack)));

    for (int i = 0; i < 1; i++)
    {
        push(&stack, 3);
    }

    DBG(stack_dump(&stack, errortest(stack)));

    printf("%d\n", pop(&stack));
    destructor(&stack);

    DBG(stack_dump(&stack, errortest(stack)));

    return 0;
}

//-----------------------------------------------------------------------------

void construct (STACK* stack, int in_size)

{

    (*stack).capacity    = in_size;
    (*stack).buf         = (int*) calloc(in_size + 2 * int_const, sizeof(int));
    (*stack).size        = 0;
    (*stack).destruction = non_destructed;
    (*stack).start_canary = stack_canary;
    (*stack).end_canary = stack_canary;

    (*stack).buf[0] = buf_canary;
    (*stack).buf[(*stack).capacity - int_const] = buf_canary;

    for (int i = (*stack).size + int_const ; i < (*stack).capacity - int_const; i++)
    {
            (*stack).buf[i] = poison;
    }

stack_ok(stack, end);
}

//-----------------------------------------------------------------------------

void push (STACK* stack, int element)
{
stack_ok(stack, start);


    if ((*stack).capacity == (*stack).size) recalloc(stack);
    (*stack).buf[(*stack).size + int_const] = element;
    (*stack).size ++;

stack_ok(stack, end);
}

//-----------------------------------------------------------------------------

int pop (STACK* stack)
{
stack_ok(stack, start);

    int element = (*stack).buf[(*stack).size - 1 + int_const];
    (*stack).buf[(*stack).size - 1 + int_const] = poison;
    (*stack).size --;

stack_ok(stack, end);
return element;
}
//-----------------------------------------------------------------------------

void recalloc (STACK* stack)
{
stack_ok(stack, start);

                                              //realloc die, when  *2
    (*stack).buf = (int*) realloc((*stack).buf,   ((*stack).capacity + upsize)  * sizeof(int));
    (*stack).capacity += upsize ;

    (*stack).buf[(*stack).capacity - int_const] = buf_canary;
    for (int i = (*stack).size; i < (*stack).capacity - int_const; i++)
    (*stack).buf[i] = poison;

stack_ok(stack, end);
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
stack_ok(stack, start);

    (*stack).capacity    = poison;
    (*stack).size        = poison;
    (*stack).destruction = destructed;
    free ((*stack).buf);
}

//-----------------------------------------------------------------------------
int hash_maker(STACK* stack)
{
int hash = 0;
hash += 1337 * (*stack).size;
hash += 228  % (*stack).capacity;
hash += 322  * (int) (*stack).buf;
hash += 1488 % ((*stack).destruction + 17);
return hash;
}

//-----------------------------------------------------------------------------

int errortest(STACK stack)
{
    int bag = 0;
    if ((stack.capacity > stack.size) && (stack.buf[stack.size + int_const] != poison ))       bag = 6;
    if (stack.capacity < stack.size)                                                           bag = 5;
    if (stack.buf == NULL)                                                                     bag = 4;
    if (&stack == NULL)                                                                        bag = 3;
    if (stack.destruction == destructed)                                                       bag = 2;
    if ((stack.buf[0] != buf_canary) || (stack.buf[stack.capacity - int_const] != buf_canary)) bag = 1;
    return bag;
}

//-----------------------------------------------------------------------------

void stack_dump (STACK* stack, int error)
{


                    printf ("\n[][][][][][][][][][][][][][][][][][][][][][][][][][][]\n");

    if (error == 0) printf("Stack (OK)\n");
    else            printf("Stack (BAD), error - %d\n", error);
                    printf("start canary - %ãll\n", (*stack).start_canary);
                    printf("adress - [%X]\n", (int)stack);
                        printf("{\n");
                        printf("\tcapacity    = %d\n", (*stack).capacity);
                        printf("\tsize        = %d\n", (*stack).size);
                        printf("\tdestruction = %d\n", (*stack).destruction);
                        printf("\thash        = %d\n", (*stack).hash);
                        printf("\tbuf [%X]\n",(int)(*stack).buf);
                            printf("\t{\n");
                            for(int i = 0; i < (*stack).capacity;i++) printf("\t\t[%d]: %d\n", i, (*stack).buf[i]);
                            printf("\t}\n");
                        printf("}\n");

                    printf ("[][][][][][][][][][][][][][][][][][][][][][][][][][][]\n\n");


}

//-----------------------------------------------------------------------------

void stack_ok (STACK* stack, int place)
{
int error = errortest(*stack);
    if (place)
    {
        if ((*stack).hash != hash_maker(stack))
        {
            stack_dump(stack, hash_error);
            //printf("hasherror\n");
            exit(error);
        }
    }
    else (*stack).hash = hash_maker(stack);

    if(error != 0)
        {
        stack_dump(stack, error);
        exit(error);
        }
}

#include <stdlib.h>

#include "mem.h"

Stack* stackInit(size_t size) {
    Stack* s = (Stack*)malloc(sizeof(Stack));
    s->data = (int*)malloc(sizeof(int) * size);
    s->ptr = 0;
    return s;
}

int stackPop(Stack* stack) {
    return stack->data[stack->ptr--];
}

void stackPush(Stack* stack, int value) {
    stack->data[++stack->ptr] = value;
}

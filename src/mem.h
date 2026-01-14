#ifndef IXEC_MEM
#define IXEC_MEM

typedef struct {
    int* data;
    size_t ptr;
} Stack;

Stack* stackInit(size_t size);
int stackPop(Stack* stack);
void stackPush(Stack* stack, int value);

#endif

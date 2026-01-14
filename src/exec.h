#ifndef IXEC_EXEC
#define IXEC_EXEC

typedef unsigned char byte;

typedef enum {
    ADD,
    SUB,
    MUL,
    DIV,
    EQ,
    NEQ,
} IOp;

typedef enum {
    NOP,
    PUSH,
    POP_TOP,
    POS_ADD,
    POS_SUB,
    POS_ADD_IF_POP,
    POS_SUB_IF_POP,
    I_OP,
    PRINT,
    PRINTLN,
    PRINTCH
} Op;

typedef struct {
    Op op;
    int arg;
} Instr;

typedef struct {
    size_t stacksize;
    size_t codesize;
    Instr* code;
} Code;

Code codeLoads(char* path);
void codeExec(Code* code);

#endif

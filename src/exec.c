#include <stdio.h>
#include <stdlib.h>

#include "exec.h"
#include "mem.h"

size_t parse_size_t(byte* buff, size_t* pos) {
    unsigned char* ptr = buff + *pos;
    size_t val = *(size_t*)ptr;
    *pos += sizeof(size_t);
    return val;
}

byte parse_byte(byte* buff, size_t* pos) {
    byte val = (byte)buff[*pos];
    *pos += 1;
    return val;
}

int parse_int(byte* buff, size_t* pos) {
    unsigned char* ptr = buff + *pos;
    int val = *(int*)ptr;
    *pos += sizeof(int);
    return val;
}

Code codeLoads(char* path) {
    Code code = {0};
    FILE* file = fopen(path, "rb");
    if (!file) return code;

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);

    byte* buff = malloc(size);
    fread(buff, 1, size, file);
    fclose(file);

    size_t ptr = 0;
    code.stacksize = parse_size_t(buff, &ptr);

    code.codesize = parse_size_t(buff, &ptr);
    code.code = malloc(sizeof(Instr) * code.codesize);
    for (size_t i = 0; i < code.codesize; i++) {
        Instr instr = {
            (Op)parse_byte(buff, &ptr),
            parse_int(buff, &ptr),
        };
        code.code[i] = instr;
    }
    free(buff);
    return code;
}

int iopHandle(int a, IOp op, int b) {
    switch (op) {
        case ADD: return a + b;
        case SUB: return a - b;
        case DIV: return a / b;
        case MUL: return a * b;
        case EQ: return a == b;
        case NEQ: return a != b;
    }
    return 0;
}

void codeExec(Code* code) {
    size_t pos = 0;
    Stack* stack = stackInit(code->stacksize);

    while (pos < code->codesize) {
        Instr instr = code->code[pos];
        switch (instr.op) {
            case NOP:break;
            case PUSH:
                stackPush(stack, instr.arg);
                break;

            case POS_ADD:
                pos += instr.arg;
                break;

            case POS_SUB:
                pos -= instr.arg;
                break;

            case POS_ADD_IF_POP:
                if (stackPop(stack))
                    pos += instr.arg;
                break;

            case POS_SUB_IF_POP:
                if (stackPop(stack))
                    pos -= instr.arg;
                break;

            case POP_TOP:
                stackPop(stack);
                break;

            case I_OP: {
                int b = stackPop(stack);
                int a = stackPop(stack);
                stackPush(stack, iopHandle(a, (IOp)instr.arg, b));
                break;
            }

            case PRINT:
                printf("%i", stackPop(stack));
                break;

            case PRINTLN:
                printf("%i\n", stackPop(stack));
                break;

            case PRINTCH:
                printf("%c\n", (char)stackPop(stack));
                break;
        }
        pos += 1;
    }
}

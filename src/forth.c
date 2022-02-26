#include "forth.h"

void pushInt(int val)
{
    stackptr++;
    printf("Pushing value onto stack: %d (ptr %d)\n", val, stackptr);
    stack[stackptr] = val;
    printf("Stack at ptr %d is now %d\n", stackptr, stack[stackptr]);
}

int popInt()
{
    printf("Popping value %d off of stack\n", stack[stackptr]);
    int res = stack[stackptr];
    stack[stackptr] = 0;
    printf("Stack at ptr %d is now %d\n", stackptr, stack[stackptr]);
    stackptr--;
    return res;
}

// TODO: finish executing tokens
// TODO: find a way to enumerate program state
void executeToken(token current)
{
    if (strcmp(current.type, "num") == 0)
    {
        pushInt(atoi(current.value));
    }
    else if (strcmp(current.type, "op") == 0)
    {
        puts("Operator detected");
        int right = popInt();
        int left = popInt();
        if (strcmp(current.value, "+") == 0) pushInt(left + right);
        else if (strcmp(current.value, "-") == 0) pushInt(left - right);
        else if (strcmp(current.value, "*") == 0) pushInt(left * right);
        else if (strcmp(current.value, "/") == 0) pushInt(left / right);
        else if (strcmp(current.value, "mod") == 0) pushInt(left % right);
    }
    else if (strcmp(current.type, "dot") == 0)
    {
        printf("cforth: %d\n", popInt());
    }
}
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
    if (state == DEF)
    {
        if (strcmp(current.type, "num") == 0)
        {
            pushInt(atoi(current.value));
        }
        else if (strcmp(current.type, "op") == 0)
        {
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
        else if (strcmp(current.type, "colon") == 0)
        {
            state = WORD;
        }
        else if (strcmp(current.type, "str") == 0)
        {
            state = STR;
            printf("cforth: ");
        }
    }
    else if (state == WORD)
    {
        if (strcmp(current.type, "semcolon") == 0 && state == WORD)
        {
            state = DEF;
        }
    }
    else if (state == STR)
    {
        char *to_print = malloc(sizeof(char) * strlen(current.value)); // So that we can modify it without mutating the original string
        strcpy(to_print, current.value);
        int offset = strcspn(to_print, "\"");
        if (offset < strlen(to_print))
        {
            to_print[offset] = '\n';
            state = DEF;
        }
        else to_print[offset] = ' ';
        printf("%s", to_print);
        free(to_print);
    }
}
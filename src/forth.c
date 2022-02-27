#include "forth.h"

void pushInt(int val)
{
    stackptr++;
    // printf("Pushing value onto stack: %d (ptr %d)\n", val, stackptr);
    stack[stackptr] = val;
    // printf("Stack at ptr %d is now %d\n", stackptr, stack[stackptr]);
}

int popInt()
{
    // printf("Popping value %d off of stack\n", stack[stackptr]);
    int res = stack[stackptr];
    stack[stackptr] = 0;
    // printf("Stack at ptr %d is now %d\n", stackptr, stack[stackptr]);
    stackptr--;
    return res;
}

// TODO: finish executing tokens
// TODO: find a way to enumerate program state
void executeToken(token current)
{
    if (state == DEF || state == EXEC)
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
            finished = 0;
        }
        else if (strcmp(current.type, "str") == 0)
        {
            state = STR;
            printf("cforth: ");
        }
        else if (strcmp(current.type, "default") == 0)
        {
            for (int i = 0; i < wordidx; i++)
            {

                printf("compare %s to %s (i:%d) results %d\n", current.value, words[i].name, i, strcmp(current.value, words[i].name));
                if (strcmp(current.value, words[i].name) == 0)
                {
                    printf("Found defined word %s\n", words[i].name);
                    for (int x = 0; x < words[i].defcount; x++)
                    {
                        printf("executing %s(%s)\n", words[i].def[x].type, words[i].def[x].value);
                        executeToken(words[i].def[x]);  // Sorta recursive..?
                    }
                }
            }
        }
    }
    else if (state == WORD)
    {
        if (!initialized) // Static word w doesn't exist, this is our first time defining a word
        {
            w.name = NULL;
            w.def = NULL;
            w.defcount = 0;
            initialized = 1;
        }
        if (!finished) // w exists but we aren't finished
        {
            if (w.name == NULL) // This is the first pass
            {
                w.name = malloc(sizeof(char) * strlen(current.value));
                strcpy(w.name, current.value);

                w.def = malloc(sizeof(token) * DEFWTOKENS);
                w.defcount = 0;
                printf("Defined new word %s\n", w.name);
            }
            else    // We already gave the word a name, so add the token to the list 
            {
                if (strcmp(current.type, "semcolon") == 0) // Don't add this to the list, just finish the word up
                {
                    words[wordidx++] = w;
                    free(w.name);
                    free(w.def);
                    w.name = NULL;
                    w.def = NULL;
                    w.defcount = 0;
                    finished = 1;
                    state = DEF;
                }
                else // Add the token to the word
                {
                    printf("Added token to %s: %s(%s)\n", w.name, current.type, current.value);
                    w.def[w.defcount].type = malloc(sizeof(char) * strlen(current.type));
                    strcpy(w.def[w.defcount].type, current.type);
                    w.def[w.defcount].value = malloc(sizeof(char) * strlen(current.value));
                    strcpy(w.def[w.defcount].value, current.value);
                    printf("Resulting word now has %s(%s)\n", w.def[w.defcount].type, w.def[w.defcount].value);
                    w.defcount++;
                    words[wordidx] = w;
                    wordidx++;
                    printf("words list wordidx(%d): %s\n", wordidx - 1, words[wordidx - 1].name);
                }
            }
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
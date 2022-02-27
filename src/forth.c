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
            // printf("String set state to %d\n", state);
            printf("cforth: ");
        }
        else if (strcmp(current.type, "default") == 0)
        {
            for (int i = 0; i < wordidx; i++)
            {
                // printf("token is %s(%s), word[%d] is %s\n", current.type, current.value, i, words[i].name);
                // printf("compare %s to %s (i:%d) results %d\n", current.value, words[i].name, i, strcmp(current.value, words[i].name));
                if (strcmp(current.value, words[i].name) == 0)
                {
                    
                    // printf("Found defined word %s\n", words[i].name);
                    for (int x = 0; x < words[i].defcount; x++)
                    {   
                        if (state == DEF) state = EXEC; // Only switch states if we are in default state (i.e. retain if/else/loop/str states)
                        // printf("executing %s(%s), state %d\n", words[i].def[x].type, words[i].def[x].value, state);
                        executeToken(words[i].def[x]);  // Sorta recursive..?
                        if (state == EXEC) state = DEF; // Only switch back if the word didn't modify the state
                    }
                    
                }
            }
        }

        if (state == EXEC)
        {
            // puts("Exec state");
            // printf("compare %s to if returns %d\n", current.value, strcmp(current.value, "if"));
            if (strcmp(current.value, "if") == 0)
            {
                // puts("Encountered if statement\n");
                if (popInt() != 0)
                {
                    // puts("Going into if statement");
                    state = IF;
                }
                else
                {
                    // puts("Skipping until else");
                    state = SKIP;
                }
            }
        }
    }
    else if (state == WORD)
    {
        if (!finished) // w exists but we aren't finished
        {
            if (words[wordidx].name == NULL) // This is the first pass
            {
                words[wordidx].name = malloc(sizeof(char) * strlen(current.value));
                strcpy(words[wordidx].name, current.value);

                words[wordidx].def = malloc(sizeof(token) * DEFWTOKENS);
                words[wordidx].defcount = 0;
                //printf("Defined new word %s\n", words[wordidx].name);
            }
            else    // We already gave the word a name, so add the token to the list 
            {
                if (strcmp(current.type, "semcolon") == 0) // Don't add this to the list, just finish the word up
                {
                    wordidx++;
                    //printf("words list wordidx(%d): %s\n", wordidx - 1, words[wordidx - 1].name);
                    finished = 1;
                    state = DEF;
                }
                else // Add the token to the word
                {
                    //printf("Added token to %s: %s(%s)\n", words[wordidx].name, current.type, current.value);
                    words[wordidx].def[words[wordidx].defcount].type = malloc(sizeof(char) * strlen(current.type));
                    strcpy(words[wordidx].def[words[wordidx].defcount].type, current.type);
                    words[wordidx].def[words[wordidx].defcount].value = malloc(sizeof(char) * strlen(current.value));
                    strcpy(words[wordidx].def[words[wordidx].defcount].value, current.value);
                    //printf("Resulting word now has %s(%s)\n", words[wordidx].def[words[wordidx].defcount].type, words[wordidx].def[words[wordidx].defcount].value);
                    words[wordidx].defcount++;
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
    else if (state == IF)   // Encountered an if token while executing a word and executing up until else clause
    {
        if (strcmp(current.value, "else") != 0)
        {
            // printf("Current token %s(%s) isn't else and cond is true\n", current.type, current.value);
            if (state == IF || state == DEF) state = EXEC;
            // printf("State is now %d\n", state);
            executeToken(current);
            if (state == DEF || state == EXEC) state = IF;
            // printf("State is now %d\n", state);
        }
        else
        {
            state = SKIP;
        }
    }
    else if (state == SKIP)
    {
        if (strcmp(current.value, "then") == 0)
        {
            state = EXEC;
        }
        else if (strcmp(current.value, "else") == 0)
        {
            state = ELSE;
        }
    }
    else if (state == ELSE)
    {
        if (strcmp(current.value, "then") != 0)
        {
            if (state == ELSE || state == DEF) state = EXEC;
            executeToken(current);
            if (state == DEF || state == EXEC) state = ELSE;
        }
        else
        {
            state = EXEC;
        }
    }
}
#include <stdio.h>
#include "../slap/src/token.h"
#include "../slap/src/gram.h"
#include "forth.h"

#define MAXRULES 64

// TODO: implement stack structure
// TODO: read every token line by line
// TODO: Figure out how the interpreter is going to interpret the tokens

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        puts("Invalid arg count");
        return 1;
    }

    FILE *grammar = fopen(argv[2], "r");
    FILE *code = fopen(argv[1], "r");

    if (grammar == NULL)
    {
        printf("File %s failed to open", argv[2]);
    }

    if (code == NULL)
    {
        printf("File %s failed to open", argv[1]);
    }


    gram_rule g = lineFromFileToRule(grammar);
	gram_rule *rules = malloc(sizeof(grammar) * MAXRULES);
	int count = 0;
    while (strcmp(g.type_name, "end") != 0) {
		if (count < MAXRULES) rules[count++] = g;
        g = lineFromFileToRule(grammar);
    }

    int tcount = 0;
    int tsize = 1024;
    token *tokens = malloc(sizeof(token) * tsize);
    char buff[64];
    while (fscanf(code, "%64s", buff) == 1)
    {
        token current = wordToToken(buff, rules, count);
        if (tcount < tsize) tokens[tcount++] = current;
        else
        {
            token *tokens = realloc(tokens, tsize * 2);
            tsize *= 2;
        }
        printf("{ %s:%s }\n", current.type, current.value);
    }

    stackptr = -1;

    for (int i = 0; i < tcount; i++)
    {
        printf("Step:");
        getc(stdin);
        puts("");
        executeToken(tokens[i]);
        for (int i = 0; i < stackptr + 8; i++)
        {
            printf("Value on stack pointer at %d is %d\n", i, stack[i]);
        }
    }

    

    fclose(grammar);
    fclose(code);
    return 0;
}

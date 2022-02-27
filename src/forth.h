#ifndef __FORTH_H_
#define __FORTH_H_

#include "../slap/src/token.h"
#include "string.h"
#include "stdlib.h"

#define STACKSIZE 1024

int stack[STACKSIZE];
int stackptr;

typedef enum State
{
    DEF,
    WORD,
    IF,
    LOOP,
    STR
} State;

State state;

typedef struct word
{
    token *def;
    int defcount;
} word;

void pushInt(int val);
int popInt();
void executeToken(token current);

#endif // __FORTH_H_

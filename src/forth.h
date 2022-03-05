#ifndef __FORTH_H_
#define __FORTH_H_

#include "../slap/src/token.h"
#include "string.h"
#include "stdlib.h"

#define STACKSIZE 1024
#define MAXWORDS 1024
#define DEFWTOKENS 64

int stack[STACKSIZE];
int stackptr;

typedef enum State
{
    DEF,
    WORD,
    EXEC,
    IF,
    SKIP,
    ELSE,
    LOOP,
    STR
} State;

State state;

typedef struct word
{
    char *name;
    token *def;
    int defcount;
} word;

int wordidx;
word words[MAXWORDS];
int finished;

void pushInt(int val);
int popInt();
void executeToken(token current);

token *looptokens;
int looptokensfull;
int looptokcount;
int space;
int loopidx;
int looplim;

#endif // __FORTH_H_

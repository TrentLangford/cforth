#ifndef __FORTH_H_
#define __FORTH_H_

#include "../slap/src/token.h"
#include "string.h"
#include "stdlib.h"

#define STACKSIZE 1024

#ifndef STACK
#define STACK
int stack[STACKSIZE];
int stackptr;
#endif

void pushInt(int val);
int popInt();
void executeToken(token current);

#endif // __FORTH_H_

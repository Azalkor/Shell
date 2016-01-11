#ifndef _EVALUATION_H
#define _EVALUATION_H

#include "Shell.h"
#include <sys/wait.h>

extern int evaluer_expr(Expression *e);
void verifier(int cond, char *s);
int evaluer_simple(Expression *e);


#endif

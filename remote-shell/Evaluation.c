#include "Shell.h"
#include "Evaluation.h"
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>



void
verifier(int cond, char *s)
{
  if (!cond)
    {
      perror(s);
      exit(1);
    }
}

void evaluer_simple(Expression *e){
	execvp(e->arguments[0], e->arguments);
	perror("exec");
	exit(1);
}

int
evaluer_expr(Expression *e)
{
    switch (e->type){
    case SIMPLE :
      {
    if(fork()==0){
      execvp(e->arguments[0],e->arguments);
      perror("exec");exit(1);
      break;
    }
    else
      wait(NULL);
    break;
      }
    case PIPE :
      {
    break;
      }
    case REDIRECTION_O :
      {
    if(fork()==0){
      int fd = open(e->arguments[0], O_CREAT | O_WRONLY,0644);
      dup2(fd,1); // est la sortie standard
      execvp(e->gauche->arguments[0],e->gauche->arguments);
      perror("exec");exit(1);
      close(fd);
    }
    else
      wait(NULL);
    break;
      }
    case BG :
      {
    int pidBG = fork();
    if(pidBG == 0)
      {
        execvp(e->gauche->arguments[0],e->gauche->arguments);
        perror("exec");exit(1);
      }
    break;
      }
    default :
      printf("\n");
    }
  return 1;
}

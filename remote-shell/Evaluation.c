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
  switch(e->type){
  case SIMPLE :
    if(fork()==0){
      evaluer_simple(e);
    }
    else{
      wait(NULL);
    }
    break;
  case REDIRECTION_O :
    {
      if(fork()==0){
	int fd= open(e->arguments[0], O_CREAT | O_RDWR, 0777);
	dup2(fd,1);
	evaluer_simple(e->gauche);
	close(fd);
      }
      else{
	wait(NULL);
      }
      break;
    }
  case BG :
    {
      int pid=fork();
      if(pid==0){
	evaluer_simple(e->gauche);
      }
      break;
    }
  case PIPE :
    {
      if(fork()==0){
	int p[2];
	pipe(p);
	if(fork()==0){
	  dup2(p[0],0);
	  evaluer_simple(e->droite);
	}
	else{
	  dup2(p[1],1);
	  evaluer_simple(e->gauche);
	  wait(NULL);
	}
      }
      else{
	wait(NULL);
      }
      break;
    }
  case VIDE :
    printf("\n");
    break;
  default :
    printf("instruction invalide\n");
  }

  return 1;
}

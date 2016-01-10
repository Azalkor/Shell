#include "Shell.h"
#include "Evaluation.h"
#include "Commandes_Internes.h"
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>


void verifier(int cond, char *s){
  if (!cond){
      perror(s);
      exit(1);
    }
}

int evaluer_simple(Expression *e){
  //printf("%s %s\n", e->arguments[0], e->arguments[1]);
  fflush(stdout);
  if (execvp(e->arguments[0], e->arguments) == -1){
    //perror("exec");
    fprintf(stderr, "execution impossible \n");
    exit(-1);
  }
  exit(0);
}

int evaluer_expr(Expression *e){
  int status = 0;
  if(!is_interne(e)){
    switch(e->type){
    case VIDE :
      printf("\r");
      break;
    case SIMPLE :
      {
	pid_t pid = fork();
	verifier(pid != -1, "erreur fork SIMPLE \n");
	if (pid == 0){
	  evaluer_simple(e);
	}
	else{
	  waitpid(pid,&status,0);
	}
	break;
      }
    case SEQUENCE :
      evaluer_expr(e->gauche);
      evaluer_expr(e->droite);
      break;
    case SEQUENCE_ET :
      if (evaluer_expr(e->gauche) == 0){
	status = evaluer_expr(e->droite);
      }
      break;
    case SEQUENCE_OU :
      /*pid_t pid = fork();
	verifier(pid != -1, "erreur fork \n");
	if (pid == 0){
	  if ((status = evaluer_expr(e->gauche)) != 0){
	    status = evaluer_expr(e->droite);
	  }
	  exit(0);
	}
	
	  else{
	  waitpid(pid,&status,0);
	  }*/
      if (evaluer_expr(e->gauche) != 0){
	status = evaluer_expr(e->droite);
      }
      break;
    case REDIRECTION_A :
      {
	pid_t pid = fork();
	verifier(pid != -1, "erreur fork REDIRECTION_A \n");		
	if(pid == 0){
	  int fd= open(e->arguments[0], O_CREAT | O_WRONLY | O_APPEND, 0644);
	  dup2(fd,1);
	  evaluer_simple(e->gauche);
	  close(fd);
	}
	else{
	  wait(&status);
	}
	break;
      }
    case REDIRECTION_O :
      {
	pid_t pid = fork();
	verifier(pid != -1, "erreur fork REDIRECTION_O \n");		
	if(pid == 0){
	  int fd= open(e->arguments[0], O_CREAT | O_WRONLY, 0644);
	  dup2(fd,1);
	  evaluer_simple(e->gauche);
	  close(fd);
	}
	else{
	  wait(&status);
	}
	break;
      }
    case REDIRECTION_E :
      {
	pid_t pid = fork();
	verifier(pid != -1, "erreur fork REDIRECTION_E \n");		
	if(pid == 0){
	  int fd= open(e->arguments[0], O_CREAT | O_WRONLY, 0644);
	  dup2(fd,2);
	  evaluer_simple(e->gauche);
	  close(fd);
	}
	else{
	  wait(&status);
	}
	break;
      }
    case REDIRECTION_EO :
      {
	pid_t pid = fork();
	verifier(pid != -1, "erreur fork REDIRECTION_EO \n");		
	if(pid == 0){
	  int fd= open(e->arguments[0], O_CREAT | O_WRONLY, 0644);
	  dup2(fd,1);
	  dup2(fd,2);
	  evaluer_simple(e->gauche);
	  close(fd);
	}
	else{
	  wait(&status);
	}
	break;
      }
    case REDIRECTION_I :
      {
	pid_t pid = fork();
	verifier(pid != -1, "erreur fork REDIRECTION_I \n");		
	if(pid == 0){
	  int fd= open(e->arguments[0], O_CREAT | O_RDONLY, 0644);
	  dup2(fd,0);
	  evaluer_simple(e->gauche);
	  close(fd);
	}
	else{
	  wait(&status);
	}
	break;
      }
    case BG :
      {
	pid_t pid = fork();
	verifier(pid != -1, "erreur fork BG \n");		
	if(pid == 0){
	  printf("[%d] %s démarré \n", getpid(), e->gauche->arguments[0]);
	  evaluer_simple(e->gauche);
	}
	else{
	  usleep(10);
	}
	break;
      }
    case PIPE :
      {	
	pid_t pid1 = fork();
	verifier(pid1 != -1, "erreur fork PIPE gauche \n");
	if (pid1 == 0){
	  int p[2];
	  pipe(p);
	  pid_t pid2 = fork();
	  verifier(pid2 != -1, "erreur fork PIPE droite\n");
	  if (pid2 == 0){
	    dup2(p[0],0);
	    evaluer_simple(e->droite);
	  }
	  else{
	    dup2(p[1],1);
	    evaluer_simple(e->gauche);
	    waitpid(pid2,&status,0);
	  }
	}
	else{
	  waitpid(pid1,&status,0);
	  usleep(10);
	}
	break;
      }
    case SOUS_SHELL :
      {
	pid_t pid = fork();
	verifier(pid != -1, "erreur fork SOUS_SHELL \n");
	if (pid == 0){
	  evaluer_expr(e->gauche);
	  exit(0);
	}
	else{
	  waitpid(pid,&status,0);
	}
	break;
      }	
    default :
      status = -1;
      printf("instruction invalide\n");
    }
  }
  else{
    return 0;
  }
  return status;
}

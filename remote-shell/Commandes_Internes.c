#include "Commandes_Internes.h"
#include "Evaluation.h"
#include <time.h>
#include <readline/history.h>
#include <signal.h>
#include <stdio.h>
#define NB_MAX_MACHINES 10

typedef struct machine{
	pid_t pid;
	pid_t pid_xcat;
	FILE * fdE;
	int fdS;
	char * nom;
}*machine;

machine machines[NB_MAX_MACHINES];
int nbMachines=0;
 
bool is_interne(Expression *e){
  if(e->type == SIMPLE){
    if(strcmp(e->arguments[0], "echo") == 0){
      my_echo(e->arguments+1);
    } 
    else if(strcmp (e->arguments[0], "date") == 0){
      my_date();
    }
    else if(strcmp(e->arguments[0], "cd") == 0){
      my_cd(e->arguments[1]);
    }  
    else if(strcmp (e->arguments[0], "pwd") == 0){
      my_pwd();
    }
    else if(strcmp(e->arguments[0], "history") == 0){
      my_history();
    }  
    else if(strcmp (e->arguments[0], "hotsname") == 0){
      my_hostname();
    }
    else if(strcmp(e->arguments[0], "kill") == 0){
      my_kill(atoi(e->arguments[1]),e->arguments[2]);
    }  
    else if(strcmp (e->arguments[0], "exit") == 0){
      my_exit();
    }
    else if(strcmp (e->arguments[0], "remote") == 0){
      my_remote(e);
    }
    else
      return false ;
    return true;
  }
}

void my_echo(char ** args){
  while ((*args)!=NULL){
    printf("%s ",(*args));
    args++;
  }
  printf("\n");
}

void my_date(){
  time_t ts = time(NULL);
  struct tm * t = localtime(&ts);
  char * mois[] = {"Janvier", "Février", "Mars", "Avril", "Mai", "Juin",
		   "Juillet", "Août", "Septembre", "Octobre", "Novembre", "Décembre"};
  char * jours[] = {"Dimanche", "Lundi", "Mardi", "Mercredi", "Jeudi", "Vendredi", "Samedi"};
  printf("%s %d %s %d, %d:%d:%02d (UTC+0100) \n", jours[t->tm_wday], t->tm_mday, mois[t->tm_mon], 1900+t->tm_year, t->tm_hour, t->tm_min, t->tm_sec);
}

void my_cd(char * arg){
  if(arg !=NULL){
    chdir(arg);
  }
  else{
    chdir(getenv("HOME"));
  }
}

void my_pwd(){
  char buf[TAILLE_ID];
  getcwd(buf,TAILLE_ID);
  printf("%s \n", buf);
}

void my_history(){
  HISTORY_STATE * hs = history_get_history_state();
  for(int i =0; i<hs->length; i++){
    HIST_ENTRY * he = hs->entries[i];
    printf("%d %s \n", i+1, he->line);
    }
}

void my_hostname(){
  char buf[TAILLE_ID];
  gethostname(buf,TAILLE_ID);
  printf("%s \n", buf);
}

void my_kill(int pid, char * sig){
  if(sig == NULL){
    sig = "9";
  }
  kill(pid,atoi(sig));
}

void my_exit(){
  exit(0);
}

void my_remote(Expression * e){
  if(strcmp(e->arguments[1], "all") == 0){
    remote_all(e->arguments+2);
  } 
  else if(strcmp (e->arguments[1], "add") == 0){
    remote_add(e->arguments+2);
  }
  else if(strcmp (e->arguments[1], "remove") == 0){
    remote_remove();
  }
  else if(strcmp (e->arguments[1], "list") == 0){
    remote_list();
  }
  else{
    remote_cmd(e->arguments[1],e->arguments+2);
  }
}

void remote_all(char ** args){
  for(int i=0; i<nbMachines; i++){
				remote_cmd(machines[i]->nom, args);
	}

}

void remote_add(char ** args){
	for (int i=nbMachines; (*args)!=NULL && i<NB_MAX_MACHINES; i++){
		machines[i] = malloc(sizeof(struct machine));
		int pE[2];
		int pS[2];
		pipe(pE);
 		pipe(pS);
		pid_t pid = fork();
 		verifier(pid != -1, "erreur fork REMOTE_ADD \n");
 		if(pid == 0){
			close(pE[1]);
			dup2(pE[0],0);
			close(pE[0]);
			
			close(pS[0]);
			dup2(pS[1],1);
			close(pS[1]);
			
			execlp("bash","bash");
			exit(0);
    }
    else{
    	machines[i]->pid=pid;
    	machines[i]->nom=malloc(sizeof(char)*strlen(*args));
    	strcpy(machines[i]->nom,*args);
    	machines[i]->fdE=fdopen(pE[1],"w");
    	machines[i]->fdS=pS[0];
    	nbMachines++;
    	pid_t pid_xcat = fork();
    	verifier(pid_xcat != -1, "erreur fork xcat \n");
 			if(pid_xcat == 0){
    		dup2(machines[i]->fdS, 0);
    		execl("xcat.sh", "xcat", NULL);
    		perror("execl xcat");
    		exit(1);
    	}
    	else{
    		machines[i]->pid_xcat=pid_xcat;
    	}
    }
  args++; 
  } 
}

void remote_remove(){
  for(int i=0;i<nbMachines;i++){
  	kill(machines[i]->pid,9);
  	kill(machines[i]->pid_xcat,9);
  	free(machines[i]->nom);
  	free(machines[i]);
  	machines[i]=NULL;
  }
  nbMachines=0;
  
}

void remote_list(){
  for(int i=0;i<NB_MAX_MACHINES;i++){
  	if(machines[i]!=NULL){
  		printf("[%d] %s\n",machines[i]->pid, machines[i]->nom);
  	}
  }
}

void remote_cmd(char * m, char ** args){
	for(int i=0;i<nbMachines;i++){
		if(strcmp(m, machines[i]->nom)==0){
			while(*args !=NULL){
				fwrite(*(args),1,strlen(*(args)),machines[i]->fdE);
				fwrite(" ",1,1,machines[i]->fdE);
				args++;
			}
			fwrite("\n",1,1,machines[i]->fdE);
			fflush(NULL);
		}
	}
}



















#include "Commandes_Internes.h"
#include <time.h>
#include <readline/history.h>
#include <signal.h>


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
  time_t  ts = time(NULL);
  struct tm * t = localtime(&ts);
  char * mois[] = {"Janvier", "Février", "Mars", "Avril", "Mai", "Juin",
		   "Juillet", "Août", "Septembre", "Octobre", "Novembre", "Décembre"};
  char * jours[] = {"Dimanche", "Lundi", "Mardi", "Mercredi", "Jeudi", "Vendredi", "Samedi"};
  printf("%s %d %s %d, %d:%d:%02d (UTC+0100)\n", jours[t->tm_wday], t->tm_mday, mois[t->tm_mon], 1900+t->tm_year, t->tm_hour, t->tm_min, t->tm_sec);
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

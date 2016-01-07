#include "Commandes_Internes.h"
#include <string.h>
#include <time.h>


bool is_interne(Expression *e){
  if(e->type == SIMPLE){
    if(strcmp(e->arguments[0], "echo") == 0){
      my_echo(e->arguments+1);
    } 
    else if(strcmp (e->arguments[0], "date") == 0){
      my_date();
    }
    else if(strcmp(e->arguments[0], "cd") == 0){
      my_cd(e);
    }  
    else if(strcmp (e->arguments[0], "pwd") == 0){
      my_pwd(e);
    }
    else if(strcmp(e->arguments[0], "history") == 0){
      my_history(e);
    }  
    else if(strcmp (e->arguments[0], "hotsname") == 0){
      my_hostname(e);
    }
    else if(strcmp(e->arguments[0], "kill") == 0){
      my_kill(e);
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
  char * mois[] = {"Janvier", "Février", "Mars", "Avril", "Mai", "Juin", "Juillet", "Août", "Septembre", "Octobre", "Novembre", "Décembre"};
  char * jours[] = {"Dimanche", "Lundi", "Mardi", "Mercredi", "Jeudi", "Vendredi", "Samedi"};
  printf("%s %d %s %d, %d:%d:%02d (UTC+0100)\n", jours[t->tm_wday], t->tm_mday, mois[t->tm_mon], 1900+t->tm_year, t->tm_hour, t->tm_min, t->tm_sec);
}

void my_cd(){

}

void my_pwd(){

}

void my_history(){

}

void my_hostname(){

}

void my_kill(){

}

void my_exit(){
  exit(0);
}

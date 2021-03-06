#ifndef COMINTERN_H
#define COMINTERN_H
#include "Shell.h"
#include <stdbool.h>

bool is_interne(Expression *e);
void my_echo(char ** args);
void my_date();
void my_cd(char * arg);
void my_pwd();
void my_history();
void my_hostname();
void my_kill(int pid, char * sig);
void my_exit();
void my_remote(Expression * e);
void remote_all(char ** args);
void remote_add(char ** args);
void remote_remove();
void remote_list();
void remote_cmd(char * m, char ** args);


#endif

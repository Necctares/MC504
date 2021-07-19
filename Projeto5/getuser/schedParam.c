#include <sys/syscall.h>
#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
#include <unistd.h>

int main(){
  struct sched_param *temp = malloc(sizeof(struct sched_param));
  int ret_value;

  ret_value = syscall(155, 0, temp); //Chama sys_sched_getparam
  printf("Valor inicial do parametro: %d\n", temp->sched_priority);
  
  temp->sched_priority = 2;
  ret_value = syscall(154, 0, temp); //Chama sys_sched_setparam
  printf("Valor final do parametro: %d\n", temp->sched_priority);

  return 0;
}

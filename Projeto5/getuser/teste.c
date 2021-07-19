#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>

#define __NR_testcall 430

int main() {
  long int retorno = syscall(__NR_testcall);
  printf("Retorno da chamada de sistema: %ld.\n", retorno);
  return 0;
}
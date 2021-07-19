#include <unistd.h>
#include <sys/syscall.h>
#include <stdio.h>
#include <errno.h>

#define SYS_getweight 428 //Syscall que pega o peso atual do usuario
#define SYS_setweight 429 //Syscall que atribui um novo peso para o usuario
#define SYS_testcall 430  //Syscall que reseta todos os pesos para o valor default (10)

int main(int argc, char **argv)
{
  int test_id, test_weight, return_value;
  errno = 0;

  return_value = syscall(SYS_testcall);
  printf("User_Weight inicializado com valores default. Syscall ID: %d\n", return_value);

  printf("Digite user id:\n");
  scanf("%d", &test_id);
  return_value = syscall(SYS_getweight, test_id);
  printf("Peso atual: %d", return_value);

  if (return_value != -1)
    printf("\nPeso do usuario %d: %d\nDigite novo peso: ", test_id, return_value);
  else
  {
    printf("\nErro na chamada de sistema(ERROR NUMBER %d)\n", errno);
    return 0;
  }

  scanf("%d", &test_weight);

  return_value = syscall(SYS_setweight, test_id, test_weight);

  if (return_value != -1)
    printf("\nPeso alterado com sucesso.\n");
  else
    printf("\nErro na chamada de sistema(ERROR NUMBER %d)\n", errno);

  return 0;
}

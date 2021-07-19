#include <unistd.h>
#include <sys/syscall.h>
#include <stdio.h>
#include <errno.h>

#define SYS_getweight 428 //Syscall que pega o peso atual do usuario
#define SYS_setweight 429 //Syscall que atribui um novo peso para o usuario

int main(int argc, char **argv)
{
    int test_id, test_weight, return_value;
    errno = 0;

    printf("Digite user id:\n");
    scanf("%d", &test_id);
    return_value = syscall(SYS_getweight, test_id);

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
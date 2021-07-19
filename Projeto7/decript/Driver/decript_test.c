#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include "decriptioctl.h"

void get_instructions(int inst[], int qnt, int fd)
{
	decript_arg_t dec;
	for (int i = 0; i < qnt; i++) {
		dec.comando = inst[i];
		if (ioctl(fd, DECRIPT_SET_COMMAND, &dec) == -1) {
			perror("decript test ioctl set command");
		} else {
			if (ioctl(fd, DECRIPT_GET_INSTRUCT, &dec) == -1) {
				perror("decript test ioctl get instruction");
			} else {
				printf("Cmd: %s", dec.instrucao);
			}
		}
	}
	return;
}

int main()
{
	int num_inst;
	int inst[20];
	int fd;
	char *dir = "/dev/decriptor";
	printf("Digite o numero de instrucoes (Max 20): ");
	scanf("%d", &num_inst);
	while (num_inst > 20) {
		printf("\nNao eh possivel realizar mais de 20 instrucoes.\n");
		printf("Digite o numero de instrucoes (Max 20): ");
		scanf("%d", &num_inst);
	}
	printf("\n");
	for (int i = 0; i < num_inst; i++) {
		printf("Digite a instrucao %d: ", i);
		scanf("%d", &inst[i]);
	}
	fd = open(dir, O_RDWR);
	get_instructions(inst, num_inst, fd);
	close(fd);
	return 0;
}
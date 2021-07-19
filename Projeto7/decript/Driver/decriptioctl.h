#ifndef DECRIPTIOCTL_H
#define DECRIPTIOCTL_H
#include <linux/ioctl.h>

typedef struct {
	int comando;
	char instrucao[17];
} decript_arg_t;

#define DECRIPT_SET_COMMAND _IOW('q', 1, decript_arg_t *)
#define DECRIPT_GET_INSTRUCT _IOR('q', 2, decript_arg_t *)

#endif
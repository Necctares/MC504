#include <linux/kernel.h>
#include <linux/syscalls.h>

#define USER_ARRAY_SIZE 65535
#define USER_DEFAULT_WEIGHT 10
extern int user_weight[USER_ARRAY_SIZE];

SYSCALL_DEFINE0(testcall)
{
	int i = 0;
	for (i = 0; i < USER_ARRAY_SIZE; i++) {
		user_weight[i] = USER_DEFAULT_WEIGHT;
	}
	return 430;
}
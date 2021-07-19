#include <linux/resource.h>
#include <linux/syscalls.h>
#include <linux/kernel.h>
#include <linux/cred.h>

#define USER_ARRAY_SIZE 65535
extern int user_weight[USER_ARRAY_SIZE];

SYSCALL_DEFINE2(setuserweight, int, uid, int, weight)
{
	if (uid < -1 || uid > 65535) {
		return -22;
	}
	if (uid == -1) {
		uid = get_current_cred()->uid.val;
	}
	if (get_current_cred()->uid.val != 0) { // root sempre tem uid = 0
		return -13;
	}
	user_weight[uid] = weight;
	return 0;
}
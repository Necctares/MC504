#include <linux/resource.h>
#include <linux/unistd.h>
#include <linux/syscalls.h>
#include <linux/kernel.h>
#include <linux/cred.h>

#define USER_ARRAY_SIZE 65535
int user_weight[USER_ARRAY_SIZE];
EXPORT_SYMBOL(user_weight);

SYSCALL_DEFINE1(getuserweight, int, uid)
{
	if (uid < -1 || uid > 65535) {
		return -22;
	} else {
		if (uid == -1) {
			uid = get_current_cred()->uid.val;
		}
		return user_weight[uid];
	}
}
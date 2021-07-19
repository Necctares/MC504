#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/kdev_t.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/string.h>
#include "decriptioctl.h"

static dev_t decript;
static struct cdev c_dev;
static struct class *cl;
static const char *coords[] = { "Direita\n",	  "Esquerda\n",
				"Cima\n",	  "Baixo\n",
				"Abra a caixa\n", "Aperte o botao\n",
				"Corra\n" };
static int comando = 0;
int i, contador;
char valor[20];

static ssize_t dec_read(struct file *f, char __user *buf, size_t len,
			loff_t *off)
{
	int pos;
	if (contador >= i) {
		return 0;
	}
	switch (valor[contador]) {
	case '0':
		pos = 0;
		break;
	case '1':
		pos = 1;
		break;
	case '2':
		pos = 2;
		break;
	case '3':
		pos = 3;
		break;
	case '4':
		pos = 4;
		break;
	case '5':
		pos = 5;
		break;
	case '6':
		pos = 6;
		break;
	default:
		return -EINVAL;
	}
	if (copy_to_user(buf, coords[pos], strlen(coords[pos]))) {
		return -EFAULT;
	} else {
		contador += 1;
		return strlen(coords[pos]);
	}
}
static ssize_t dec_write(struct file *f, const char __user *buf, size_t len,
			 loff_t *off)
{
	if (len > 20) {
		return -EINVAL;
	}
	if (copy_from_user(&valor, buf, len)) {
		return -EFAULT;
	}
	i = len;
	contador = 0;
	return len;
}

static long decript_ioctl(struct file *f, unsigned int cmd, unsigned long arg)
{
	decript_arg_t dec;
	switch (cmd) {
	case DECRIPT_SET_COMMAND:
		if (copy_from_user(&dec, (decript_arg_t *)arg,
				   sizeof(decript_arg_t))) {
			return -EACCES;
		}
		comando = dec.comando;
		break;
	case DECRIPT_GET_INSTRUCT:
		strcpy(dec.instrucao, coords[comando]);
		if (copy_to_user((decript_arg_t *)arg, &dec,
				 sizeof(decript_arg_t))) {
			return -EACCES;
		}
		break;
	default:
		return -EINVAL;
	}
	return 0;
}

static struct file_operations decript_fops = { .owner = THIS_MODULE,
					       .read = dec_read,
					       .write = dec_write,
					       .unlocked_ioctl =
						       decript_ioctl };

static int __init decript_init(void)
{
	printk(KERN_INFO "Ok: descript registrado com sucesso.");
	if (alloc_chrdev_region(&decript, 0, 1, "Decript") < 0) {
		return -1;
	}
	if ((cl = class_create(THIS_MODULE, "decriptdrv")) == NULL) {
		unregister_chrdev_region(decript, 1);
		return -1;
	}
	if (device_create(cl, NULL, decript, NULL, "decriptor") == NULL) {
		class_destroy(cl);
		unregister_chrdev_region(decript, 1);
		return -1;
	}
	cdev_init(&c_dev, &decript_fops);
	if (cdev_add(&c_dev, decript, 1) == -1) {
		device_destroy(cl, decript);
		class_destroy(cl);
		unregister_chrdev_region(decript, 1);
		return -1;
	}
	printk(KERN_INFO "<Major, Minor>: <%d, %d>\n", MAJOR(decript),
	       MINOR(decript));
	return 0;
}

static void __exit decript_exit(void)
{
	cdev_del(&c_dev);
	device_destroy(cl, decript);
	class_destroy(cl);
	unregister_chrdev_region(decript, 1);
	printk(KERN_INFO "ByeBye: Removido registro de descript com sucesso.");
}

module_init(decript_init);
module_exit(decript_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Alan F. Ribeiro");
MODULE_DESCRIPTION("Descriptografa mensagens");
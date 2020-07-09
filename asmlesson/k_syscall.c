#include <linux/module.h>
#include <linux/kernel.h>

/* 127 syscall */

void **sys_call_table;

asmlinkage static int sys_myprint(int number)
{
	return printk("%d\n", number);
}

static int __init msys_init(void)
{
	sys_call_table = 0xc0325500;
	sys_call_table[127] = sys_myprint;

	return 0;
}

static void __exit msys_exit(void)
{
	/* do nothing */
}

module_init(msys_init);
module_exit(msys_exit);

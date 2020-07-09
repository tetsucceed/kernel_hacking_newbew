#include <linux/module.h>
#include <linux/init.h>
#include <linux/gfp.h>

MODULE_LICENSE("GPL");

char *ch;

static int __init kmemalloc_init(void)
{
	ch = __get_free_page(GFP_KERNEL);
	sprintf(ch, "Hello.. %lu\n", jiffies);

	return 0;
}

static void __exit kmemalloc_exit(void)
{
        printk(KERN_INFO "%s", ch);
	free_page(ch);
}

module_init(kmemalloc_init);
module_exit(kmemalloc_exit);

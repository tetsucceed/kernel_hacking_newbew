#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/interrupt.h>

#define HANDLER_NAME "simple_handler"

irqreturn_t handler(int irq, void *arg)
{
	/* Handler code */
	printk(KERN_INFO "%s: %lu %s\n", HANDLER_NAME,
			jiffies, (char *) arg);

	return IRQ_HANDLED;
}

const int irq = 13;

static int __init handler_init(void)
{
	/* Initialization. Return 0 in the case of success, negative error code on failure */
	return request_irq(irq, handler, 0, "Weird_IRQ", "IRQ, Hello!");
}

static void __exit handler_exit(void)
{
	/* Deinitialisation */
	free_irq(irq, "Weird_IRQ");
	
}


module_init(handler_init);
module_exit(handler_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("George Vorobyev");
MODULE_DESCRIPTION("Simple IRQ handler");


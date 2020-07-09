#include <linux/module.h>
#include <linux/fs.h>
#include <linux/time.h>
#include <linux/init.h>
#include <linux/string.h>
#include <linux/smp_lock.h>
#include <linux/pagemap.h>
#include <asm/div64.h>
#include <linux/seq_file.h>
#include <linux/proc_fs.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Igor Markov");

#define get_gdt(ptr) asm("sgdt %0" : : "m" (ptr));

struct gdt_entry {
	u32 a;
	u32 b;
};

/* Memory operand layout for the SGDT/LGDT assembler instructions  */
struct xptr {
	u16 lim;
	struct gdt_entry *  addr;
} __attribute__((packed));

void * gdtshow_start (struct seq_file *m, loff_t *pos)
{
	struct xptr gdt;

	get_gdt(gdt);

	if (*pos > (gdt.lim+1)/sizeof(*gdt.addr))
		return NULL;

	return gdt.addr + *pos;
}

void gdtshow_stop (struct seq_file *m, void *v)
{
	/* Nothing to do */
}

void * gdtshow_next (struct seq_file *m, void *v, loff_t *pos)
{
	++*pos;
	/* It's convinient to re-use gdtshow_start. It's not always the case. */
	return gdtshow_start(m, pos);
}

int gdtshow_show (struct seq_file *m, void *v)
{
	struct gdt_entry * e = (struct gdt_entry *)v;
	u32 base = (e->a >> 16) + ((e->b & 255) << 16) + (e->b & 0xff000000);
	int type = (e->b >> 8) & 31;
	u32 lim = (e->a & 0xffff) + (e->b & 0xf0000);
	int dpl = (e->b >> 13) &3;
	if (test_bit(23, &e->b)) lim = (lim << 12) + 0xfff;
	
	return seq_printf(m, "%02x: base %08x limit %08x type %02x dpl %x, %s\n",
		(u32)v & 255, base, lim, type, dpl,
		test_bit(22, &e->b)? "32" : "16");
}


struct seq_operations gdtshow_ops = {
	.start = gdtshow_start,
	.stop = gdtshow_stop,
	.next = gdtshow_next,
	.show = gdtshow_show
};

static int gdtshow_open(struct inode *inode, struct file *file)
{
	return seq_open(file, &gdtshow_ops);
}

static struct file_operations gdtshow_file_operations = {
	.open		= gdtshow_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= seq_release,
};

struct proc_dir_entry * entry;

static int __init init_gdtshow(void)
{
	entry = create_proc_entry("GDT", 0444, NULL);
	if(entry)
		entry->proc_fops = &gdtshow_file_operations;

	return 0;
}

static void __exit exit_gdtshow(void)
{
	remove_proc_entry("GDT", NULL);
}

module_init(init_gdtshow)
module_exit(exit_gdtshow)

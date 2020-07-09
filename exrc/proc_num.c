#include <linux/module.h>
#include <linux/init.h>
#include <linux/list.h>
#include <linux/sched.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <asm/uaccess.h>

MODULE_LICENSE("GPL");

LIST_HEAD(head);

struct list_num {
	long num;
	struct list_head *list;
};

struct proc_dir_entry *proc_entry;
struct kmem_cache *proc_cache;
struct list_num *element;

static int is_read;

/*ssize_t (*write) (struct file *, const char __user *, size_t, loff_t *);*/

/* user write smth to /proc/magic_numbers */
static ssize_t proc_write(struct file *f, const char __user *udata,
	size_t howmuch, loff_t *pos)
{
	char local_udata[10];
	long local_num;
	struct list_num *ln;

	if (howmuch > 9)
		return -EINVAL;
	if (copy_from_user(local_udata, udata, howmuch))
		return -EFAULT;
	local_udata[howmuch] = '\0';

	local_num = simple_strtoul(local_udata, NULL, 10);
	printk(KERN_INFO "DEBUG: %lu\n", local_num);
	element->num = local_num;
	
	list_add(element->list, head);

	return howmuch;
}

/*
static ssize_t proc_read(struct file *f, const char __user *udata,
	size_t howmuch, loff_t *pos)
{
	return 0;
}
*/

static int proc_open(struct inode *ind, struct file *f)
{
	++is_read; 
	return seq_open(f, NULL);
}

static struct file_operations p_fops = {
	.open = proc_open,
	.read = seq_read,
	.llseek = seq_lseek,
	.write = proc_write,
	.release = seq_release
};

static int __init proc_num_init(void)
{
	proc_cache = kmem_cache_create("magic_cache", sizeof(struct list_num),
					0, 0, NULL);
	if(proc_cache == NULL) {
		printk(KERN_INFO "proc_num: cache_create fail\n");
		return -1;
	}

	element = (struct list_num *) kmem_cache_alloc(proc_cache, GFP_KERNEL);
	if (element == NULL)
		return -1;
	proc_entry = create_proc_entry("magic_numbers", 0644, NULL);
	if (proc_entry)
		proc_entry->proc_fops = &p_fops;

	printk(KERN_INFO "proc_num: loading\n");
	return 0;
}

static void __exit proc_num_exit(void)
{
	remove_proc_entry("magic_numbers", NULL);
	kmem_cache_free(proc_cache, element);
	printk(KERN_INFO "proc_num: unloading\n");
}

module_init(proc_num_init);
module_exit(proc_num_exit);

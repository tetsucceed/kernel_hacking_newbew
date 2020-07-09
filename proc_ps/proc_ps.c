#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/sched.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>

/*more information on http://tldp.org/LDP/lkmpg/2.6/html/x861.html */

MODULE_LICENSE("GPL");

struct lpids {
	pid_t pid;
	char comms[TASK_COMM_LEN];
	struct lpids *next;
};

struct lpids *list_lpids;
struct kmem_cache *lpids_cache;
struct proc_dir_entry *ps_entry;
static unsigned long counter;

void *ps_start(struct seq_file *m, loff_t *pos)
{
	struct task_struct *p;
	struct lpids *this;

	p = current;
	this = NULL;
	counter = 0;

	if(*pos == 0) {
		lpids_cache = kmem_cache_create("lpids_cache", sizeof(struct lpids), 0, 0, NULL);
		if(lpids_cache == NULL)
			return NULL;

		for_each_process(p) {
			list_lpids = (struct lpids *) kmem_cache_alloc(lpids_cache, GFP_KERNEL);

			list_lpids->next = this;
			this = list_lpids;

			list_lpids->pid = p->pid;
			strcpy(list_lpids->comms, p->comm);
		}
	} else {
		*pos = 0;
		return NULL;
	}

	return list_lpids;
}

void ps_stop(struct seq_file *m, loff_t *pos)
{
	struct lpids *this_lpid;

	while(list_lpids != NULL) {
		this_lpid = list_lpids;
		list_lpids = list_lpids->next;

		if(this_lpid != NULL) {
			kmem_cache_free(lpids_cache, this_lpid);

#ifdef __DEBUG
			printk(KERN_INFO "%s: destroy element of list\n", MODULE_NAME);
#endif

		}
	}
}

void *ps_next(struct seq_file *m, void *v, loff_t *pos)
{
	struct lpids *top;

	if(counter) {
		top = (struct lpids *) v;
		top = top->next;
		++*pos;
	} else {
		top = (struct lpids *) v;
		++counter;
		++*pos;
	}

	return top;
}

int ps_show(struct seq_file *m, void *v)
{
	struct lpids *local = (struct lpids *) v;

	return seq_printf(m, "process: %s pid: %d\n", local->comms, local->pid);
}

struct seq_operations ps_operations {
	.start = ps_start;
	.stop = ps_stop;
	.next = ps_next;
	.show = ps_show;
};

static int ps_open(struct inode *ind, struct file *fl)
{
	return seq_open(file, &ps_operations);
}

static struct file_operations ps_file_operations {
	.open = ps_open;
	.read = seq_read;
	.llseek = seq_lseek;
	.release = seq_release;
};

static int __init ps_init(void)
{
	ps_entry = create_proc_entry("proc_ps", 0666, NULL);
	if(ps_entry)
		ps_entry->proc_fops = &ps_file_operations;

	return 0;
}

static void __exit ps_exit(void)
{
	remove_proc_entry("proc_ps", NULL);
}

module_init(ps_init);
module_exit(ps_exit);
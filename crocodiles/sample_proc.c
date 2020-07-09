#include <linux/module.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/proc_fs.h>

MODULE_LICENSE("GPL");

#define FILE_NAME "crocodile"
#define SYMLINK_NAME "funny_crocodile"
#define DIR_NAME "greenkiller"

struct proc_dir_entry *gpd_entry, *sml_entry, *d_entry;
static unsigned long long j1, j2;

static int read_proc(char *page, char **start, off_t off, 
			int count, int *eof, void *data)
{
	int a;
	struct task_struct *this = current;
	j2 = jiffies;
	j2 = j2 - j1;

	a = sprintf(page, "pid: %d, comm: %s, jiffies: %lu\ndjiffes: %llu", this->pid, this->comm, jiffies, j2);

	*eof = 1;
	return a;
}
	

static int __init sample_proc_init(void)
{
	j1 = jiffies;
	d_entry = proc_mkdir(DIR_NAME, NULL);
	d_entry->uid = 1000;
	d_entry->gid = 1000;
	gpd_entry = create_proc_read_entry(FILE_NAME, 0644, d_entry, &read_proc, NULL);
	sml_entry = proc_symlink(SYMLINK_NAME, d_entry, "/proc/greenkiller/crocodile");
	
	return 0;
}

static void __exit sample_proc_exit(void)
{
	remove_proc_entry(SYMLINK_NAME, NULL);
	remove_proc_entry(DIR_NAME, NULL);
	remove_proc_entry(FILE_NAME, NULL);
}

module_init(sample_proc_init);
module_exit(sample_proc_exit);

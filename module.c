#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/sched/signal.h>

#define PROC_NAME "mem_map_module"

static int show_process_memory(struct seq_file* m, void* v) {
  struct task_struct* task;

  for_each_process(task) {
    struct mm_struct* mm = task->mm;
    if (mm) {
      seq_printf(m, "PID: %d -> Start: %p, End: %p\n", task->pid, mm->mmap->vm_start, mm->mmap->vm_end);
    }
  }
  return 0;
}

static int proc_open(struct inode* inode, struct file* file) {
  return single_open(file, show_process_memory, NULL);
}

static struct proc_ops proc_fops = {
  .proc_open = proc_open,
  .proc_read = seq_read,
  .proc_lseek = seq_lseek,
  .proc_release = single_release
};

static int __init myinit(void) {
  proc_create(PROC_NAME, 0, NULL, &proc_fops);
  return 0;
}

static void __exit myexit(void) {
  remove_proc_entry(PROC_NAME, NULL);
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ameya Khanzode");
MODULE_DESCRIPTION("Memory Map Kernel Module");

module_init(myinit());
module_exit(myexit());

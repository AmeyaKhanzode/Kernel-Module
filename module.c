#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

#define PROC_NAME "mem_map_module"

static int show_process_memory(struct seq_file* m, void* v) {
  struct task_struct* task;

  for_each_process(task) {
    if (task->mm) {
      seq_printf(m, "PID: %d -> Start: %p, End: %p\n");
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
  .proc_release - single_release
};

static int __init myinit() {
  proc_create(PRO_NAME, 0, NULL, &proc_fops);
  return 0;
}

static void __exit myexit() {
  remove_proc_entry(PROC_NAME, NULL);
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ameya Khanzode");
MODULE_DESCRIPTION("Memory Map Kernel Module");

module_init(myinit());
module_exit(myexit());

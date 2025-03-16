#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/sched/signal.h>
#include <linux/mm.h>
#include <linux/mm_types.h>
#include <linux/sched/mm.h>
#include <linux/vmalloc.h>

#define PROC_NAME "mem_map"
#define TARGET_PID 2876 // Change this to your parent PID

static void show_vma(struct seq_file *m, struct task_struct *task) {
    struct mm_struct *mm;
    struct vm_area_struct *vma;
    struct vma_iterator vmi;

    mm = task->mm;
    if (!mm || !mmget_not_zero(mm))
        return;

    mmap_read_lock(mm);
    vma_iter_init(&vmi, mm, 0);
    vma = vma_find(&vmi, ULONG_MAX);

    if (vma) {
        unsigned long size = (vma->vm_end - vma->vm_start) / 1024;
        seq_printf(m, "|     |---PID: %d\n", task->pid);
        seq_printf(m, "|     |---Start: 0x%lx\n", vma->vm_start);
        seq_printf(m, "|     |---End: 0x%lx\n", vma->vm_end);
        seq_printf(m, "|     |---Memory: %lu KB\n\n", size);
    }

    mmap_read_unlock(mm);
    mmput(mm);
}

static int show_process_memory(struct seq_file *m, void *v) {
    struct task_struct *parent, *child;
    int count = 0;

    for_each_process(parent) {
        if (parent->pid != TARGET_PID)
            continue;

        seq_printf(m, "Parent PID: %d\n", parent->pid);

        list_for_each_entry(child, &parent->children, sibling) {
            seq_printf(m, "|---Child %d:\n", ++count);
            show_vma(m, child);
        }

        break; 
    }

    return 0;
}

static int proc_open(struct inode *inode, struct file *file) {
    return single_open(file, show_process_memory, NULL);
}

static const struct proc_ops proc_fops = {
    .proc_open = proc_open,
    .proc_read = seq_read,
    .proc_lseek = seq_lseek,
    .proc_release = single_release,
};

static int __init myinit(void) {
    if (!proc_create(PROC_NAME, 0, NULL, &proc_fops)) {
        printk(KERN_ERR "Failed to create /proc/%s\n", PROC_NAME);
        return -ENOMEM;
    }
    printk(KERN_INFO "Memory map module loaded\n");
    return 0;
}

static void __exit myexit(void) {
    remove_proc_entry(PROC_NAME, NULL);
    printk(KERN_INFO "Memory map module unloaded\n");
}

module_init(myinit);
module_exit(myexit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ameya Khanzode");
MODULE_DESCRIPTION("Memory Map Kernel Module");


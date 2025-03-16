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

static int show_process_memory(struct seq_file *m, void *v) {
    struct task_struct *task;
    struct mm_struct *mm;
    struct vm_area_struct *vma;
    struct vma_iterator vmi;

    for_each_process(task) {
        mm = task->mm;
        if (!mm) {
            continue;
        }

        if (!mmget_not_zero(mm)) {
            continue;
        }

        mmap_read_lock(mm);
        seq_printf(m, "PID: %d - Process: %s\n", task->pid, task->comm);

        vma_iter_init(&vmi, mm, 0);
        for (vma = vma_find(&vmi, ULONG_MAX); vma; vma = vma_next(&vmi)) {
            unsigned long size = (vma->vm_end - vma->vm_start) / 1024;
            seq_printf(m, "  Start: 0x%lx, End: 0x%lx, Size: %lu KB\n",
                      vma->vm_start,
                      vma->vm_end,
                      size);
        }

        seq_puts(m, "\n");
        mmap_read_unlock(mm);
        mmput(mm);
    }
    return 0;
}

static int proc_open(struct inode *inode, struct file *file) {
    printk(KERN_INFO "Opening proc file\n");
    return single_open(file, show_process_memory, NULL);
}

static const struct proc_ops proc_fops = {
    .proc_open = proc_open,
    .proc_read = seq_read,
    .proc_lseek = seq_lseek,
    .proc_release = single_release,
};

static int __init myinit(void) {
    printk(KERN_INFO "Attempting to create proc entry: %s\n", PROC_NAME);
    if (!proc_create(PROC_NAME, 0, NULL, &proc_fops)) {
        printk(KERN_ERR "Failed to create proc entry %s\n", PROC_NAME);
        return -ENOMEM;
    }
    printk(KERN_INFO "Memory map module loaded successfully\n");
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

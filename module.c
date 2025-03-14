#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/sched/signal.h>
#include <linux/mm.h>
#include <linux/mm_types.h>
#include <linux/sched/mm.h>

#define PROC_NAME "mem_map_module"

static int show_process_memory(struct seq_file* m, void* v) {
    struct task_struct* task;
    struct mm_struct* mm;
    struct vm_area_struct* vma;

    for_each_process(task) {
        mm = task->mm;
        if (mm) {
            // Acquire a reference to the mm_struct
            if (!mmget_not_zero(mm))
                continue;
                
            // Acquire the mmap_lock for reading
            mmap_read_lock(mm);
            
            seq_printf(m, "PID: %d - Process: %s\n", task->pid, task->comm);
            
            // Use VMA iterator for Linux 6.11+
            VMA_ITERATOR(vmi, mm, 0);
            for_each_vma(vmi, vma) {
                seq_printf(m, "  Start: %lx, End: %lx, Size: %lu KB\n", 
                          vma->vm_start, 
                          vma->vm_end,
                          (vma->vm_end - vma->vm_start) / 1024);
            }
            
            // Release the mmap_lock
            mmap_read_unlock(mm);
            
            // Release the reference to mm_struct
            mmput(mm);
        }
    }
    return 0;
}

static int proc_open(struct inode* inode, struct file* file) {
    return single_open(file, show_process_memory, NULL);
}

static const struct proc_ops proc_fops = {
    .proc_open = proc_open,
    .proc_read = seq_read,
    .proc_lseek = seq_lseek,
    .proc_release = single_release
};

static int __init myinit(void) {
    proc_create(PROC_NAME, 0, NULL, &proc_fops);
    printk(KERN_INFO "Memory map module loaded\n");
    return 0;
}

static void __exit myexit(void) {
    remove_proc_entry(PROC_NAME, NULL);
    printk(KERN_INFO "Memory map module unloaded\n");
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ameya Khanzode");
MODULE_DESCRIPTION("Memory Map Kernel Module");

module_init(myinit);
module_exit(myexit);

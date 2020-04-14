#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>

#define MAX_CPU 64

DECLARE_PER_CPU(unsigned long, current_pid);
DECLARE_PER_CPU(unsigned long, lane_flag);

static unsigned long task_signal[MAX_CPU];
module_param_array(task_signal, ulong, NULL, 0644);
MODULE_PARM_DESC(task_signal, "Per-CPU signal shows the runnning task.");

static unsigned long flag_signal[MAX_CPU];
module_param_array(flag_signal, ulong, NULL, 0644);
MODULE_PARM_DESC(flag_signal, "Per-CPU signal shows status of lane.");

int init_module(void) {
	int cpu;
	printk(KERN_ALERT "init kernel sensor\n");
	for_each_possible_cpu(cpu) {
		if (cpu >= 8) break;
		task_signal[cpu] = (unsigned long)__pa(per_cpu_ptr(&current_pid, cpu)); 
		flag_signal[cpu] = (unsigned long)__pa(per_cpu_ptr(&lane_flag, cpu)); 
		printk(KERN_ALERT "cpu=%d, pid=%d\n", cpu, task_signal[cpu]);
	}

	return 0;
}

void cleanup_module(void) {
	printk(KERN_ALERT "stop kernel sensor\n");
}


#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>

#define MAX_CPU 64


static long wxg_current_task[100];
module_param_array(wxg_current_task, long, NULL, 0644);
MODULE_PARM_DESC(wxg_current_task, "Current process info.");


int init_module(void) {
	printk(KERN_ALERT "init kernel sensor\n");
	wxg_current_task[0] = current->pid;
	wxg_current_task[1] = current->state;
	wxg_current_task[2] = current->on_rq;
	return 0;
}

void cleanup_module(void) {
	printk(KERN_ALERT "stop kernel sensor\n");
}


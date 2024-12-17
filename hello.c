// SPDX-License-Identifier: GPL-2.0 OR BSD-3-Clause
#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/list.h>
#include <linux/ktime.h>
#include <linux/slab.h>

MODULE_AUTHOR("Srebniuk Kateryna <ksreiebniukk@gmail.com>");
MODULE_DESCRIPTION("Hello, world in Linux Kernel Training");
MODULE_LICENSE("Dual BSD/GPL");

static unsigned int count = 1; 
module_param(count, uint, 0444); 


struct time_event {
    struct list_head list;
    ktime_t time;
};


static LIST_HEAD(time_list);

static int __init hello_init(void)
{
    unsigned int i = 0;

    if (count == 0 || (count >= 5 && count <= 10)) {
        pr_warn("Warning. Continuing...\n");
    } else if (count > 10) {
        BUG_ON(1);
    }

    for (i = 0; i < count; i++) {
        struct time_event *event;

        if (i == 5) {
            event = NULL;
        } else {
            event = kmalloc(sizeof(*event), GFP_KERNEL);
        }

        BUG_ON(!event);

        event->time = ktime_get();
        list_add_tail(&event->list, &time_list);
        pr_info("Hello, world!\n");
    }

    return 0;
}

static void __exit hello_exit(void)
{
    struct time_event *event;
    struct list_head *pos, *q;

    list_for_each_safe(pos, q, &time_list) {
        event = list_entry(pos, struct time_event, list);
        pr_info("Event time: %lld ns\n", ktime_to_ns(event->time));
        list_del(pos);
        kfree(event);
    }
}

module_init(hello_init);
module_exit(hello_exit);

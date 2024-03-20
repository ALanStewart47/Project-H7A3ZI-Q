/*
 * Copyright (c) 2006-2024, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-03-20     ALanStewart    first version
 */

#include <rtthread.h>


#define DBG_TAG "main"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

static rt_thread_t tid1 =   RT_NULL;

static char thread2_stack[1024];

static struct rt_thread thread2;


static void thread1_entry(void *parameter)
{
    rt_uint32_t count   =   0;

    while(1){
        rt_kprintf("thread1 count： %d\n",count++);
        rt_thread_mdelay(500);
    }
}


static void thread2_entry(void *parameter)
{
    rt_uint32_t count   =   0;

    for(count;count<10;count++)
    {
        rt_kprintf("thread2 count : %d\r\n",count);
        rt_thread_mdelay(500);
    }
    rt_kprintf("thread exit\r\n");
}


int thread_sample(void)
{
    tid1 = rt_thread_create("thread1", thread1_entry, RT_NULL, 512, 25, 5);
}



int main(void)
{
    int count = 1;
    while (count++)
    {
        //LOG_D("Hello RT-Thread!");
        rt_thread_mdelay(1000);
    }

    return RT_EOK;
}



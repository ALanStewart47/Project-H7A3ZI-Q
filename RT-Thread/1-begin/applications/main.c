/*
 * Copyright (c) 2006-2024, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-03-19     RT-Thread    first version
 */

#include <rtthread.h>
//#include <main.h>


#define DBG_TAG "main"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

static struct rt_thread led_thread;
static char led_thread_stack[256];

static rt_thread_t  uart3_thread =   RT_NULL;


static void led_thread_entry(void *par){
    while(1){
        rt_pin_write(16,0);
        rt_thread_delay(50);
        rt_pin_write(16,1);
        rt_thread_delay(50);
    }
}


static void uart3_thread_entry(void *par){
    while(1){
        LOG_D("thread 2\r\n");
        rt_thread_delay(500);
        LOG_D("thread 2-1\r\n");
        rt_thread_delay(500);
    }
}




void uart_blink(void){
    uart3_thread = rt_thread_create("uart3_thread",
                                        uart3_thread_entry ,
                                        RT_NULL,
                                        256,
                                        RT_THREAD_PRIORITY_MAX-1,
                                        30);

    if (uart3_thread != RT_NULL) {
        rt_thread_startup(uart3_thread);
    }

}


int main(void)
{
    int count = 1;
    rt_err_t rst2;

    rt_pin_mode(16,0);

    rst2 = rt_thread_init(&led_thread,
                            "ld1_blink",
                            led_thread_entry,
                            RT_NULL,
                            &led_thread_stack[0],
                            sizeof(led_thread_stack),
                            RT_THREAD_PRIORITY_MAX-1,
                            50);

        if (rst2 == RT_EOK) {
            rt_thread_startup(&led_thread);
        }




    while (count++)
    {
        //LOG_D("Hello RT-Thread!");
        rt_thread_mdelay(1000);
    }

    return RT_EOK;
}

MSH_CMD_EXPORT(uart_blink, uart3 sample);

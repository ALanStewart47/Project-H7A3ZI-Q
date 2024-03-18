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
#include <main.h>

#define DBG_TAG "main"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

static struct rt_thread led_thread;
static char led_thread_stack[256];

static void led_thread_entry(void *par){
    while(1){
        HAL_GPIO_WritePin(LD1_GPIO_Port,LD1_Pin, SET);
        rt_thread_delay(500);
        HAL_GPIO_WritePin(LD1_GPIO_Port,LD1_Pin, RESET);
        rt_thread_delay(500);
    }
}



int main(void)
{
    int count = 1;
    rt_err_t rst2;

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
        LOG_D("Hello RT-Thread!");
        rt_thread_mdelay(1000);
    }

    return RT_EOK;
}

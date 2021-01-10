/*
 * Copyright (c) 2006-2018
 *
 * SPDX-License-Identifier:
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020           wangrui
 */

#include <flow.h>

/* 1，初始化一个struct flow变量 */
static struct flow fl_led;


static char led_flash(struct flow *fl)
{
    FL_HEAD(fl);
    while(1)
    {
        FL_LOCK_DELAY(fl, FL_CLOCK_SEC * 1); /* 延时一秒 */
        led_open();
        FL_LOCK_DELAY(fl, FL_CLOCK_SEC * 1); /* 延时一秒 */
        led_close();
    }
    FL_TAIL(fl);
}

int main(void)
{
    FL_INIT(&fl_led);
    while(1)
    {
        led_flash(&fl_led);
        //other_process();
    }
    return 0;
}
/*
 * Copyright (c) 2006-2021
 *
 * SPDX-License-Identifier:
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020           wangrui
 */

#include <flow.h>

unsigned long flow_tick;

void fl_timer_set(struct flow_timer *t, unsigned long interval)
{
    t->interval = interval;
    t->start = flow_tick;
}

void fl_timer_reset(struct flow_timer *t)
{
    t->start += t->interval;
}

void fl_timer_restart(struct flow_timer *t)
{
    t->start = flow_tick;
}

unsigned char fl_timer_timeout(struct flow_timer *t)
{
    return ((flow_tick - t->start) >= t->interval) ? 1U : 0U;
}

unsigned long fl_hour_much_time(struct flow_timer *t)
{
    unsigned long time_len = t->start + t->interval;

    if (time_len >= flow_tick) {
        return (time_len - flow_tick);

    } else {
        return 0U;
    }
}

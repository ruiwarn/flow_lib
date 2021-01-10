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

char fl_timer_timeout(struct flow_timer *t)
{
    return ((flow_tick - t->start) >= t->interval) ? 1 : 0;
}

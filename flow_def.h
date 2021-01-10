/*
 * Copyright (c) 2006-2018
 *
 * SPDX-License-Identifier:
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020           wangrui
 */

#ifndef __FLOW_DEF_
#define __FLOW_DEF_

#define FLOW_WAIT    (0)
#define FLOW_LOCK    (1)
#define FLOW_FINISH  (2)
#define FLOW_END     (3)

struct flow {
    unsigned short line;
    unsigned long time;
};

struct flow_timer {
    unsigned long start;
    unsigned long interval;
};

struct flow_sem {
    unsigned short count;
};

extern unsigned long flow_tick;






#endif /* __FLOW_DEF_ */



/*
 * Copyright (c) 2006-2018
 *
 * SPDX-License-Identifier:
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020           wangrui
 */

#ifndef __FLOW_SEM_H__
#define __FLOW_SEM_H__

#include <flow_def.h>
#include <flow_core.h>


#define FLOW_SEM_INIT(s, c) (s)->count = c

#define FLOW_LOCK_WAIT_SEM(f, s) \
do{\
FLOW_LOCK_WAIT(f, (s)->count > 0);\
--(s)->count;\
}while(0)

#define FLOW_LOCK_WAIT_SEM_OR_TIMEOUT(f, s, t) \
do{\
(f)->time=flow_tick;\
(s)->time=(t);\
FLOW_LOCK_WAIT(f, (((s)->count>0)||((flow_tick-(f)->time)>=((s)->time))));\
if(((s)->count>0)&&((flow_tick-(f)->time)<((s)->time))) --(s)->count;\
}while(0)

#define FLOW_SEM_RELEASE(s) ++(s)->count


#endif /* __FLOW_SEM_H__ */


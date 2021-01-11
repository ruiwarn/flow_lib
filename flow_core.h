/*
 * Copyright (c) 2006-2018
 *
 * SPDX-License-Identifier:
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020           wangrui
 */

#ifndef __FLOW_CORE_
#define __FLOW_CORE_

#include <flow_def.h>


#define FLOW_INIT(f) ((f)->line=0)

#define FLOW_HEAD(f) {\
char lock_once_flag=0;\
if (lock_once_flag) {;}\
switch((f)->line) { case 0:

#define FLOW_TAIL(f) };\
lock_once_flag=(f)->line=0;\
return FLOW_END; };

#define FLOW_LOCK_WAIT(f, judge) \
do{\
(f)->line=__LINE__;case __LINE__:;\
if(!(judge)) return FLOW_WAIT;\
}while(0)

#define FLOW_LOCK_WHILE(f, judge) \
do{\
(f)->line=__LINE__;case __LINE__:;\
if(judge) return FLOW_WAIT;\
}while(0)

#define FLOW_EXIT(f) \
do{\
(f)->line=0;\
return FLOW_FINISH;\
}while(0)

#define FLOW_LOCK_ONCE(f) \
do{\
lock_once_flag=1;\
(f)->line=__LINE__;case __LINE__:;\
if(lock_once_flag) return FLOW_LOCK;\
}while(0)

#define FLOW_WAIT_PROCESS_END(f, process) FLOW_LOCK_WHILE(f, (process)<FLOW_FINISH)

#define FLOW_WAIT_CHILD_PROCESS_END(f, cf, process) \
do{\
FLOW_INIT((cf));             \
FLOW_WAIT_PROCESS_END((f), (process));     \
}while(0)

#define FLOW_LOCK_DELAY(f,t) \
do{\
(f)->time=flow_tick;\
FLOW_LOCK_WAIT((f),((flow_tick-(f)->time)>=(t)));\
}while(0)

#define FLOW_LOCK_DELAY_OR_WAIT(f, judge, t) \
do{\
(f)->time=flow_tick;\
FLOW_LOCK_WAIT((f),((judge)||((flow_tick-(f)->time)>=(t))));\
}while(0)


#endif /* __FLOW_CORE_ */



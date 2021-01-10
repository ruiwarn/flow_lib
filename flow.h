/*
 * Copyright (c) 2006-2018
 *
 * SPDX-License-Identifier:
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020           wangrui
 */

#ifndef __FLOW_
#define __FLOW_

#include <flow_def.h>
#include <flow_core.h>
#include <flow_sem.h>

#define FL_HARD_TICK        (500)                 /* 系统硬件中断一次所需要的时间，单位ms */
#define FL_CLOCK_SEC        (1000/FL_HARD_TICK)   /* 一秒钟需要的tick，可以除也可以自行添加其它宏 */

/**
 * 初始化一个flow进程
 */
#define FL_INIT(fl)                                     FLOW_INIT(fl)

/**
 * 如果函数是一个flow进程，那么必须放在函数前面
 */
#define FL_HEAD(fl)                                     FLOW_HEAD(fl)

/**
 * 如果函数是一个flow进程，那么必须放在函数最后面
 */
#define FL_TAIL(fl)                                     FLOW_TAIL(fl)

/**
 * 给进程加锁，直到judge为真，加锁期间一直放开cpu给其他进程使用
 */
#define FL_LOCK_WAIT(fl, judge)                         FLOW_LOCK_WAIT(fl, judge)

/**
 * 如果judge为真，就一直给进程加锁，加锁期间一直放开cpu给其他进程使用
 */
#define FL_LOCK_WHILE(fl, judge)                        FLOW_LOCK_WHILE(fl, judge)

/**
 * 如果judge为真，就一直给进程加锁，加锁期间一直放开cpu给其他进程使用
 */
#define FL_EXIT(fl)                                     FLOW_EXIT(fl)

/**
 * 无条件锁住进程一次
 */
#define FL_LOCK_ONCE(fl)                                FLOW_LOCK_ONCE(fl)

/**
 * 等待一个flow进程结束
 */
#define FL_WAIT_PROCESS_END(fl, process)                FLOW_WAIT_PROCESS_END(fl, process)

/**
 * 等待一个flow子进程结束
 */
#define FL_WAIT_CHILD(fl, cfl, process)                 FLOW_WAIT_CHILD_PROCESS_END(fl, cfl, process)

/**
 * 给进程加锁time的时长，加锁期间一直放开cpu给其他进程使用，time如果用FL_CLOCK_SEC来乘，那么time的单位就是s
 */
#define FL_LOCK_DELAY(fl,time)                          FLOW_LOCK_DELAY(fl,time)

/**
 * 给进程加锁time的时长，期间如果judge为真，就解锁进程
 */
#define FL_LOCK_DELAY_OR_WAIT(fl,judge,time)            FLOW_LOCK_DELAY_OR_WAIT(fl,judge,time)

/**
 * 初始化一个信号量
 */
#define FL_SEM_INIT(sem, count)                         FLOW_SEM_INIT(sem, count)

/**
 * 给进程加锁，直到有信号释放
 */
#define FL_LOCK_WAIT_SEM(f, sem)                        FLOW_LOCK_WAIT_SEM(f, sem)

/**
 * 给进程加锁，直到有信号或者超时
 */
#define FL_LOCK_WAIT_SEM_OR_TIMEOUT(fl, sem, time)      FLOW_LOCK_WAIT_SEM_OR_TIMEOUT(fl, sem, time)

/**
 * 释放一个信号量
 */
#define FL_SEM_RELEASE(sem)                             FLOW_SEM_RELEASE(sem)

/**
 * 初始化一个软件定时器
 */
void fl_timer_set(struct flow_timer *t, unsigned long interval);

/**
 * 复位一个软件定时器
 */
void fl_timer_reset(struct flow_timer *t);

/**
 * 重启一个软件定时器
 */
void fl_timer_restart(struct flow_timer *t);

/**
 * 检测一个软件定时器是否超时，0为不超时，1为超时
 */
char fl_timer_timeout(struct flow_timer *t);



#endif /* __FLOW_ */
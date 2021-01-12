# flow_lib

#### 介绍
适用于嵌入式单片机的裸机程序微库，只占用你的rom 6个字节，是的，6个字节。颠覆式的设计思维，让你写代码的时候像flow(流水)一样丝滑，让你永远不用在为delay时cpu空转而烦恼，附加的超轻便的软件定时器让你轻松实现各种定时需求，另还有信号量的配方，让你任务间的同步像诗一样写意，并且能让你裸机程序效率提升百倍以上。

#### 移植说明
移植特别简单，flow_def.h有一个全局变量：
```
extern unsigned long flow_tick;
```

把这个变量放在你的某个硬件中断里去，这个硬件中断一定要是一直运行的，推荐RTC半秒中断，或者systick中断都可以。

然后在flow.h里的第一行有个宏
```
#define FL_HARD_TICK        (500)                 /* 系统硬件中断一次所需要的时间，单位ms */
```

把这里的值改成你的硬件中断一次所需的时间，单位是毫秒，比如你的flow_tick放在了一个500ms中断一次的rtc里，那么这里的宏FL_HARD_TICK的值就是500，具体中断设为多少取决于你的系统最短一次的延时的时间。

假如我的最短延时需求是100ms，那么我就得给个100ms中断一次的硬件中断源，宏FL_HARD_TICK的值就是100，我就可以这样使用：
```
FL_LOCK_DELAY(fl, FL_CLOCK_SEC /10);
```
来延时100ms。

#### 使用说明
核心文件时flow.h，看这里的注释基本就会使用大部分功能。

```
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
 * flow头，必须放在函数内的最前面
 */
#define FL_HEAD(fl)                                     FLOW_HEAD(fl)

/**
 * flow尾，必须放在函数内的最后面
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
 * 退出该进程
 */
#define FL_EXIT(fl)                                     FLOW_EXIT(fl)

/**
 * 无条件锁住进程一次，下次进来再接着往下运行
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
 * 给进程加锁，时长为time，加锁期间一直放开cpu给其他进程使用，time如果用FL_CLOCK_SEC来乘，那么time的单位就是s
 * 此处time必须是常数
 */
#define FL_LOCK_DELAY(fl,time)                          FLOW_LOCK_DELAY(fl,time)

/**
 * 给进程加锁，时长为time，延时期间如果judge为真，就直接解锁进程
 * 此处time必须是常数
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
 * 给进程加锁，直到有信号或者超时，此处time可以为变量，其他的接口处time必须是常数
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

/**
 * 检测一个软件定时器还剩多少时间超时，单位为硬件tick，比如硬件tick 500ms中断一次，那么
 * 返回的时间单位就是500ms
 */
unsigned long fl_hour_much_time(struct flow_timer *t);

#endif /* __FLOW_ */
```


简单举个例子，先从需求说起，假如说你现在需要一个函数，这个函数的功能是每隔1s让你的led亮一次，正常设计的要么起个软件定时器或者硬件定时器，甚至状态机可以实现需求，但是都太low了，让我们看一下如何用flow库来实现这个函数。

该函数格式如下：

```
char led_flash(struct flow *fl)
{}
```
其中char、struct flow *fl是必备的。

再来看看函数里面的内容格式：

```
char led_flash(struct flow *fl)
{
    FL_HEAD(fl);
    FL_TAIL(fl);
}
```
函数里面的FL_HEAD和FL_TAIL是使用flow库的所必须的宏，FL_HEAD(fl)放到函数的最前面，如果你的函数内部有变量定义的话放在变量定义的后面。而FL_TAIL(fl)是放在函数最后面一行的。

基本格式有了，再来看下如何实现延时一秒呢？其实只用一个语句就OK。

```
char led_flash(struct flow *fl)
{
    FL_HEAD(fl);
    FL_LOCK_DELAY(fl, FL_CLOCK_SEC * 1);
    led_open();
    FL_LOCK_DELAY(fl, FL_CLOCK_SEC * 1);
    led_close();
    FL_TAIL(fl);
}
```

是的，你没看错，仅仅只需要FL_LOCK_DELAY(fl, FL_CLOCK_SEC * 1)这一个语句就OK，当执行到这个语句的时候该函数就会让出CPU权限，当延时时间到了之后，就会回来接着执行FL_LOCK_DELAY(fl, FL_CLOCK_SEC * 1)下面的语句。一直到FL_TAIL(fl)，该函数就会结束任务，再也不会执行了，那么如果我们想让它一直循环执行呢？看下面：

```
char led_flash(struct flow *fl)
{
    FL_HEAD(fl);
    while(1)
    {
        FL_LOCK_DELAY(fl, FL_CLOCK_SEC * 1);
        led_open();
        FL_LOCK_DELAY(fl, FL_CLOCK_SEC * 1);
        led_close();
    }
    FL_TAIL(fl);
}
```
看起来像不像个进程？其实也有点操作系统的样子了。。。

光有这个函数也不行，还得进行一些额外的操作

比如：

```
static struct flow fl_led; /* 1，定义一个struct flow变量给这个函数使用 */

static char led_flash(struct flow *fl)
{
    FL_HEAD(fl);
    led_init();  /* 这里还能解决你的初始化问题，这里的函数只会在开机时运行一次，以后永远不会运行。注意：如果放在FL_HEAD(fl)前面，那么就是每次轮到这个进程运行时就会 
                    运行一次，总之很灵活 */
    while(1)
    {
        FL_LOCK_DELAY(fl, FL_CLOCK_SEC * 1);
        led_open();
        FL_LOCK_DELAY(fl, FL_CLOCK_SEC * 1);
        led_close();
    }
    FL_TAIL(fl);
}

int main(void)
{
    FL_INIT(&fl_led);  /* 2，初始化struct flow变量 */
    while(1)
    {
        led_flash(&fl_led); /* 3，把led_flash进程放在main函数的while循环里 */
        ...
    }
    return 0;
}
```
经过以上3步，就可以实现进程之间的切换啦。然后想根据某个条件来锁住线程释放CPU的话，可以把里面的
```
FL_LOCK_DELAY(fl, FL_CLOCK_SEC * 1);
```
换成
```
FL_LOCK_WAIT(fl, judge);
```
当里面的judge为假时线程就一直锁住在这一行语句，当judge为真时就可以往下执行啦。同理可以完成很多其他的神奇功能，让你的cpu再也不空转啦，具体请看flow.h文件。。。。

这个版本暂时先写这么多，先看看example.c。
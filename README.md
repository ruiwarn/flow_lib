# flow_lib

#### 介绍
适用于嵌入式单片机的裸机程序微库，只占用你的rom 6个字节，是的，6个字节。颠覆式的设计思维，让你写代码的时候像flow(流水)一样丝滑，让你永远不用在为delay时cpu空转而烦恼，附加的超轻便的软件定时器让你轻松实现各种定时需求，令还有信号量的配方，让你任务间的同步像诗一样写意，并且能让你逻辑程序效率提升百倍以上。

#### 移植说明
移植特别简单，flow_def.h有一个全局变量extern unsigned long flow_tick;，把这个变量放在你的某个硬件中断里去，这个硬件中断一定要是一直运行的，推荐RTC半秒中断，或者ststick中断都可以。
然后在flow.h里的第一行有个宏FL_HARD_TICK，这个值改成你的硬件中断一次所需的时间，单位是毫秒，必须你的flow_tick放在了一个500ms中断一次的rtc里，那么这里的宏FL_HARD_TICK的值就是500，具体中断设为多少取决于你的系统最短一次的延时的时间，比如我的最短延时需求是100ms，那么我就得给个100ms中断一次的硬件中断源，宏FL_HARD_TICK的值就是100，我就可以这样使用：FL_LOCK_DELAY(fl, FL_CLOCK_SEC /10);来延时100ms。

#### 使用说明
先从需求说起，假如说你现在需要一个函数，这个函数的功能是每隔1s让你的led亮一次，正常设计的要么起个软件定时器或者硬件定时器，甚至状态机可以实现需求，但是都太low了，让我们看一下如何用flow库来实现这个函数。
该函数格式如下：
char led_flash(struct flow *fl)
{}
其中char、struct flow *fl是必备的
再来看看函数里面的内容格式：
char led_flash(struct flow *fl)
{
    FL_HEAD(fl);
    FL_TAIL(fl);
}
函数里面的FL_HEAD和FL_TAIL是使用flow库的所必须的宏，FL_HEAD(fl)放到函数的最前面，如果你的函数内部有变量定义的话放在变量定义的后面。而FL_TAIL(fl)是放在函数最后面一行的。
基本格式有了，再来看下如何实现延时一秒呢？其实只用一个语句就OK。
char led_flash(struct flow *fl)
{
    FL_HEAD(fl);
    FL_LOCK_DELAY(fl, FL_CLOCK_SEC * 1);
    led_open();
    FL_LOCK_DELAY(fl, FL_CLOCK_SEC * 1);
    led_close();
    FL_TAIL(fl);
}
是的，你没看错，仅仅只需要FL_LOCK_DELAY(fl, FL_CLOCK_SEC * 1)这一个语句就OK，当执行到这个语句的时候该函数就会让出CPU权限，当延时时间到了之后，就会回来接着执行FL_LOCK_DELAY(fl, FL_CLOCK_SEC * 1)下面的语句。一直到FL_TAIL(fl)，该函数就会结束任务，再也不会执行了，那么如果我们想让它一直循环执行呢？看下面：
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
看起来像不像个进程？其实也有点操作系统的样子了。。。
光有这个函数也不行，还得进行一些额外的操作，以下一一说明：
1，初始化一个struct flow变量给这个函数使用
2，把它放在main函数的while循环里
比如：
static struct flow fl_led;

static char led_flash(struct flow *fl)
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

int main(void)
{
    FL_INIT(&fl_led);
    while(1)
    {
        led_flash(&fl_led);
        ...
    }
    return 0;
}
这个版本暂时先写这么多，看example.c就好。
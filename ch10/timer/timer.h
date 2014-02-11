#ifndef TIMER_H_
#define TIMER_H_

/*
 * 创建一个timer，成功返回tid(tid>=0)
 * 失败返回-1,并设置errno
 * ERANGE:创建的timer数超过了限制
 */
int timer_create(int msec, void (*handler)(int));

/*
 * 启动一个已经指定的计数器
 */
int timer_start(int tid);

/*
 * 停止指定的计数器
 */
int timer_stop(int tid);

/*
 * 关闭一个指定的计数器
 */
int timer_close(int tid);

#endif /*TIMER_H_*/

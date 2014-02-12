#ifndef TIMER2_H__
#define TIMER2_H__
/******************type declare***********************/
struct timer;
/* macro define */
#define MAX_TIMERS 3	
#define FALSE	0
#define TRUE	1

/******************function prototype*****************/
/*
 * 初始化计时器
 */
void timer_init();
/* 
 * 注册计时器
 */
struct timer *timer_declare(long usec, void (*tmr_handler)(int));

/*
 * 取消计时器
 */
void timer_undeclare(struct timer *t);

#endif


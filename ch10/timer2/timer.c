/*
 *具体的设计细节，请看 
 *http://www.cnblogs.com/CoreyGao/archive/2013/05/01/3053417.html
 */
#include "timer.h"
#include <sys/types.h>
#include <unistd.h>
#include <sys/time.h>
#include <signal.h>
#include <limits.h>
#include <stdio.h>

typedef long TIME;

#define US_PER_SEC 1000000	
#define VERY_LONG_TIME	LONG_MAX
#define time_now	get_system_time()

static struct timer 
{
	int inuse;		/* 0 for not in use, 1 for in use */
	TIME interval;  /* interval usecs before expired */
	void (*tmr_handler)(); /* handler the signal SIGALRM */ 
	char *event;		/* TRUE for event is occure */
} timers[MAX_TIMERS];
static struct sigaction  newact, oldact;
static struct timer *timer_next; 

volatile TIME time_timer_reset;  /*time when physical timer is reset*/
volatile sig_atomic_t can_interrupts = 1;	/*check interruptes can raise */
volatile sig_atomic_t has_install_interrupt_handler = 0;	/*check has install interrupt handler*/
static sigset_t oldmask;

static void enable_interrupts();
static void disable_interrupts();
static TIME get_system_time();
static void timer_update(TIME interval);
static void start_physical_timer(TIME interval);
static void stop_physical_timer();
static void timer_interrupt_handler(int signo);

static void disable_interrupts()
{
	while(can_interrupts != 1)
		;
	sigset_t mask;
	sigemptyset(&mask);
	sigaddset(&mask, SIGALRM);
	sigprocmask(SIG_BLOCK, &mask, &oldmask);	
	can_interrupts = 0;
}

static void enable_interrupts()
{
	while(can_interrupts != 0)
		;	
	sigprocmask(SIG_SETMASK, &oldmask, NULL);
	can_interrupts = 1;
}

static TIME get_system_time()
{
	TIME sys_time = 0;
	int status = -1;
	struct timeval tv;
	struct timezone tz;
	status = gettimeofday(&tv, &tz);
	if (status == -1)
		return status;
	sys_time = tv.tv_sec * US_PER_SEC + tv.tv_usec;
	return sys_time;
}

static void timer_update(TIME interval)
{
	static struct timer timer_last = {
		FALSE,
		VERY_LONG_TIME,
		NULL,
		NULL};
	struct timer *t;
	
	timer_next = &timer_last;
	for( t = timers; t < &timers[MAX_TIMERS]; t++)
	{
		if (t->inuse)
		{
#ifdef DEBUG
			printf("interval = %08ld, t->interval = %08ld, timer_next->interval = %10ld\n", interval, t->interval, timer_next->interval);
#endif
			if (interval < t->interval)
			{
				t->interval -= interval;
				if (t->interval < timer_next->interval)
					timer_next = t;
			}
			else  /*expired*/
			{
				/*tell scheduler*/
				t->event = (char *)TRUE;
				t->inuse = FALSE;	/*remove timer*/	
			}
		}	
	}		
	if (!timer_next->inuse) timer_next = 0;
}

static void start_physical_timer(TIME interval)
{
	struct itimerval itv;
	if (!has_install_interrupt_handler)
	{
		newact.sa_flags = 0;
		sigemptyset(&newact.sa_mask);	
		newact.sa_handler = timer_interrupt_handler;
		sigaction(SIGALRM, &newact, &oldact);
		has_install_interrupt_handler = 1;
	}	
	itv.it_interval.tv_sec = 0;
	itv.it_interval.tv_usec = 0;
	itv.it_value.tv_sec = interval / US_PER_SEC;
	itv.it_value.tv_usec = interval % US_PER_SEC;
	setitimer(ITIMER_REAL, &itv, NULL);	
}

static void stop_physical_timer()
{
	sigaction(SIGALRM, &oldact, NULL);
}
static void timer_scheduler()
{
	struct timer *t;
	for (t = timers; t < &timers[MAX_TIMERS]; t++)
	{
		if (t->event == (char *)TRUE)
		{
			if (t->tmr_handler != NULL)
				t->tmr_handler(0);
			t->event = (char *)FALSE;
		}
	}
}
static void timer_interrupt_handler(int signo)
{
#ifdef DEBUG
	printf("caughted interrupted.\n");
#endif
	timer_update( time_now - time_timer_reset);
	timer_scheduler();
	if (timer_next)
	{
#ifdef DEBUG
		printf("timer_next\n");
#endif
		fflush(NULL);
		time_timer_reset = time_now;
		start_physical_timer(timer_next->interval);
	}	  
	else
	{
		stop_physical_timer();
	}
	
} 

void timer_init()
{
	struct timer *t;
	for (t = timers; t < &timers[MAX_TIMERS]; t++)
	{
		t->inuse = FALSE;
		t->event = FALSE;
	}
}

struct timer* timer_declare(TIME interval, void (*handler)(int))
{
	struct timer *t;
	
	disable_interrupts();

	if (interval <= 0)
	{
		enable_interrupts();
		return (struct timer *)0;
	}
	for (t = timers; t < &timers[MAX_TIMERS]; t++)
	{
		if (!t->inuse)
		{
			t->event = FALSE;
			t->tmr_handler = handler;
			t->interval = interval;
			break;
		}
	}		
	if (t == &timers[MAX_TIMERS])
	{
		enable_interrupts();
		return (struct timer *)0;
	}
	if (!timer_next)
	{
		time_timer_reset = time_now;
		timer_next = t;
		start_physical_timer(timer_next->interval);
	}
	else if (time_now + interval < time_timer_reset + timer_next->interval)
	{
		timer_update(time_now - time_timer_reset);
		time_timer_reset = time_now;
		timer_next = t;
		start_physical_timer(timer_next->interval);
	}
	else
	{
		/*has not expired, do nothing*/
	}			
	t->inuse = TRUE;
 	enable_interrupts();

	return t;
}

void timer_undeclare(struct timer *t)
{
	disable_interrupts();
	if (t == (struct timer *)0)
	{
		enable_interrupts();
		return ;
	}
	
	if (!t->inuse)
	{
		enable_interrupts();
		return;
	}
	
	t->inuse = FALSE;
	/* check if we are waiting on it */
	if (t == timer_next)
	{
		timer_update(time_now - time_timer_reset);
		if (timer_next)
		{
			time_timer_reset = time_now;
			start_physical_timer(timer_next->interval);
		}
	}				
	enable_interrupts();
}


#include "timer.h"

#include <sys/types.h>
#include <sys/time.h>
#include <signal.h>
#include <errno.h>
#include <limits.h>

#ifdef BOOL
#undef
#define BOOL char 
#define TRUE 1
#define FALSE 0 
#define MAXTMRS	1024

/* FOR hasinit global variable */
#define HAS_NOT_INIT	0
#define HAS_INIT	1

/* TIMER STATUS */
#define NO_IN_USE	0
#define STOP	1
#define USING	2

struct tmr_instance
{
	void (*tmr_handler)(int);
	int tmr_interval;
	BOOL tmr_status;	
};

typedef struct tmr_instance tmr_instance_t;


extern int errno;

static tmr_instance_t g_tmr_instance[MAXTMRS];
static sig_atomic_t g_hasinit = HAS_NOT_INIT;
static int g_min_interval = MAX_UINT;

static void interrupt_handler(int signo);
static int tmr_init(void);

static int tmr_init(void)
{
	struct sigaction act;
	struct itimerval itval;
	int i = 0;
	for(i = 0; i < MAXTMRS; i++)
	{
		g_tmr_instance[i].tmr_status = NOT_IN_USE ;
	}		
	
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	#ifdef SA_INTERRUPT
	act.sa_flags = SA_INTERRUPT;
	#endif	
	act.sa_handler = interrupt_handler;
	if (sigaction(SIGALRM, &act, NULL) < 0)
		return -1;	
	itval.it_interval.tv_sec = 0;
	itval.it_interval.tv_usec = 0;
	itval.it_value.tv_sec = g_min_interval;
	itval.it_value.tv_usec = 0;
	setitimer(ITIMER_REAL, &itval, NULL);	
	g_hasinit = HAS_INIT;

	return 0;	
}

int timer_create(int usec, void (*handler)(int))
{
	int i = 0;

	if (handler == NULL || usec == 0)
	{
		errno = EINVAL;
		return -1;
	}
	
	while(i < MAXTMRS && g_tmr_instance[i].tmr_status == NO_IN_USE)
	{
		g_tmr_instance[i].tmr_status = STOP;
		g_tmr_instance[i].tmr_handler = handler;
		g_tmr_instance[i].tmr_interval = usec;
		i ++;
	}
	if (i == MAXTMRS)
	{
		errno = ERANGE;
		return -1;
	}	
	 
	return i;	
}	

int timer_start(int tid)
{
			
}	

int timer_stop(int tid)
{
	g_tmr_instance[tid].tmr_status = STOP;	
}

int timer_close(int tid)
{
	g_tmr_instance[tid].tmr_status = NOT_IN_USE;
}

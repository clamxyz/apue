#include "timer.h"

#include <sys/types.h>
#include <sys/time.h>
#include <signal.h>
#include <errno.h>
#include <limits.h>

#ifdef BOOL
#undef BOOL
#endif
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
static long g_min_interval = UINT_MAX;

static void interrupt_handler(int signo);
static int tmr_init(void);
static void start_physic_timer(long usec);
static int valid_tid(int tid);
static long get_secs_from_itimerval(struct itimerval itval);
static void set_secs_to_itimerval(struct itimerval *pitval, int usec)

static void interrupt_handler(int signo)
{
	
}

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

static void start_physic_timer(long usec)
{
	struct itimerval itval;
	itval.it_interval.tv_sec = 0;
	itval.it_interval.tv_usec = 0;
	itval.it_value.tv_sec = usec / 1000;
	itval.it_value.tv_usec = usec % 1000;
	setitimer(ITIMER_REAL, &itval, NULL);	 
}

static int valid_tid(int tid)
{
	if (tid < 0 || tid >= MAXTMRS)
	{
		errno = ERANGE;
		return -1;
	}

	return 0;
}

static long get_secs_from_itimerval(struct itimerval itval)
{
	return itval.it_value.tv_sec * 1000 + itval.it_val.tv_usec;
}	

static void set_secs_to_itimerval(struct itimerval *pitval, int usec)
{
	pitval->it_value.tv_sec = usec / 1000;
	pitval->it_value.tv_usec = usec % 1000;
	pitval->it_interval.tv_sec = 0;
	pitval->it_interval.tv_usec = 0; 
}

int timer_create(int usec, void (*handler)(int))
{
	int i = 0;

	if (handler == NULL || usec == 0)
	{
		errno = EINVAL;
		return -1;
	}
	if (!g_hasinit && tmr_init() != 0)
	{
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
	sigset_t newmask, oldmask;
	long usec;
	struct itimerval cur_itval;
	struct itimerval next_itval;
	int i = 0;

	if (valid_tid(tid) == -1)
		return -1;
	if (g_tmr_instance[tid].tmr_status != STOP)
	{
		errno = EINVAL;
		return -1;
	}	
	sigemptyset(&newmask);
	sigaddset(&newmask, SIGALRM);
	sigprocmask(SIG_BLOCK, &newmask, &oldmask);
	getitimerval(ITIMER_REAL, &cur_itval);
	usec = get_secs_from_itimerval(cur_itval);
	if (usec == 0)
	{
		set_secs_to_itimerval(&cur_itval, g_tmr_instance[tid].tmr_interval);
		setitimerval(ITIMER_REAL, &cur_itval, NULL);
	}
	else
	{
		
	}
	sigprocmask(SIG_SETMASK, &oldmask, NULL);
		
}	

int timer_stop(int tid)
{
	int i = 0;
	if (vaild_tid(tid) == -1)
		return -1;
	if (g_tmr_instance[tid].tmr_status == NOT_IN_USE)
	{
		errno = EINVAL;
		return -1;
	}
	
	g_tmr_instance[tid].tmr_status = STOP;
	
	while(i < MAXTMRS && g_tmr_instance[i++].tmr_status != USING )
			;
	if (i == MAXTMRS)
	{
		g_min_interval = UINT_MAX;
	}
}

int timer_close(int tid)
{
	int i = 0;
	if (valid_tid(tid) == -1)
		return -1;
	if (g_tmr_instance[tid].tmr_status == NOT_IN_USE)
	{
		errno = EINVAL;
		return -1;
	}
	g_tmr_instance[tid].tmr_status = NOT_IN_USE;
	while (i < MAXTMRS && g_tmr_instance[i++].tmr_status != USING)
		;
	if (i == MAXTMRS)
	{
		g_min_interval = UINT_MAX;
	}	
}


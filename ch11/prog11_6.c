#include <stdlib.h>
#include <pthread.h>

#define NHASH	29L
#define HASH(val) ((unsigned long)val % NHASH)

struct foo{
	pthread_mutex_t f_lock;
	struct foo *f_next;
	int f_count;
	/* other staff */
};

struct foo *hashfoo[NHASH];
pthread_mutex_t hashlock = PTHREAD_MUTEX_INITIALIZER; 

struct foo *foo_alloc()
{
	struct foo *pf;
	int idx;

	if ((pf = (struct foo *)malloc(sizeof(struct foo))) == NULL)
		return NULL;

	pthread_mutex_init(&pf->f_lock, NULL);
	idx = HASH(pf);
	pthread_mutex_lock(&hashlock);
	pf->f_next = hashfoo[idx];
	hashfoo[idx] = pf->f_next;
	pthread_mutex_unlock(&hashlock);	
	pf->f_count = 1;
	return pf;
}

void foo_hold(struct foo *pf)
{
	pthread_mutex_lock(&pf->f_lock);
	pf->f_count++;
	pthread_mutex_unlock(&pf->f_lock);	
}

void foo_rele(struct foo *pf)
{
	pthread_mutex_lock(&pf->f_lock);
	if (pf->f_count == 1)
	{
		pthread_mutex_unlock(&pf->f_lock);
		pthread_mutex_lock(&hashlock);
		pthread_mutex_lock(&pf->f_lock);
		if (--pf->f_count == 0)	
		{
			pthread_mutex_unlock(&pf->f_lock);
			pthread_mutex_unlock(&hashlock);
			pthread_mutex_destroy(&pf->f_lock);
			pthread_mutex_destroy(&hashlock);
			free(pf);
		}
		else
		{
			pthread_mutex_unlock(&pf->f_lock);
			pthread_mutex_unlock(&hashlock);
		}
	}
	else
	{
		pf->f_count--;
		pthread_mutex_unlock(&pf->f_lock);
	}
}



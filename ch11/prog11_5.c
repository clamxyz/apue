#include <pthread.h>
#include <stdlib.h>

struct foo
{
	int f_count;
	pthread_mutex_t f_lock;
	/* more staff */
};

struct foo *struct_alloc()
{
	struct foo *pf = NULL;

	if ((pf = (struct foo*)malloc(sizeof(struct foo))) != NULL)
	{
		pf->f_count = 1;
		pthread_mutex_init(&pf->f_lock, NULL);
	}

	return pf;
}

void hold(struct foo *pf)
{
	pthread_mutex_lock(&pf->f_lock);
	pf->f_count++;
	pthread_mutex_unlock(&pf->f_lock);	
}

void rele(struct foo *pf)
{
	pthread_mutex_lock(&pf->f_lock);
	if (--pf->f_count == 0)
	{
		pthread_mutex_unlock(&pf->f_lock);
		pthread_mutex_destroy(&pf->f_lock);
		/* other staff ... */
		free(pf);
	}
	else
	{
		pthread_mutex_unlock(&pf->f_lock);
	}
}




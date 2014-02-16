#include <pthread.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>

#define ARG_MAX 10240
pthread_key_t key;
pthread_once_t init_once = PTHREAD_ONCE_INIT;
pthread_mutex_t env_mutex = PTHREAD_MUTEX_INITIALIZER;

void thread_init()
{
	pthread_key_create(&key, free);
}

extern char **environ;

char *getenv(const char *name)
{
	int i, len;
	void *p;
	
	len = strlen(name);
	pthread_once(&init_once, thread_init);
	pthread_mutex_lock(&env_mutex);
	p = pthread_getspecific(key);
	if (p == NULL && (p = malloc(ARG_MAX)))
	{
		pthread_setspecific(key, p);
	}
	if (p != NULL)
	{
		for (i = 0; environ[i] != NULL; i++)
		{
			if (strncmp(environ[i], name, len) == 0 && environ[i][len] == '=')
			{
				strcpy((char *)p, environ[i]);	
				pthread_mutex_unlock(&env_mutex);
				return (char *)p;
			}
		}	
	}	
	pthread_mutex_unlock(&env_mutex);

	return NULL;
}	

int main()
{
	return 0;
}


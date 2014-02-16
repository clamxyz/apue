#include <limits.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>

extern char **environ;

pthread_once_t init_one = PTHREAD_ONCE_INIT;
pthread_mutex_t env_mutex;

void thread_init(void )
{
	pthread_mutexattr_t attr;
	
	pthread_mutexattr_init(&attr);
	pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
	pthread_mutex_init(&env_mutex, &attr);
	pthread_mutexattr_destroy(&attr);
}

int l_getenv_r(const char *name, char *envbuf, int len)
{
	int i;
	int nlen = strlen(name);

	pthread_once(&init_one, thread_init);
	pthread_mutex_lock(&env_mutex);
	for(i = 0;environ[i] != NULL; i++)
	{
		if (strncmp(environ[i], name, nlen) == 0 && environ[i][nlen] == '=')
		{
			if (strlen(environ[i]) >= len)
			{
				pthread_mutex_unlock(&env_mutex);
				return (ENOSPC);
			}
			strcpy(envbuf, environ[i]);
			pthread_mutex_unlock(&env_mutex);	
			return 0;				
		} 		
	}	
	pthread_mutex_unlock(&env_mutex);
	return ENOENT;	
}

int main(int argc, char *argv[])
{
	return 0;
}


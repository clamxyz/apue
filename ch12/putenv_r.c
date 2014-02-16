#include <pthread.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

extern char **environ;
static char **new_env, **old_env;
pthread_mutex_t	env_mutex;
pthread_once_t	init_once = PTHREAD_ONCE_INIT;

int l_putenv_r_thread_safe(const char *key, const char *val);

int l_putenv_r(const char *key, const char *value)
{
	int err;
	sigset_t newmask, oldmask;
	sigfillset(&newmask);
	err = pthread_sigmask(SIG_BLOCK, &newmask, &oldmask);
	if (err)
		return err;
	err = l_putenv_r_thread_safe(key, value);			
	if (err) return err;
	pthread_sigmask(SIG_SETMASK, &oldmask, NULL);
	return 0;	
}

static void thr_init(void)
{
	pthread_mutexattr_t attr;
	pthread_mutexattr_init(&attr);
	pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
	pthread_mutex_init(&env_mutex, &attr);
	pthread_mutexattr_destroy(&attr);
	new_env = old_env = environ;
}

static char **realloc_env(int add_num)
{
	char **p = environ;
	int plen = 0;
	int i = 0;
	while(*p++)
	{
		plen++;	
	}	
	p = (char **)malloc(sizeof(char *) * (plen + 1 + add_num));
	
	memset(p, 0, sizeof(char *) *(plen + 1 + add_num));
	for (i = 0;environ[i] != NULL; i++)
		p[i] = environ[i];	
	
	return p; 
}

int l_putenv_r_thread_safe(const char *key, const char *val)
{
	int olen; /*old value's length */
	int vlen; /*new value's length */
	int klen; /* key's length */
	int i;	/* for index */
	pthread_once(&init_once, thr_init);
	
	pthread_mutex_lock(&env_mutex);
	klen = strlen(key);
	vlen = strlen(val);
	for (i = 0;environ[i] != NULL; i++)
	{
		if (strncmp(environ[i], key, klen) == 0 && environ[i][klen] == '=')
		{
			/*hit it*/
			olen = strlen(&environ[i][klen+1]);
			if (olen < vlen) 
			{
				environ[i] = malloc(vlen + klen + 1 + 1);	/* one for '=' and one for '\0'*/
				strcpy(environ[i], key);
				strcat(environ[i], "=");
				strcat(environ[i], val);		 		
				pthread_mutex_unlock(&env_mutex);
				return 0;
			} 	
			else
			
				/* buffer is enough */
				strcpy(&environ[i][olen+1], val);		
				pthread_mutex_unlock(&env_mutex);
				return 0;
			}
		 }				
	}				
	/* not hit */
	if (new_env == old_env)	
	{
		new_env = realloc_env(1);	
	}
	else
	{
		environ = realloc(environ, sizeof(char *) *(i + 2));
		new_env = environ;
	}	
	environ[i] = (char *)malloc(sizeof(char) *(vlen + klen + 2));
	strcpy(environ[i], key);
	strcat(environ[i], "=");
	strcat(environ[i], val);	
	environ[i+1] = NULL;
	pthread_mutex_unlock(&env_mutex);

	return 0;
}

int main()
{
	return 0;
}


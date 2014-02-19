#include "public.h"
#include <sys/ipc.h>
#include <sys/shm.h>

struct shmbuf
{
	char stext[1024];
} *pshm;

int main()
{
	key_t key;
	int shmid;
	int subj_id = 1;
	char path[] = "/home/clamxyz/apue/ch15/";
	
	if ( (key = ftok(path, subj_id)) < 0)
	{
		perror("ftok error");
		return -1;
	}
	
	if ( (shmid = shmget(key, sizeof (struct shmbuf), IPC_CREAT | 0600)) < 0)
	{
		perror("shmget error");
		return -1;
	}
	if ( (pshm = (struct shmbuf *)shmat(shmid, 0, 0)) < 0)
	{
		perror("shmat error");
		return -1;
	}
	memcpy(pshm->stext, "Hello, World", sizeof ("Hello, World"));
	if ( shmdt(pshm) < 0)
	{
		perror("shmdt error");
		return -1;
	}	

	return 0;
}


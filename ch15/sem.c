#include "public.h"
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/wait.h>

 union semun {
               int              val;    /* Value for SETVAL */
               struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
               unsigned short  *array;  /* Array for GETALL, SETALL */
               struct seminfo  *__buf;  /* Buffer for IPC_INFO
                                           (Linux-specific) */
           };

int sem_init(char *path, int subj_id)
{
	key_t key;
	int semid;
	union semun sun;

	key = ftok(path, subj_id);
	if (key < 0)
		return -1;
	if ( (semid = semget(key, 1, IPC_CREAT | 0660)) < 0)
		return -1;
	sun.val = 1;
	if ( semctl(semid, 0, SETVAL, sun) < 0)
	{
		semctl(semid, 0, IPC_RMID);	
		return -1;
	}
	return semid;
}

int sem_lock(int semid)
{
	struct sembuf buf;
	buf.sem_num = 0;
	buf.sem_op = -1;
	buf.sem_flg = SEM_UNDO;
	return semop(semid, &buf, 1); 	
}

int sem_unlock(int semid)
{
	struct sembuf ops;
	ops.sem_num = 0;
	ops.sem_op = +1;
	ops.sem_flg = SEM_UNDO;
	return semop(semid, &ops, 1);
}

int sem_destroy(int semid)
{
	return semctl(semid, 0, IPC_RMID);
}

int main(int argc, char *argv[])
{
	pid_t pid;
	int semid;
	
	if (argc != 3)
	{
		printf("usage:%s <path> <subject_id>\n", argv[0]);
		return -1;
	}
	if ((semid = sem_init(argv[1], atoi(argv[2]))) < 0)
	{
		perror("sem_init error");
		return -1;
	}
	
	if ( (pid = fork()) < 0)
	{
		perror("fork error");
		return -1;
	}
	else if ( pid == 0)
	{
		int i = 0;
		while (i < 10)
		{
			if (0 > sem_lock(semid))
			{
				perror("child:sem_lock error");
				return -1;
			}
			printf("child:%08d\n", i++);	
			if ( 0 > sem_unlock(semid))
			{
				perror("child:sem_unlock error");
				return -1;
			}
		}	
		_exit(0);
	}
	else
	{
		int i = 0;
		while(i < 10)
		{
			if (sem_lock(semid) < 0)
			{
				perror("parent:sem_lock error");
				return -1;
			}
			printf("parent:%08d\n", i++);
			if (sem_unlock(semid) < 0)
			{
				perror("parent:sem_unlock error");
				return -1;
			}
		}	
		waitpid(pid, NULL, 0);
		if (sem_destroy(semid) < 0)
		{
			perror("sem_destroy error");
			return -1;
		}
	}
	return 0;
}


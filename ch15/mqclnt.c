#include "public.h"
#include <sys/ipc.h>
#include <sys/msg.h>

struct mcontext
{
	long type;
	char mtext[512];
}; 

int main(void)
{
	key_t key;
	int msgqid;
	char path[] = "/home/clamxyz/apue/ch15/";
	int subj_id = 5;
	struct mcontext mctxt;
	struct msqid_ds msgstat;

	key = ftok(path, subj_id);
	if (key < 0)
	{
		printf("ftok error[%d][%s]\n", errno, strerror(errno));
		return -1;
	}
	msgqid = msgget(key, 0660);
	if (msgqid < 0)
	{
		printf("msgget error[%d][%s]\n", errno, strerror(errno));
		return -1;
	}
	if (msgrcv(msgqid, &mctxt, sizeof(mctxt) - sizeof(long), 1, 0) < 0)
	{
		printf("msgrcv error [%d][%s]\n", errno, strerror(errno));
		return -1;
	}	
	printf("recv msg %s\n", mctxt.mtext);
	strcpy(mctxt.mtext, "Hello, I'm MSQ QUEUE Client\n");	
	mctxt.type = 2;
	printf("send msg %s\n", mctxt.mtext);
	msgsnd(msgqid, &mctxt, sizeof(mctxt) - sizeof(long), 0); 	
	printf("VIEW MSG QUEUE STAT\n");
	if (msgctl(msgqid, IPC_STAT, &msgstat) < 0)
	{
		printf("msgctl error[%d][%s]\n", errno, strerror(errno));
		return -1;
	}

	return 0;	
}


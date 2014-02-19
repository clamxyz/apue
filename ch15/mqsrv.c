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
	msgqid = msgget(key, IPC_CREAT | 0660);
	if (msgqid < 0)
	{
		printf("msgget error[%d][%s]\n", errno, strerror(errno));
		return -1;
	}
	strcpy(mctxt.mtext, "Hello, I'm MSQ QUEUE Server\n");	
	mctxt.type = 1;
	printf("send msg %s\n", mctxt.mtext);
	msgsnd(msgqid, &mctxt, sizeof(mctxt) - sizeof(long), 0); 	
	if (msgrcv(msgqid, &mctxt, sizeof(mctxt) - sizeof(long), 2, 0) < 0)
	{
		printf("msgrcv error [%d][%s]\n", errno, strerror(errno));
		return -1;
	}	
	printf("recv msg %s\n", mctxt.mtext);
	printf("VIEW MSG QUEUE STAT\n");
	if (msgctl(msgqid, IPC_STAT, &msgstat) < 0)
	{
		printf("msgctl error[%d][%s]\n", errno, strerror(errno));
		return -1;
	}
	printf("last rcv time = %d\n", msgstat.msg_rtime);	
	printf("last snd time = %d\n", msgstat.msg_stime);
	printf("last create time = %d\n", msgstat.msg_ctime);
	printf("last rcv pid = %d\n", msgstat.msg_lrpid);
	printf("last snd pid = %d\n", msgstat.msg_lspid);
	printf("Max number of Queue = %d\n", msgstat.msg_qbytes);
	printf("Message Queue qnum = %d\n", msgstat.msg_qnum);

	printf("DELETE MSG QUEUE\n");
	if (msgctl(msgqid, IPC_RMID, NULL) < 0)
	{
		printf("DELETE MSGQID FAILED.\n");
		return -1;
	}	

	return 0;	
}


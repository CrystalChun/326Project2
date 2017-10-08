#include <sys/ipc.h>
#include <sys/msg.h>
#include <iostream>
using namespace std;
struct buf {
	long mtype;
	int msg;
};
// Receiver - 251 and 997
int main(int argc, const char * argv[]) {
	buf msgbuf1;
	int qid = msgget(ftok(".",'u'),IPC_EXCL|IPC_CREAT|0600);
	cout <<"qid:" <<qid<<endl;
	int size = sizeof(msgbuf1)-sizeof(long);
	
	msgrcv(qid, (struct msgbuf *) & msgbuf1, size, 251, 0);
	cout  << ": gets message" << endl;
	cout << "message:" << msgbuf1.msg << endl;
	return 0;
}

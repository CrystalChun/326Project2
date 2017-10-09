#include <sys/ipc.h>
#include <sys/msg.h>
#include <iostream>
using namespace std;
struct buf {
	long mtype;
	int msg;
};
// Receiver - 251 and 997, both must terminate before this can terminate
int main(int argc, const char * argv[]) {
	buf msgbuf1;
	buf msgbuf2; 
	int qid = msgget(ftok(".",'u'),IPC_EXCL|IPC_CREAT|0600);
	cout <<"qid:" <<qid<<endl;
	int size = sizeof(msgbuf1)-sizeof(long);
	int times = 0;
	while(times < 21) {
		
		msgrcv(qid, (struct msgbuf *) & msgbuf1, size, 251, 0);
		times ++;
		cout  << "Receiver 251 " << times;
		cout << " message:" << msgbuf1.msg << endl;
		msgrcv(qid, (struct msgbuf *) &msgbuf2, size, 0, 0);
		if(msgbuf2.mtype == 257) {
			cout << msgbuf2.mtype << ": " << msgbuf2.msg << endl; msgsnd(qid, (struct msgbuf *) &msgbuf2, size, 0);
		}
		else {
			cout << "Not 257 " <<msgbuf2.msg << endl;
		}
	}
	return 0;
}

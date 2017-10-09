#include <sys/ipc.h>
#include <sys/msg.h>
#include <iostream>
using namespace std;
struct buf {
	long mtype;
	int msg;
};
// Receiver - 257 and 997
int main(int argc, const char* argv[]) {
	buf msgbuf1;
	int qid = msgget(ftok(".",'u'),0);
	cout << "qid: " << qid << endl;
	msgbuf1.mtype = 257;
	int size = sizeof(msgbuf1)-sizeof(long);
	int times = 0;
	while(times < 21) {
	times ++;
	msgrcv(qid,(struct msgbuf *) &msgbuf1, size, 257, 0);
	cout << "257 "<<times<<" message: " << msgbuf1.msg << endl;
}	return 0;
}

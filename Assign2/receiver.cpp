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
	int qid = msgget(ftok(".",'u'),IPC_EXCL|IPC_CREAT|0600);
	cout <<"qid:" <<qid<<endl;
	int size = sizeof(msgbuf1)-sizeof(long);
	int times = 0;
    while(msgrcv(qid,(struct msgbuf *) &msgbuf1, size, 0, 0) > -1)
    {
        cout << "Got message #" << ++times;
        if(msgbuf1.mtype == 251)
        {
            cout << "Sender: " << msgbuf1.mtype << endl<< "Message: " << msgbuf1.msg << endl;
        }
        else if(msgbuf1.mtype == 997)
        {
            cout << "Sender: " << msgbuf1.mtype << endl<< "Message: " << msgbuf1.msg << endl;
            cout << "Sending ack to 997 . . ." << endl;
            msgbuf1.mtype = 897;
            msgsnd(qid,(struct msgbuf *) &msgbuf1, size, 0);
        }
        else if(msgbuf1.mtype == 257)
        {
            // Send away
            cout <<"Receiver 1, got 257,message: "<< msgbuf1.msg<< " sending out . . ." << endl;
            msgsnd(qid,(struct msgbuf *) &msgbuf1, size, 0);
        }
        else{
            // Took out ack message, send back out
            cout << "Recever 1 took ack message 897, sending back out . . . " << endl;
            msgsnd(qid,(struct msgbuf *)&msgbuf1, size, 0);
        }
        
    }
	return 0;
}

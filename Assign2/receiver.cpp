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
	int qid = msgget(ftok(".",'u'),0);
	cout <<"qid:" <<qid<<endl;
	int size = sizeof(msgbuf1)-sizeof(long);
    bool rec1 = true;
    bool rec2 = true;
	int times = 0;
    while(msgrcv(qid,(struct msgbuf *) &msgbuf1, size, 0, 0) > -1 &&( rec1|| rec2))
    {
        cout << "Got message # " << ++times << endl;
        if(msgbuf1.mtype == 251)
        {
            if(msgbuf1.msg == 1000) {
                cout << "Sender 251 terminated." << endl;
                rec1 = false;
            } else {
                cout << "Sender: " << msgbuf1.mtype << endl<< "Message: " << msgbuf1.msg << endl;
        
            }
        }
        else if(msgbuf1.mtype == 997)
        {
            if(msgbuf1.msg < 100) {
                cout <<"997 is terminating " << endl;
                rec2 = false;
            } else {
                cout << "Sender: " << msgbuf1.mtype << endl<< "Message: " << msgbuf1.msg << endl;
                cout << "Sending ack to 997 . . ." << endl;

                msgbuf1.mtype = 897;
                msgsnd(qid,(struct msgbuf *) &msgbuf1, size, 0);
            }
        }
        else if(msgbuf1.mtype == 257)
        {
            // Send away
            cout <<"Receiver 1, got 257,message: "<< msgbuf1.msg<< " sending out . . ." << endl;
            msgsnd(qid,(struct msgbuf *) &msgbuf1, size, 0);
        }
        else{
            // Took out ack message, send back out
            cout << "Receiver 1 took ack message 897, sending back out . . . " << endl;
            msgsnd(qid,(struct msgbuf *)&msgbuf1, size, 0);
        }
        
    }
	return 0;
}

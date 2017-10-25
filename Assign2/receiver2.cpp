#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <iostream>
using namespace std;
struct buf {
	long mtype;
	int msg;
    bool is997;
};
// Receiver - 257 and 997
int main(int argc, const char* argv[]) {
	buf msgbuf1;
	int qid = msgget(ftok(".",'u'),0);
	cout << "qid: " << qid << endl;
	msgbuf1.mtype = 257;
    bool term = true;
	int size = sizeof(msgbuf1)-sizeof(long);
	int times = 0;
    while(msgrcv(qid,(struct msgbuf *) &msgbuf1, size, 1254, 0) > -1 && times < 5000)
    {
        cout << "Message # " << times<<endl;
        if(msgbuf1.msg % 257 == 0 && !msgbuf1.is997)
        {
		times++;
            cout << "Sender: 257 "<< endl<< "Message: " << msgbuf1.msg << endl;
            msgbuf1.mtype = 50;
            msgbuf1.msg = 1;
            msgbuf1.is997 = false;
            msgsnd(qid, (struct msgbuf*) &msgbuf1, size, 0);
        }
        else
        {
		    if(msgbuf1.msg < 100) {
                    cout << "997 terminating" << endl;
                    term = true;
            } else {
                times++;
                cout << "Sender: 997 "  << endl<< "Message: " << msgbuf1.msg << endl;
                cout << "Sending ack to 997 . . ." << endl;
                msgbuf1.mtype = 897;
                msgsnd(qid,(struct msgbuf *) &msgbuf1, size, 0);
                term = false;
            }
            
        }
    }
    msgbuf1.mtype = 50;
    msgbuf1.msg = 0;
    msgbuf1.is997 = false;
    msgsnd(qid, (struct msgbuf *) &msgbuf1, size, 0);
    cout << "Reached " << times << " messages. Terminating . . ." << endl;
    if(!term) {
        msgbuf1.mtype = 897;
        msgbuf1.msg = 1000;
        msgbuf1.is997 = true;
        msgsnd(qid, (struct msgbuf *) &msgbuf1, size, 0);
    }
    while(true) {
        struct msqid_ds bufa;
        int remove = msgctl(qid, IPC_STAT, &bufa);
        //cout << "something left " << endl;
        if(bufa.msg_qnum <=0) { break; }
    
    }
    
    msgctl(qid, IPC_RMID, NULL);
cout << "Terminated " << endl;
}

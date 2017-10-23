#include <sys/ipc.h>
#include <sys/types.h>
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
    while(msgrcv(qid,(struct msgbuf *) &msgbuf1, size, 0, 0) > -1 && times < 5000)
    {
        cout << "Message #" << times<<"Mtype: " << msgbuf1.mtype << endl;
        if(msgbuf1.mtype == 257)
        {
		times++;
            cout << "Sender: " << msgbuf1.mtype << endl<< "Message: " << msgbuf1.msg << endl;
            msgbuf1.mtype = 50;
            msgbuf1.msg = 1;
            msgsnd(qid, (struct msgbuf*) &msgbuf1, size, 0);
        }
        else if(msgbuf1.mtype == 997)
        {
		    if(msgbuf1.msg < 100) {
                    cout << "997 terminating" << endl;
            } else {
                times++;
                cout << "Sender: " << msgbuf1.mtype << endl<< "Message: " << msgbuf1.msg << endl;
                cout << "Sending ack to 997 . . ." << endl;
                msgbuf1.mtype = 897;
                msgsnd(qid,(struct msgbuf *) &msgbuf1, size, 0);
            }
        }
        else if(msgbuf1.mtype == 251)
        {
            // Send away
            cout <<"Receiver 2, got 251, message "<<msgbuf1.msg <<" sending out . . ." << endl;
            msgsnd(qid,(struct msgbuf *) &msgbuf1, size, 0);
        }
        else {
            cout << "Got random message, sending out..." << endl;
            msgsnd(qid,(struct msgbuf*)&msgbuf1, size, 0);
        }
        
    }
    msgbuf1.mtype = 50;
    msgbuf1.msg = 0;
    msgsnd(qid, (struct msgbuf *) &msgbuf1, size, 0);
    cout << "Reached " << times << " messages. Terminating . . .";
    while(true) {
        struct msqid_ds bufa;
        int remove = msgctl(qid, IPC_STAT, &bufa);
        if(bufa.msg_qnum <=0) { break; }
        cout << "Num messages: " << bufa.msg_qnum << endl;
    }
    msgctl(qid, IPC_RMID, NULL);
}

#include <sys/ipc.h>
#include <sys/msg.h>
#include <iostream>
using namespace std;

struct buf {
	long mtype;
	int msg;
    bool is997;
};

// Receiver 1 for senders 251 and 997
int main(int argc, const char * argv[]) {
	buf msgbuf1;
    int size = sizeof(msgbuf1)-sizeof(long);
    bool rec1 = true; // Whether or not sender 251 terminated
    bool rec2 = true; // Whether or not sender 997 terminated
    int times = 0; // The number of messages this receiver received
    
    // Create message queue
	int qid = msgget(ftok(".",'u'),IPC_EXCL|IPC_CREAT|0600);

    // Get messages with mtype 1248 while both senders are still active
    while(rec1 || rec2) {
        
        msgrcv(qid, (struct msgbuf *) &msgbuf1, size, 1248, 0);
        
        cout << "Got message # " << ++times << endl;
        // Sender 251
        if(!msgbuf1.is997 || msgbuf1.msg == 1000) {
            
            // Tests if sender 251 terminated
            if(msgbuf1.msg == 1000) {
                cout << "Sender 251 terminated." << endl;
                rec1 = false;
            } else {
                cout << "Sender: 251, Message: " << msgbuf1.msg << endl;
            }
        } else { // Sender 997
           
            // Tests if sender 997 terminated
            if(msgbuf1.msg < 100) {
                cout << "997 is terminating " << endl;
                rec2 = false;
            } else {
                cout << "Sender: 997, Message: " << msgbuf1.msg << endl;
                cout << "Sending ack to 997 . . ." << endl;
                msgbuf1.is997 = true;
                msgbuf1.mtype = 897;
                
                msgsnd(qid, (struct msgbuf *) &msgbuf1, size, 0);
                
                cout << "Sent ack to 997: " << msgbuf1.msg << endl;
            }
        }
    }
	return 0;
}

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

// Receiver 2 - listens to senders: 257 and 997
int main(int argc, const char* argv[]) {
	buf msgbuf1;
 
    bool term = true; // Flag to see if 997 sender terminated
    int size = sizeof(msgbuf1)-sizeof(long);
    int times = 0; // The number of messages this receiver received
    
    // Get qid
	int qid = msgget(ftok(".",'u'),0);

    // Listens to mtype 1254
    while(times < 5000)
    {
        msgrcv(qid, (struct msgbuf *) &msgbuf1, size, 1254, 0);
        cout << "Message # " << ++times <<endl;
        
        // Checks if the sender is 257 or 997
        if(!msgbuf1.is997)
        {
            cout << "Sender: 257, Message: " << msgbuf1.msg << endl;
            
            // This receiver is terminating
            if (times >= 5000) {
                // Flags 257 that this is terminating
                msgbuf1.mtype = 50;
                msgbuf1.msg = 0;
                msgbuf1.is997 = false;
                
                msgsnd(qid, (struct msgbuf *) &msgbuf1, size, 0);
                
                // If 997 hasn't terminated, get last message and terminate connection
                if(!term) {
                    msgrcv(qid, (struct msgbuf *) &msgbuf1, size, 1254, 0);
                    
                    msgbuf1.mtype = 897;
                    msgbuf1.msg = 1000;
                    msgbuf1.is997 = true;
                    
                    msgsnd(qid, (struct msgbuf *) &msgbuf1, size, 0);
                }
            } else {
                // Flag 257 that this isn't terminating
                msgbuf1.mtype = 50;
                msgbuf1.msg = 1;
                msgbuf1.is997 = false;
                
                msgsnd(qid, (struct msgbuf *) &msgbuf1, size, 0);
            }
            
        } else {
            // Tests to see if sender 997 is terminating
		    if(msgbuf1.msg < 100) {
                    cout << "997 terminating" << endl;
                    term = true;
            } else {
                cout << "Sender: 997, Message: " << msgbuf1.msg << endl;
                cout << "Sending ack to 997 . . ." << endl;
                
                // This receiver is terminating
                if(times >= 5000) {
                    // Tells 997 to terminate if this is terminating
                    msgbuf1.mtype = 897;
                    msgbuf1.is997 = true;
                    msgbuf1.msg = 1000;
                    msgsnd(qid, (struct msgbuf *) &msgbuf1, size, 0);
                    
                    // Flag to 257 to terminate
                    msgbuf1.mtype = 50;
                    msgbuf1.msg = 0;
                    msgbuf1.is997 = false;
                    
                    msgsnd(qid, (struct msgbuf *) &msgbuf1, size, 0);
                } else {
                    // Sends acknowledgement message to 997
                    msgbuf1.mtype = 897;
                    msgbuf1.is997 = true;
                    
                    msgsnd(qid, (struct msgbuf *) &msgbuf1, size, 0);
                }
                
                term = false;
            }
        }
    }
    
    cout << "Reached " << times << " messages. Terminating . . ." << endl;
    
    // Waits until 251 sender is also destroyed
    msgrcv(qid, (struct msgbuf *) &msgbuf1, size, 111, 0);

    // Deallocates message queue
    msgctl(qid, IPC_RMID, NULL);
    cout << "Terminated message queue" << endl;
}

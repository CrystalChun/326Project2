#include <sys/ipc.h>
#include <sys/msg.h>
#include <iostream>
using namespace std;

struct buf {
	long mtype; // M-type for message queue
	int msg;    // The message
    bool is997; // Tells whether or not this message is sender 997
};

// Sender 997
int main(int argc, const char * argv[]) {
    // Initialize this sender
	buf msgbuf1;

    int size = sizeof(msgbuf1) - sizeof(msgbuf1.mtype);
    int times = 0; // The number of times 997 sent a message
    bool term = false; // Whether or not receiver 2 terminated
    srand((unsigned)time(0));
    int lastSent = 0;
    
    // Get qid
	int qid = msgget(ftok(".",'u'),0);
    
    // Generate and send random number
    while(true) {
        int num = rand();
        
        // Sender 997 generated a number less than 100 and is terminating
        if(num < 100) {
            cout << "Sender 997 generated number less than 100, ";
            cout << "number: " << num << endl;
            msgbuf1.msg = num;
            
            // Send to receiver 1 that this is terminating
            msgbuf1.mtype = 1248;
            msgsnd(qid, (struct msgbuf*) &msgbuf1, size, 0);
            cout << "Sent term message: " << msgbuf1.msg << " to receiver 1" << endl;
            
            // Send to receiver 2 if receiver 2 hasn't terminated yet
            if(!term) {
                msgbuf1.mtype = 1254;
                msgsnd(qid, (struct msgbuf*) &msgbuf1, size, 0);
                cout << "Sent term message: " << msgbuf1.msg << " to receiver 2" << endl;
            }
            break;
        } else if(num % 997 == 0) {
            int send = rand() % 2;

            // Set mtype to either receiver 2 or receiver 1
            if(send == 1 && !term) {
                msgbuf1.mtype = 1254;
                cout << "Sending to receiver 2..." << endl;
            } else {
                msgbuf1.mtype = 1248;
                cout << "Sending to receiver 1..." << endl;
            }
            
            msgbuf1.msg = num;
            msgbuf1.is997 = true;
            times ++;
            
            msgsnd(qid,(struct msgbuf*) &msgbuf1, size, 0);
            
            cout << "Sender 997, message #" << times << ", message: " << num << endl;
            cout << "Waiting for acknowledge message " << endl;
            
            // Acknowledge message
            msgrcv(qid, (struct msgbuf*) &msgbuf1, size, 897, 0);
            cout << "Acknowledge message: " << msgbuf1.msg << endl;
            
            // Checks if receiver 2 has terminated
            if(msgbuf1.msg == 1000) {
                term = true;
                lastSent = num;
           }
       }
    }
    
    cout << "Sender 997 terminating . . . Messages sent: " << times << endl;
    cout << "Last sent to receiver 2: " << lastSent << endl;
    return 0;
}

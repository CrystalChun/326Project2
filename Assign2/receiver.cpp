// Crystal Chun 012680952
// File names associated with this assignment:
//  receiver.cpp, receiver2.cpp,
//  sender.cpp, sender1.cpp, sender2.cpp
// Description:
//  This is the first receiver (receiver.cpp)
//  and it receives messages
//  from sender 251 and 997.

#include <sys/ipc.h>
#include <sys/msg.h>
#include <iostream>
using namespace std;

// The msgbuf structure for sending messages between processes
struct buf {
    long mtype; // M-type for message queue
    int msg;    // The message
    bool is997; // Tells whether or not this message is sender 997
};

// Receiver 1 for senders 251 and 997
int main(int argc, const char * argv[]) {
    buf msgbuf1;
    int size = sizeof(msgbuf1) - sizeof(long);
    bool sen1 = true; // Whether or not sender 251 terminated
    bool sen2 = true; // Whether or not sender 997 terminated
    int times = 0; // The number of messages this receiver received
    
    // Create message queue
    int qid = msgget(ftok(".",'u'),IPC_EXCL|IPC_CREAT|0600);
    
    // Get messages with mtype 1248 while either senders are still active
    while(sen1 || sen2) {
        
        msgrcv(qid, (struct msgbuf *) &msgbuf1, size, 1248, 0);
        
        cout << "Got message # " << ++times << endl;
        // Sender 251
        if(!msgbuf1.is997 || msgbuf1.msg == 1000) {
            
            // Tests if sender 251 terminated
            if(msgbuf1.msg == 1000) {
                cout << "Sender 251 terminated." << endl;
                sen1 = false;
            } else {
                cout << "Sender: 251, Message: " << msgbuf1.msg << endl;
            }
        } else if (msgbuf1.is997 && sen2){ // Sender 997
            
            // Tests if sender 997 terminated
            if(msgbuf1.msg < 100) {
                cout << "997 is terminating " << endl;
                sen2 = false;
            } else {
                cout << "Sender: 997, Message: " << msgbuf1.msg << endl;
                cout << "Sending acknowledgement to 997 . . ." << endl;
                msgbuf1.is997 = true;
                msgbuf1.mtype = 897;
                
                msgsnd(qid, (struct msgbuf *) &msgbuf1, size, 0);
                
                cout << "Sent ack to 997: " << msgbuf1.msg << endl;
            }
        }
    }
    
    // Tell receiver 2 to deallocate message queue
    msgbuf1.msg = 0;
    msgbuf1.mtype = 111;
    
    msgsnd(qid, (struct msgbuf *) &msgbuf1, size, 0);
    
    cout << "Receiver 1, received " << times << " messages and now terminated." << endl;
    return 0;
}

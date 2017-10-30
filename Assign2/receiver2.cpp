// Crystal Chun 012680952
// File names associated with this assignment:
//  receiver.cpp, receiver2.cpp,
//  sender.cpp, sender1.cpp, sender2.cpp
// Description:
//  This is the second receiver (receiver2.cpp)
//  and it receives messages
//  from sender 257 and 997.

#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <iostream>
using namespace std;

// Function declarations
void setupStructAndSend(struct buf &, long, int, bool, int, int);
void send(int, int, struct buf &);
void terminateReceiver(struct buf &, bool, int, int);
void deallocateQueue(struct buf &, int, int);

// The msgbuf structure for sending messages between processes
struct buf {
    long mtype; // M-type for message queue
    int msg;    // The message
    bool is997; // Tells whether or not this message is sender 997
};

// Receiver 2 - listens to senders: 257 and 997
int main(int argc, const char* argv[]) {
    buf msgbuf1;
    
    bool sender997_Terminated = true; // Flag to see if 997 sender terminated
    int size = sizeof(msgbuf1) - sizeof(long);
    int times = 0; // The number of messages this receiver received
    
    // Get qid
    int qid = msgget(ftok(".",'u'),0);
    
    // Listens to mtype 1254
    while(times < 5000) {
        msgrcv(qid, (struct msgbuf *) &msgbuf1, size, 1254, 0);
        cout << "Message # " << ++times << endl;
        
        // Checks if the sender is 257 or 997
        if(!msgbuf1.is997) {
            cout << "Sender: 257, Message: " << msgbuf1.msg << endl;

            // Send flag to 257 that this isn't terminating
            setupStructAndSend(msgbuf1, 50, 1, false, qid, size);
        } else {
            // Tests to see if sender 997 is terminating
            if(msgbuf1.msg < 100) {
                // 997 is terminating
                cout << "997 terminating" << endl;
                sender997_Terminated = true;
            } else {
                cout << "Sender: 997, Message: " << msgbuf1.msg << endl;
                cout << "Sending ack to 997 . . ." << endl;

                // Sends acknowledgement message to 997
                setupStructAndSend(msgbuf1, 897, msgbuf1.msg, true, qid, size);
                sender997_Terminated = false;
            }
        }
    }
    terminateReceiver(msgbuf1, sender997_Terminated, qid, size);
    
    cout << "Reached " << times << " messages. Terminating . . ." << endl;
    
    deallocateQueue(msgbuf1, qid, size);
    cout << "Terminated and deallocated message queue." << endl;
}

void deallocateQueue(buf & msgbuf1, int qid, int size) {
    // Waits until 251 sender is also destroyed
    msgrcv(qid, (struct msgbuf *) &msgbuf1, size, 111, 0);
    
    // Waits until receiver 1 has finished
    msgrcv(qid, (struct msgbuf *) &msgbuf1, size, 111, 0);
    
    // Deallocates message queue
    msgctl(qid, IPC_RMID, NULL);
    cout << "Terminated message queue" << endl;
}
void terminateReceiver(buf & msgbuf1, bool sender997_terminated, int qid, int size) {
    msgrcv(qid, (struct msgbuf *) &msgbuf1, size, 1254, 0);
    if(msgbuf1.is997) {
        // Get last 257 message
        msgrcv(qid, (struct msgbuf *) &msgbuf1, size, 1254, 0);
    } else {
        // Get last 997 message
        msgrcv(qid, (struct msgbuf *) &msgbuf1, size, 1254, 0);
    }
    // Tells 257 to terminate
    setupStructAndSend(msgbuf1, 50, 0, false, qid, size);
    
    // If 997 hasn't terminated, tell it to terminate
    if(!sender997_terminated) {
        setupStructAndSend(msgbuf1, 897, 1000, true, qid, size);
    }
}

// Sets up the message buf structure by assigning them to the specified values
// and then sending the message buf to the message queue
// @param msgbuf1: The message buf structure
// @param mtype: The mtype for this message buf
// @param msg: The message to be sent
// @param is997: Whether or not this is sender 997
// @param qid: The message queue ID
// @param size: The size of the message buf
void setupStructAndSend(buf & msgbuf1, long mtype, int msg, bool is997, int qid, int size) {
    msgbuf1.msg = msg;
    msgbuf1.mtype = mtype;
    msgbuf1.is997 = is997;
    
    send(qid, size, msgbuf1);
}

// Sends the message in the msgbuf structure
// @param qid: The message queue id
// @param size: The size of the message buf
// @param msgbuf1: The message buf structure
void send(int qid, int size, buf & msgbuf1) {
    msgsnd(qid, (struct msgbuf *) &msgbuf1, size, 0);
}

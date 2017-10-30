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

void setupStructAndSend(struct buf &, long, int, bool, int, int);
bool handleSender251(struct buf &);
bool handleSender997(int, int, struct buf &);
void send(int, int, struct buf &);

// The msgbuf structure for sending messages between processes
struct buf {
    long mtype; // M-type for message queue
    int msg;    // The message
    bool is997; // Tells whether or not this message is sender 997
};

// Receiver 1 for senders 251 and 997
int main(int argc, const char * argv[]) {
    // Initialize
    buf msgbuf1;
    int size = sizeof(msgbuf1) - sizeof(long);
    bool sender251 = true; // Whether or not sender 251 terminated
    bool sender997 = true; // Whether or not sender 997 terminated
    int times = 0; // The number of messages this receiver received
    
    // Create message queue
    int qid = msgget(ftok(".",'u'),IPC_EXCL|IPC_CREAT|0600);
    
    // Get messages with mtype 1248 while either senders are still active
    while(sender251 || sender997) {
        
        msgrcv(qid, (struct msgbuf *) &msgbuf1, size, 1248, 0);
        
        cout << "Got message # " << ++times << endl;
        // Sender 251
        if(!msgbuf1.is997 || msgbuf1.msg == 1000) {
            sender251 = handleSender251(msgbuf1);
           
        } else if (msgbuf1.is997){ // Sender 997
            sender997 = handleSender997(qid, size, msgbuf1);
        }
    }
    
    // Tell receiver 2 to deallocate message queue
    setupStructAndSend(msgbuf1, 111, 0, false, qid, size);
    
    cout << "Receiver 1, received " << times << " messages and now terminated." << endl;
    return 0;
}

// Handles the 251 sender
// @param msgbuf1: The message buf structure
bool handleSender251(buf & msgbuf1) {
    // Tests if sender 251 terminated
    if(msgbuf1.msg == 1000) {
        cout << "Sender 251 terminated." << endl;
        return false;
    } else {
        cout << "Sender: 251, Message: " << msgbuf1.msg << endl;
        return true;
    }
}

// Handles the 997 sender
// @param qid: The message queue id
// @param size: The size of the message buf
// @param msgbuf1: The message buf structure
bool handleSender997(int qid, int size, buf &msgbuf1) {
    // Tests if sender 997 terminated
    if(msgbuf1.msg < 100) {
        cout << "997 is terminating " << endl;
        return false;
    } else {
        // 997 is not terminating
        cout << "Sender: 997, Message: " << msgbuf1.msg << endl;
        cout << "Sending acknowledgement to 997 . . ." << endl;
        
        // Send acknowledgement message
        setupStructAndSend(msgbuf1, 897, msgbuf1.msg, true, qid, size);
        
        cout << "Sent ack to 997: " << msgbuf1.msg << endl;
        return true;
    }
}
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

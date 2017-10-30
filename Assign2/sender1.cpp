// Crystal Chun 012680952
// File names associated with this assignment:
//  receiver.cpp, receiver2.cpp,
//  sender.cpp, sender1.cpp, sender2.cpp
// Description:
//  This is the first sender (257, sender1.cpp).
//  It sends messages to the second receiver whenever
//  it generates a random number that is divisible by
//  257.

#include <iostream>
#include <sys/ipc.h>
#include <sys/msg.h>

using namespace std;

// Function declarations
void setupStructAndSend(struct buf &, long, int, bool, int, int);
void send(int, int, struct buf &);

// The msgbuf structure for sending messages between processes
struct buf {
    long mtype; // M-type for message queue
    int msg;    // The message
    bool is997; // Tells whether or not this message is sender 997
};

// 257 Sender
int main(int argc, const char * argv[]) {
    // Initialize this sender
    buf msgbuf1;
    int size = sizeof(msgbuf1) - sizeof(msgbuf1.mtype);
    
    int times = 0; // Number of messages this sender sent
    srand((unsigned)time(0));
    
    // Get qid
    int qid = msgget(ftok(".",'u'),0);
    
    // Generates random numbers and sends it if the number is divisible by 257
    while(true) { 
        int num = rand();
        
        if(num % 257 == 0) {
            times ++;
            setupStructAndSend(msgbuf1, 1254, num, false, qid, size);

            cout << "Sender 257, message #" << times << ", message: " << num << endl;
            
            // Gets flag from receiver to see if it's terminating
            msgrcv(qid, (struct msgbuf*) &msgbuf1, size, 50, 0);
            
            // If receiver terminated, terminates this sender
            if(msgbuf1.msg == 0) {
                break;
            }
        }
    }
    
    cout << "257 terminating ... Sent " << times << " messages.";
    return 0;
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

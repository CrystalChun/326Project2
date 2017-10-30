// Crystal Chun 012680952
// File names associated with this assignment:
//  receiver.cpp, receiver2.cpp,
//  sender.cpp, sender1.cpp, sender2.cpp
// Description:
//  This is the first sender (997, sender2.cpp).
//  It sends messages to the first or second receiver
//  whenever it generates a random number that is
//  divisible by 997.

#include <sys/ipc.h>
#include <sys/msg.h>
#include <iostream>

using namespace std;

// Function declarations
void terminate(struct buf &, int, int, int, bool);
void setupStructAndSend(struct buf &, long, int, bool, int, int);
void send(int, int, struct buf &);

// The msgbuf structure for sending messages between processes
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
    bool receiver2_term = false; // Whether or not receiver 2 terminated
    srand((unsigned)time(0));
    
    // Get qid
    int qid = msgget(ftok(".",'u'),0);
    
    // Generate and send random number
    while(true) {
        int num = rand();
        
        // Sender 997 generated a number less than 100 and is terminating
        if(num < 100) {
            cout << "Sender 997 generated number less than 100, ";
            cout << "number: " << num << endl;
            
            terminate(msgbuf1, num, qid, size, receiver2_term);
            break;
        } else if(num % 997 == 0) {
            int send = rand() % 2;
            
            // Set mtype for either receiver 2 or receiver 1
            if(send == 1 && !receiver2_term) {
                msgbuf1.mtype = 1254;
                cout << "Sending to receiver 2..." << endl;
            } else {
                msgbuf1.mtype = 1248;
                cout << "Sending to receiver 1..." << endl;
            }
            
            setupStructAndSend(msgbuf1, msgbuf1.mtype, num, true, qid, size);
            times ++;
            
            cout << "Sender 997, message #" << times << ", message: " << num << endl;
            cout << "Waiting for acknowledge message " << endl;
            
            // Acknowledge message
            msgrcv(qid, (struct msgbuf*) &msgbuf1, size, 897, 0);
            cout << "Acknowledge message: " << msgbuf1.msg << endl;
            
            // Checks if receiver 2 has terminated
            if(msgbuf1.msg == 1000) {
                receiver2_term = true;
            }
        }
    }
    
    cout << "Sender 997 terminating . . . Number of messages sent: " << times << endl;
    return 0;
}

// This sender is terminating, so send messages to both receivers to notify them
// @param msgbuf1: The message buf structure
// @param msg: The message to send
// @param qid: The message queue ID
// @param size: The size of the message structure
// @param receiver2_terminated: Whether or not receiver two has already terminated
void terminate(buf & msgbuf1, int msg, int qid, int size, bool receiver2_terminated) {
    // Send message to receiver one
    setupStructAndSend(msgbuf1, 1248, msg, true, qid, size);
    
    // If receiver two hasn't terminated, send termination message to it
    if(!receiver2_terminated) {
        setupStructAndSend(msgbuf1, 1254, msg, true, qid, size);
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

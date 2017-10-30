// Crystal Chun 012680952
// File names associated with this assignment:
//  receiver.cpp, receiver2.cpp,
//  sender.cpp, sender1.cpp, sender2.cpp
// Description:
//  This is the first sender (251, sender.cpp).
//  It sends messages to the first receiver whenever
//  it generates a random number that is divisible by
//  251.

#include <iostream>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <signal.h>

using namespace std;

// Function declarations
void setupStructAndSend(struct buf &, long, int, bool, int, int);
void send(int, int, struct buf &);
void end(int sigNum);

// The msgbuf structure for sending messages between processes
struct buf {
    long mtype; // M-type for message queue
    int msg;    // The message
    bool is997; // Tells whether or not this message is sender 997
};


// Sender 251
int main(int argc, const char * argv[]) {
    buf msgbuf1;
    
    int size = sizeof(msgbuf1) - sizeof(msgbuf1.mtype);
    int times = 0;
    srand((unsigned)time(0));
    
    // Get qid
    int qid = msgget(ftok(".",'u'),0);
    
    // Listens for kill signal
    signal(SIGUSR1, end);
    
    // Generates random numbers and sends message if divisble by 251
    while(true) {
        int num = rand();
        
        if(num % 251 == 0) {
            times ++;
            setupStructAndSend(msgbuf1, 1248, num, false, qid, size);
            cout << "251 sender, message #" << times << ", message: " << num << endl;
        }
    }
    cout << "251 terminating... Sent: " << times << " messages." << endl;
    return 0;
}

// Listens for kill command for this process
// @param sigNum: The signal number associated with the kill command
void end(int sigNum) {
    if(sigNum == SIGUSR1) { // Checks if correct kill signal triggered
        cout << "Ending . .. " << endl;
        
        buf msgbuf2;
        int size = sizeof(msgbuf2) - sizeof(msgbuf2.mtype);
        int qid = msgget(ftok(".",'u'), 0);
        setupStructAndSend(msgbuf2, 1248, 1000, false, size, qid);
        
        // Sends alert to receiver 2 to notify that this terminated
        setupStructAndSend(msgbuf2, 111, 1000, false, size, qid);
        
        cout << "251 terminated. " <<endl;
        
        exit(0);
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

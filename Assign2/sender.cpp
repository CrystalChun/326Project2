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

// The msgbuf structure for sending messages between processes
struct buf {
    long mtype; // M-type for message queue
    int msg;    // The message
    bool is997; // Tells whether or not this message is sender 997
};

void end(int sigNum);

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
            msgbuf1.mtype = 1248;
            msgbuf1.is997 = false;
            msgbuf1.msg = num;
            
            msgsnd(qid,(struct msgbuf *) &msgbuf1, size, 0);
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
        msgbuf2.mtype = 1248;
        msgbuf2.msg = 1000;
        msgbuf2.is997 = false;
        int size = sizeof(msgbuf2) - sizeof(msgbuf2.mtype);
        int qid = msgget(ftok(".",'u'),0);
        
        msgsnd(qid, (struct msgbuf *) &msgbuf2, size, 0);
        
        // Sends alert to receiver 2 to notify that this terminated
        msgbuf2.mtype = 111;
        msgsnd(qid, (struct msgbuf *) &msgbuf2, size, 0);
        
        cout << "251 terminated. " <<endl;
        
        exit(0);
    }
}

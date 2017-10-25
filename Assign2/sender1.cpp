#include <iostream>
#include <sys/ipc.h>
#include <sys/msg.h>

using namespace std;

struct buf {
    long mtype;
    int msg;
    bool is997;
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
            msgbuf1.msg = num;
            msgbuf1.mtype = 1254;
            msgbuf1.is997 = false;
            
            msgsnd(qid,(struct msgbuf*) &msgbuf1, size, 0);
            cout << "Sender 257, message #" << times << ", message: " << num << endl;
            
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

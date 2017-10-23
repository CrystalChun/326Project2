#include <iostream>
#include <sys/ipc.h>
#include <sys/msg.h>

using namespace std;

struct buf {
    long mtype;
    int msg;
};

// 257 Sender
int main(int argc, const char * argv[]) {
    buf msgbuf1;
    msgbuf1.mtype = 257;
    int size = sizeof(msgbuf1) - sizeof(msgbuf1.mtype);
    
    // Get qid
    int qid = msgget(ftok(".",'u'),0);
    cout << "qid: " << qid << endl;
    
    srand((unsigned)time(0));
    int times = 0;
    
    // Generates random numbers and sends it if the number is divisible by 257
    while(true) { 
	    int num = rand();
        
	    if(num % 257 == 0) {
            msgbuf1.msg = num;
            msgsnd(qid,(struct msgbuf*) &msgbuf1, size, 0);
            cout << "Sender 257, message #" << times << ", message: " << num << endl;
            if(times > 3000) {
                break;
            }
            times ++;
	    }
    }
    cout << "257 terminating ... Sent " << times << " messages.";
    return 0;
}

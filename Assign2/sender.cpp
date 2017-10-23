
#include <iostream>
#include <sys/ipc.h>
#include <sys/msg.h>

using namespace std;

struct buf {
    long mtype;
    int msg;
};

// Sender - 251
int main(int argc, const char * argv[]) {
    buf msgbuf1;
    msgbuf1.mtype = 251;
    
    int size = sizeof(msgbuf1) - sizeof(msgbuf1.mtype);
    
    // Get qid
    int qid = msgget(ftok(".",'u'),0);
    cout << "qid: " << qid << endl;
    
    srand((unsigned)time(0));
	int times = 0;
    
    // Generates random numbers and sends message if divisble by 251
    while(times < 5) {
    	int num = rand();
    	if(num % 251 == 0) {
            times ++;
            msgbuf1.msg = num;
        	msgsnd(qid,(struct msgbuf *) &msgbuf1, size, 0);
            cout << "251 sender, message #" << times << ", message: " << num << endl;
    	}
    }
    cout << "251 terminating... Sent: " << times << " messages.";
    return 0;
}


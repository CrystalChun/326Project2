
#include <iostream>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <signal.h>

using namespace std;

struct buf {
    long mtype;
    int msg;
    bool is997;
};

void end(int sigNum);
// Sender - 251
int main(int argc, const char * argv[]) {
    buf msgbuf1;
    msgbuf1.mtype = 251;
    msgbuf1.is997 = false;
    int size = sizeof(msgbuf1) - sizeof(msgbuf1.mtype);
    
    // Get qid
    int qid = msgget(ftok(".",'u'),0);
    cout << "qid: " << qid << endl;
    srand((unsigned)time(0));
	int times = 0;
    signal(SIGUSR1, end);//get_info(qid, (struct msgbuf *) &msgbuf1, size, 251);
    // Generates random numbers and sends message if divisble by 251
    while(true) {
        
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
void end(int sigNum) {
    if(sigNum == SIGUSR1) {
        cout << "Ending . .. "<<endl;
        buf msgbuf2;
        msgbuf2.mtype = 1248;
        int size = sizeof(msgbuf2)-sizeof(msgbuf2.mtype);
        msgbuf2.msg = 1000;
        int qid = msgget(ftok(".",'u'),0);
        cout << qid << " qid" << endl;
        msgsnd(qid, (struct msgbuf *) &msgbuf2, size, 0);
        cout << "251 terminated. " <<endl;
        exit(0);
    }
}

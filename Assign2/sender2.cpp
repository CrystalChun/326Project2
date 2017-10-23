#include <sys/ipc.h>
#include <sys/msg.h>
#include <iostream>
using namespace std;

struct buf {
	long mtype;
	int msg;
};
// 997 sender
int main(int argc, const char * argv[]) { 
	buf msgbuf1;
    msgbuf1.mtype = 997;
    int size = sizeof(msgbuf1) - sizeof(msgbuf1.mtype);
    
    // Get qid
	int qid = msgget(ftok(".",'u'),0);
    cout << "qid: " << qid << endl;
   
    srand((unsigned)time(0));
    int times = 0;
    
    // Generate random number
    while(true) {
        int num = rand();
        
        if(num < 100) {
            cout << "Sender 997 generated number less than 100 " <<endl;
            msgbuf1.msg = num;
            cout <<"num: " << num << endl;
            msgsnd(qid, (struct msgbuf*) &msgbuf1, size, 0);
            msgsnd(qid, (struct msgbuf*) &msgbuf1, size, 0);

            break;
        }
        else if(num % 997 == 0) {
            msgbuf1.mtype = 997;
            msgbuf1.msg = num;
            times ++;
            
            msgsnd(qid,(struct msgbuf*) &msgbuf1, size, 0);
            
            cout << "Sender 997, message #" << times << ", message: " << num << endl;
            
            msgrcv(qid,(struct msgbuf*) &msgbuf1, size, 897, 0);
            cout << "Acknowledge message: " << msgbuf1.msg << endl;
        }
    }
    
    cout << "Sender 997 terminating . . . Messages sent: " << times << endl;
    return 0;
}

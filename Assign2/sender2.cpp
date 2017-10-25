#include <sys/ipc.h>
#include <sys/msg.h>
#include <iostream>
using namespace std;

struct buf {
	long mtype;
	int msg;
    bool is997;
};
// 997 sender
int main(int argc, const char * argv[]) { 
	buf msgbuf1;
    msgbuf1.mtype = 997;
    msgbuf1.is997 = true;
    int size = sizeof(msgbuf1) - sizeof(msgbuf1.mtype);
    
    // Get qid
	int qid = msgget(ftok(".",'u'),0);
    cout << "qid: " << qid << endl;
   
    srand((unsigned)time(0));
    int times = 0;
    bool term = false;
    // Generate random number
    while(true) {
        int num = rand();
        
        if(num < 100) {
            cout << "Sender 997 generated number less than 100 " <<endl;
            msgbuf1.msg = num;
            cout <<"num: " << num << endl;
            msgbuf1.mtype = 1248;
            msgsnd(qid, (struct msgbuf*) &msgbuf1, size, 0);
            cout << "Sent term message: " << msgbuf1.msg << endl;
            if(!term) {
                cout << "Sent term message to receiver 1 and 2 " << endl;
                msgbuf1.mtype = 1254;
                msgsnd(qid, (struct msgbuf*) &msgbuf1, size, 0);
                cout << "Sent term message: " << msgbuf1.msg << endl;
            }

            break;
        }
        else if(num % 997 == 0) {
            int send = 0;
            if(!term) {
                send = rand() % 2;
            }
                           
            cout << send << endl;
            if(send == 1) {
                msgbuf1.mtype = 1254;
                cout << "Sending to receiver 2" << endl;
            } else {
                msgbuf1.mtype = 1248;
                cout << "Sending to receiver 1" << endl;
            }
            
            msgbuf1.msg = num;
            times ++;
            
            msgsnd(qid,(struct msgbuf*) &msgbuf1, size, 0);
            
            cout << "Sender 997, message #" << times << ", message: " << num << endl;
            cout << "Waiting for acknowledge message " << endl;
            msgrcv(qid,(struct msgbuf*) &msgbuf1, size, 897, 0);
            cout << "Acknowledge message: " << msgbuf1.msg << endl;
            if(msgbuf1.msg == 1000) {
                term = true;
           }
       }
    }
    
    cout << "Sender 997 terminating . . . Messages sent: " << times << endl;
    return 0;
}

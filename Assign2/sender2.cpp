#include <sys/ipc.h>
#include <sys/msg.h>
#include <iostream>
using namespace std;

struct buf {
	long mtype;
	int msg;
};
int main(int argc, const char * argv[]) { 
	buf msgbuf1;
	int qid = msgget(ftok(".",'u'),0);
    	cout << "qid: " << qid << endl;
    	msgbuf1.mtype = 997;
    	int size = sizeof(msgbuf1) - sizeof(msgbuf1.mtype);
    	srand((unsigned)time(0));
	int times = 0;
    	// Generate random number
    	while(true){ 
    		int num = rand();
//    		cout << num << endl;
		if(num < 100) {
			break;
		}
    		else if(num % 997 == 0)
    		{
        		msgbuf1.msg = num;
			msgsnd(qid,(struct msgbuf*)& msgbuf1, size, 0);
        		cout <<times <<": " << num <<endl;
			msgrcv(qid,(struct msgbuf*)&msgbuf1,size,997,0);
			cout << "Acknowledge: " << msgbuf1.msg << endl;
    		}
   	 }
    
    return 0;
}

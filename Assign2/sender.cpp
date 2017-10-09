
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
    // Get qid
    int qid = msgget(ftok(".",'u'),0);
    cout << "qid: " << qid << endl;
    msgbuf1.mtype = 251;
    int size = sizeof(msgbuf1) - sizeof(msgbuf1.mtype);
    srand((unsigned)time(0));
    // Generate random number
	int times = 0;
    while(times < 20){
	 
    	int num = rand();
//    	cout << num << endl;
    	if(num % 251 == 0)
    	{
	times ++;
		msgbuf1.msg = num;
        	msgsnd(qid,(struct msgbuf*)& msgbuf1, size, 0);
        	cout <<times <<". " << num <<endl;
	//	break;
    	}
    }
    
    return 0;
}

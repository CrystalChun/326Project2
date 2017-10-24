#include <sys/ipc.h>
#include <sys/msg.h>
#include <iostream>
#include <sys/types.h>
using namespace std;
struct buf {
    long mtype;
    int msg;
    bool is997;
};
void clear(int qid, int size, buf & msgbuf1);
int main(int argc, const char * argv[]) {
    
    int qid = msgget(ftok(".",'u'), 0);
    cout << qid << endl;
    
    buf msgbuf1;
    msgbuf1.mtype = 1248;
    int size = sizeof(msgbuf1) - sizeof(msgbuf1.mtype);
    if(*argv[1] == 'c') { clear(qid, size, msgbuf1); } else {
int err =msgrcv(qid, (struct msgbuf *) &msgbuf1, size, 0, 0);
    cout << "Error: " <<err << endl;
    cout << "What's left: " << msgbuf1.mtype << " msg: " << msgbuf1.msg <<endl;
    } 
        return 0;
}
void clear(int qid, int size, buf & msgbuf1) {
    while(true) {
        struct msqid_ds bufa;
        int remove = msgctl(qid, IPC_STAT, &bufa);
        if(bufa.msg_qnum <= 0) {break; }
int err =msgrcv(qid, (struct msgbuf *) &msgbuf1, size, 0, 0);
    cout << "Error: " <<err << endl;
    cout << "What's left: " << msgbuf1.mtype << " msg: " << msgbuf1.msg <<endl;

    }
}

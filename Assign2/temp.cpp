#include <sys/ipc.h>
#include <sys/msg.h>
#include <iostream>
using namespace std;
struct buf {
    long mtype;
    int msg;
    bool is997;
};
int main() {
    int qid = msgget(ftok(".",'u'), 0);
    cout << qid << endl;
    buf msgbuf1;
    int size = sizeof(msgbuf1) - sizeof(long);
cout<<
    msgrcv(qid, (struct msgbuf *) &msgbuf1, size, 0, 0);
    if(msgbuf1.is997) {
       cout << " 997" << endl;    } else { cout << "Not 997 " << endl; }
    cout << "What's left: " << msgbuf1.mtype << " msg: " << msgbuf1.msg <<endl;

        return 0;
}

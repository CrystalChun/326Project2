//
//  main.cpp
//  Assign2
//
//  Created by Crystal Chun on 10/2/17.
//  Copyright © 2017 Crystal Chun. All rights reserved.
//

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
    // Get qid
    int qid = msgget(ftok(".",'u'),0);
    cout << "qid: " << qid << endl;
    msgbuf1.mtype = 257;
    int size = sizeof(msgbuf1) - sizeof(msgbuf1.mtype);
    
    // Generate random number
    
    srand((unsigned)time(0));
    while(true) { 
    int num = rand();
    //cout << num << endl;
    if(num % 257 == 0)
    {
        msgsnd(qid,(struct msgbuf*)& msgbuf1, size, 0);
        cout << num;
	break;
    }
    }
    
    return 0;
}

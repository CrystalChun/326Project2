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
int main(int argc, const char * argv[]) {
    buf msgbuf1;
    // Get qid
    int qid = msgget(ftok(".",'u'),0);
    msgbuf1.mtype = 251;
    int size = sizeof(msgbuf1) - sizeof(msgbuf1.mtype);
    
    // Generate random number
    
    srand((unsigned)time(0));
    int num = rand();
    cout << num << endl;
    if(num % 251 == 0)
    {
        msgsnd(qid,(struct msgbuf*)& msgbuf1, size, 0);
        cout << num;
    }
    
    
    return 0;
}


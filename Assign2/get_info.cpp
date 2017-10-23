#include<iostream>
#include<signal.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <cstring> // memcpy
#include <cstdlib> // malloc
#include "get_info.hpp"
using namespace std;
int gqid;
msgbuf *gmbuf;
int gsize;
long gmtype;

void sig_handler(int signo)
{
    cout << "in handler" << endl;
    if (signo == SIGUSR1) {
        gmbuf->mtype = gmtype;
        cout << "gmbuf: " << gmbuf << endl;
        msgsnd(gqid,gmbuf, gsize, 0);
        free (gmbuf);
        cout << "Sig "<<SIGUSR1 << endl;
        _exit(0);
    }
}
void get_info(int qid, msgbuf *mbuf, int size, long mtype) {
    
    signal(SIGUSR1, sig_handler);
    gqid = qid;
    gsize = size;
    gmtype = mtype;
    gmbuf = (struct msgbuf *)malloc(size);
    memcpy (gmbuf,mbuf,size);
}


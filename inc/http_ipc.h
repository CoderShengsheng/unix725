#ifndef __HTTP__IPC__H
#define __HTTP__IPC__H

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <err.h>
#include <fcntl.h>
#include "http_error.h"

#define FILEPATH "."

#define MODE 1

#ifndef BUFSIZE
#define BUFSIZE 1024
#else
#undef BUFSIZE
#endif

#ifndef CHECK 
  #define  check(msg) do{ \
    fprintf(stderr, "time:%s:error::%s::%s::line number:%d:function:%s::filename:%s\n",__TIME__,msg, \
    strerror(errno),__LINE__,__func__,__FILE__);\
    exit(0);\
  }while(0)

 #define __info(msg) do{\
  fprintf(stdout, "time:%s::%s::line number:%d:function:%s::filename:%s\n",__TIME__,msg\
   ,__LINE__,__func__,__FILE__);\
      }while(0)
#endif 

typedef struct _msg {
  long mtype;
  char mcontext[BUFSIZE];
} msg_t;

// ftok;
key_t Ftok() {
  key_t rval;
  if ((rval = ftok(FILEPATH, MODE)) == -1) {
      check("ftok");
  }
  return rval;
}

// int    msgget ( key_t  key , int  msgflg );
int open_queue() {
  int qid;
  if ((qid == msgget(ftok(".", 1), O_CREAT | 0666)) == -1) {
         check("msgget");
  }
  return qid;
}

int open_queue2(){
  int qid;
   if ((qid == msgget(ftok(".", 2), O_CREAT | 0666)) == -1) {
         check("msget2");
  }
  return qid;
  
}
int open_queue3(){
   int qid;
   if ((qid == msgget(ftok(".", 3), O_CREAT | 0666)) == -1) {
             check("msgget3");
  }
  return qid;
}

// int  msgsnd ( int msgid ,  struct msgbuf*msgp , int msgsz, int msgflg );

int send_queue(int msgid, msg_t *p) {
  int length, result;

  length = sizeof(msg_t) - sizeof(long);

  if ((result = msgsnd(msgid, p, length, 0) == -1)) {
  check("msgsnd");
  }
  return result;
}

// int  msgrcv( int  msgid , struct   msgbuf*  msgp ,  int msgsz ,  long msgtyp,
// int msgflg);

int rcv_queue(int msgid, msg_t *p, long type) {
  int length, result;
  length = sizeof(msg_t) - sizeof(long);
  if ((result = msgrcv(msgid, p, length, type, 0)) == -1) {
       printf("msgid=%d",msgid);
        check("msgrcv");
  }
  return result;
}

int peek_message( int qid, long type )
{
  int result, length;
  if((result = msgrcv( qid, NULL, 0, type, IPC_NOWAIT)) == -1)
  {
  if(errno == E2BIG)
  return(-1);
  }
  return(0);
}




#endif
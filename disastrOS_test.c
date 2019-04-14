
#include <stdio.h>
#include <unistd.h>
#include <poll.h>

#include "disastrOS.h"

// we need this to handle the sleep state
void sleeperFunction(void* args){
  printf("Hello, I am the sleeper, and I sleep %d\n",disastrOS_getpid());
  while(1) {
    getc(stdin);
    disastrOS_printStatus();
  }
}

void childFunction(void* args){
  printf("Hello, I am the child function %d\n",disastrOS_getpid());
  printf("I will iterate a bit, before terminating\n");
  int type=0;
  int mode=0;
  int fd=disastrOS_openResource(disastrOS_getpid(),type,mode);
  printf("fd=%d\n", fd);
  printf("PID: %d, terminating\n", disastrOS_getpid());
  
  
  // test semafori
  int ris;
  int fdc;
  //test open
  if (disastrOS_getpid() == 2 ){
	  printf("apro un semaforo con id negativo\n");
	  fdc = disastrOS_semOpen(-3,2);
	  printf("deve essere sbagliato-> %d\n",fdc);
	  
	  
	  printf("apro un Semaforo:\n");
	  fdc = disastrOS_semOpen(1,3);
	  printf("deve essere corretto-> %d\n",fdc);
	  
	  disastrOS_printStatus();
	  
	  
	  
	  
	  
	  
	  
	  //test close
	  printf("chiudo il semaforo:\n");
	  ris = disastrOS_semClose(fdc);
	  printf("deve essere corretto-> %d\n",ris);
	  
	  printf("chiudo un semaforo non aperto\n");
	  ris = disastrOS_semClose(fdc);
	  printf("deve essere sbagliato-> %d\n",ris);
	  
	  disastrOS_printStatus();
  }
  //test wait && post
  printf("apro un Semaforo:\n");
  ris = disastrOS_semOpen(0,1);
  
  printf("deve essere corretto-> %d\n",ris);
  fdc = ris;
  
  
  if (disastrOS_getpid() <6 ){
  printf("faccio una semwait sul semaforo, pid = %d\n",disastrOS_getpid());
  ris = disastrOS_semWait(fdc);
  }

  if(disastrOS_getpid() >=6){
   printf("faccio una sempost sul semaforo\n");
	ris = disastrOS_semPost(fdc);
  }
  
  /*//chiudo il semaforo
  printf("chiudo il semaforo:\n");
  ris = disastrOS_semClose(fdc);
  printf("deve essere corretto-> %d\n",ris);
  */
  for (int i=0; i<(disastrOS_getpid()+1); ++i){
    printf("PID: %d, iterate %d\n", disastrOS_getpid(), i);
    disastrOS_sleep((20-disastrOS_getpid())*5);
  }
  disastrOS_exit(disastrOS_getpid()+1);
}


void initFunction(void* args) {
  disastrOS_printStatus();
  printf("hello, I am init and I just started\n");
  disastrOS_spawn(sleeperFunction, 0);
  

  printf("I feel like to spawn 10 nice threads\n");
  int alive_children=0;
  for (int i=0; i<10; ++i) {
    int type=0;
    int mode=DSOS_CREATE;
    printf("mode: %d\n", mode);
    printf("opening resource (and creating if necessary)\n");
    int fd=disastrOS_openResource(i,type,mode);
    printf("fd=%d\n", fd);
    disastrOS_spawn(childFunction, 0);
    alive_children++;
  }

  disastrOS_printStatus();
  int retval;
  int pid;
  while(alive_children>0 && (pid=disastrOS_wait(0, &retval))>=0){ 
    disastrOS_printStatus();
    printf("initFunction, child: %d terminated, retval:%d, alive: %d \n",
	   pid, retval, alive_children);
    --alive_children;
  }
  printf("shutdown!");
  disastrOS_shutdown();
}

int main(int argc, char** argv){
  char* logfilename=0;
  if (argc>1) {
    logfilename=argv[1];
  }
  // we create the init process processes
  // the first is in the running variable
  // the others are in the ready queue
  printf("the function pointer is: %p", childFunction);
  // spawn an init process
  printf("start\n");
  disastrOS_start(initFunction, 0, logfilename);
  return 0;
}


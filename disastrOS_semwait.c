#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include "disastrOS.h"
#include "disastrOS_syscalls.h"
#include "disastrOS_semaphore.h"
#include "disastrOS_semdescriptor.h"

void internal_semWait(){
  // do stuff :)
  
  //prendo l'fd come argomento
  int sem_fd = running->syscall_args[0];
  
  //cerco il semdesescriptor tramite il suo fd
  SemDescriptor* sd = SemDescriptorList_byFd(&running->sem_descriptors,sem_fd);
  if(!sd){
	disastrOS_debug("semaforo non esistente \n");
	running->syscall_retvalue =  SEM_ERROR;
	return;
  }
  
  //prendo il semaforo dal semdescriptor
  Semaphore* s = sd->semaphore;
  if(!s){
	disastrOS_debug("semaforo non esistente \n");
	running->syscall_retvalue =  SEM_ERROR;
	return;
  }
  

  s->count--;
	
  if(s->count < 0){
	  List_detach(&s->descriptors, (ListItem*) sd->ptr);
	
	SemDescriptorPtr* ptr = sd->ptr;
	if (!ptr) {
        running->syscall_retvalue = -1;
        return;
	}
	
	//metto il descrittore nella lista dei waiting
	List_insert(&s->waiting_descriptors,s->waiting_descriptors.last,(ListItem*)ptr);
	
	//metto il processo in waiting
	running->status = Waiting;
	
	//levo il processo dalla lista dei ready
	//List_detach(&ready_list,(ListItem*)running);
	
	//metto il processo dalla lista dei waiting
	List_insert(&waiting_list,waiting_list.last,(ListItem*)running);
	
	PCB* p= (PCB*)List_detach(&ready_list,ready_list.first);
	running = p;

	
	
  }
  disastrOS_printStatus();
  running->syscall_retvalue = 0;
}

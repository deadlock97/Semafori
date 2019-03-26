#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include "disastrOS.h"
#include "disastrOS_syscalls.h"
#include "disastrOS_semaphore.h"
#include "disastrOS_semdescriptor.h"

void internal_semWait(){
  // do stuff :)
  
  int sem_id = running->syscall_args[0];
  
  Semaphore* s = SemaphoreList_byId(&semaphores_list,sem_id);
  if(sem_id < 0  || !s){
	disastrOS_debug("semaforo non esistente \n");
	running->syscall_retvalue =  SEM_ERROR;
	return;
  }
  if(s->count > 0){
	s->count--;
	return;
  }
  else{
	 
	SemDescriptor* sd = SemDescriptorList_byFd(&running->sem_descriptors,sem_id);
	SemDescriptorPtr* ptr = sd->ptr;
	
	//rimuovo ptr dalla lista dei descrittori di s
	List_detach(&s->descriptors,(ListItem*)ptr);
	
	//metto il descrittore nella lista dei waiting
	List_insert(&s->waiting_descriptors,s->waiting_descriptors.last,(ListItem*)ptr);
	
	//metto il processo in waiting
	running->status = Waiting;
	
	//levo il processo dalla lista dei ready
	List_detach(&ready_list,(ListItem*)running);
	
	//metto il processo dalla lista dei waiting
	List_insert(&waiting_list,waiting_list.last,(ListItem*)running);
	
	
	
  }
  
  
  
}

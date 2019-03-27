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
  
  //verifico che il contatore sia > 0: se si, decremento il contatore, altrimenti inserisco il processo nella lista di attesa
  
  if(s->count > 0){
	s->count--;
	return;
  }
  else{
	 
	
	SemDescriptorPtr* ptr = sd->ptr;
	
	//rimuovo ptr dalla lista dei descrittori di s (necessario?)
	//List_detach(&s->descriptors,(ListItem*)ptr);
	
	//metto il descrittore nella lista dei waiting
	List_insert(&s->waiting_descriptors,s->waiting_descriptors.last,(ListItem*)ptr);
	
	//metto il processo in waiting
	running->status = Waiting;
	
	//levo il processo dalla lista dei ready
	List_detach(&ready_list,(ListItem*)running);
	
	//metto il processo dalla lista dei waiting
	List_insert(&waiting_list,waiting_list.last,(ListItem*)running);
	
	
	running->syscall_retvalue = 0;
	
	
	
  }
  
 
  
  
  
}

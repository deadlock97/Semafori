#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include "disastrOS.h"
#include "disastrOS_syscalls.h"
#include "disastrOS_semaphore.h"
#include "disastrOS_semdescriptor.h"

void internal_semClose(){
  
  //prendo il fd del semaforo da chiudere
  int sem_fd = running->syscall_args[0];
  
  SemDescriptor* sd = SemDescriptorList_byFd(&running->sem_descriptors,sem_fd);
  if(!sd){
	running->syscall_retvalue = SEM_ERROR;
	return;
  }
  
  //rimuovo il descritto dalla lista dei processi
  sd = (SemDescriptor*)List_detach(&running->sem_descriptors,(ListItem*)sd);
  assert(sd);
  
  Semaphore* s = sd->semaphore;
  
  //rimuovo il puntatore del sem descriptor dalla lista dei semafori
  SemDescriptorPtr* sdptr = (SemDescriptorPtr*)List_detach(&s->descriptors,(ListItem*)(sd->ptr));
  assert(sdptr);
  Semaphore_free(s);
  SemDescriptorPtr_free(sdptr);
  running->syscall_retvalue = 0;
  
	
}

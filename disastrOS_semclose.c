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
	disastrOS_debug("semdescriptor non trovato, errore\n");
	running->syscall_retvalue = SEM_ERROR;
	return;
  }
  
  //rimuovo il descrittore dalla lista dei sem descriptor del processo in esecuzione
  sd = (SemDescriptor*)List_detach(&running->sem_descriptors,(ListItem*)sd);
  
  
  Semaphore* s = sd->semaphore;
  
  //rimuovo il puntatore del sem descriptor dalla lista del semafori
  SemDescriptorPtr* sdptr = (SemDescriptorPtr*)List_detach(&s->descriptors,(ListItem*)(sd->ptr));
  
  SemDescriptor_free(sd);
  SemDescriptorPtr_free(sdptr);
  
  if(s->descriptors.size == 0){
	s = (Semaphore*) List_detach(&semaphores_list, (ListItem*) s);
    Semaphore_free(s);
  }
  
  running->syscall_retvalue = 0;
  
	
}

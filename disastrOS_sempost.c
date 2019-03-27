#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include "disastrOS.h"
#include "disastrOS_syscalls.h"
#include "disastrOS_semaphore.h"
#include "disastrOS_semdescriptor.h"

void internal_semPost(){
  
	//prendo l'id del semaforo
	int sem_id = running->syscall_args[0];
	Semaphore* s = SemaphoreList_byId(&semaphores_list,sem_id);
	
	if(sem_id < 0  || !s){
		disastrOS_debug("semaforo non esistente \n");
		running->syscall_retvalue =  SEM_ERROR;
		return;
	}
	
	if(s->count < 0){
		SemDescriptor* sd = SemDescriptorList_byFd(&running->sem_descriptors,sem_id);
	}
	else{
		s->count++;
	}
}

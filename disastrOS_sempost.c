#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include "disastrOS.h"
#include "disastrOS_syscalls.h"
#include "disastrOS_semaphore.h"
#include "disastrOS_semdescriptor.h"

void internal_semPost(){
  
	//prendo l'id del semaforo
	int sem_fd = running->syscall_args[0];
	SemDescriptor* sd = SemDescriptorList_byFd(&semaphores_list,sem_fd);
	
	Semaphore* s = sd->semaphore;
	
	if(!s){
		disastrOS_debug("semaforo non esistente \n");
		running->syscall_retvalue =  SEM_ERROR;
		return;
	}
	
	if(s->count > 0){
		s->count++;
	}
	else{
		SemDescriptorPtr* towakeptr = (SemDescriptorPtr*)List_detach(&s->waiting_descriptors,s->waiting_descriptors.first);
		SemDescriptor* sdtowake = towakeptr->descriptor;
		
		sdtowake->pcb->status=Ready;
		List_insert(&ready_list,ready_list.last,(ListItem*)(sdtowake->pcb));
		if(!(s->waiting_descriptors.first))
			s->count++;
		
	}
}

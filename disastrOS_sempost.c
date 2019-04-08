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
	SemDescriptor* sd = SemDescriptorList_byFd(&running->sem_descriptors,sem_fd);
	
	Semaphore* s = sd->semaphore;
	
	if(!s){
		disastrOS_debug("semaforo non esistente \n");
		running->syscall_retvalue =  SEM_ERROR;
		return;
	}
	
	
		s->count++;
	
	if(s->count <= 0){
		//2
		SemDescriptorPtr* sdtowakeptr = (SemDescriptorPtr*)List_detach(&s->waiting_descriptors,s->waiting_descriptors.first);
		SemDescriptor* sdtowake = sdtowakeptr->descriptor;
		
		List_detach(&waiting_list,(ListItem*)sdtowake->pcb);
		List_insert(&s->descriptors, s->descriptors.last, (ListItem*) sdtowakeptr);
		running->status=Ready;
		running = sdtowake->pcb;
		List_insert(&ready_list,ready_list.last,(ListItem*)running); //1
		
		
	}
}

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
	if(!sd){
		disastrOS_debug("descrittore del semaforo non esistente \n");
		running->syscall_retvalue =  SEM_ERROR;
		return;
	}
	Semaphore* s = sd->semaphore;
	
	
	if(!s){
		disastrOS_debug("semaforo non esistente \n");
		running->syscall_retvalue =  SEM_ERROR;
		return;
	}
	
	
	s->count++;

	if(s->count <= 0){
		
		// prendo il sem_descriptorptr del primo elemento nella lista dei descrittori in attesa del semaforo dato
		SemDescriptorPtr* sdtowakeptr = (SemDescriptorPtr*)List_detach(&s->waiting_descriptors,s->waiting_descriptors.first);
		
		SemDescriptor* sdtowake = sdtowakeptr->descriptor;
		List_insert(&s->descriptors, s->descriptors.last, (ListItem*) sdtowakeptr);
		//tolgo il processo (relativo al semdescriptor da svegliare) dalla lista dei processi in attesa
		List_detach(&waiting_list,(ListItem*)sdtowake->pcb);
		
		
		//cambio il processo da mettere in running
		running->status=Ready;
		List_insert(&ready_list,ready_list.last,(ListItem*)running); 
		running = sdtowake->pcb;
		running->status = Running;
		
		
		
	}
	disastrOS_printStatus();
	running->syscall_retvalue = 0;
}

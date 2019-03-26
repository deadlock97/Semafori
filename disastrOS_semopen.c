#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include "disastrOS.h"
#include "disastrOS_syscalls.h"
#include "disastrOS_semaphore.h"
#include "disastrOS_semdescriptor.h"

void internal_semOpen(){
	int sem_id = running->syscall_args[0];	
	
	if(sem_id < 0){
		disastrOS_debug("Errore: id del semaforo <0 \n");
		running->syscall_retvalue = SEM_ERROR;
		return;
	}
	
	int count = running->syscall_args[1];
	Semaphore* s;
	
	//controllo se il semaforo esiste gia
	
	s = SemaphoreList_byId(&semaphores_list,sem_id);
	
	
	if(!s){
		s = Semaphore_alloc(sem_id,count);
		if(!s){
			disastrOS_debug("Errore nell'allocazione del semaforo\n");
			running->syscall_retvalue = SEM_ERROR;
			return;
		}
	}
	SemDescriptor* sd = SemDescriptor_alloc(running->last_sem_fd,s,running);
	if(!sd){
		disastrOS_debug("Errore nell'allocazione del descrittore del semaforo\n");
		running->syscall_retvalue = SEM_ERROR;
		return;
	}
	running->last_sem_fd++;
	
	SemDescriptorPtr* sdptr = SemDescriptorPtr_alloc(sd);
	List_insert(&running->sem_descriptors,running->sem_descriptors.last,(ListItem*)sd);
	
	sd->ptr =sdptr;
	
	//restituisco l'id del semaforo
	running->syscall_retvalue = s->id;
}

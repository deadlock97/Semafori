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
	
	//printf("id = %d\n",sem_id);
	//printf("count = %d\n",count);
	
	
	//controllo se il semaforo esiste gia
	printf("s : %p\n",s);
	s = SemaphoreList_byId(&semaphores_list,sem_id);
	printf("s : %p\n",s);
	
	if(!s){
		s = Semaphore_alloc(sem_id,count);
		if(!s){
			disastrOS_debug("Errore nell'allocazione del semaforo\n");
			running->syscall_retvalue = SEM_ERROR;
			return;
		}
		List_insert(&semaphores_list,semaphores_list.last,(ListItem*)s);
	}
	
	SemDescriptor* sd = SemDescriptor_alloc(running->last_sem_fd,s,running);
	if(!sd){
		disastrOS_debug("Errore nell'allocazione del descrittore del semaforo\n");
		running->syscall_retvalue = SEM_ERROR;
		return;
	}
	
	List_insert(&running->sem_descriptors,running->sem_descriptors.last,(ListItem*)sd);
	running->last_sem_fd++;
	
	SemDescriptorPtr* sdptr = SemDescriptorPtr_alloc(sd);
	
	//aggiungo all lista dei descrittori del semaforo sdptr
	List_insert(&s->descriptors,s->descriptors.last,(ListItem*)sdptr);
	
	
	sd->ptr =sdptr;
	
	//restituisco l'id del semaforo
	
	running->syscall_retvalue = sd->fd;
}

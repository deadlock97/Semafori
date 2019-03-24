#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include "disastrOS.h"
#include "disastrOS_syscalls.h"
#include "disastrOS_semaphore.h"
#include "disastrOS_semdescriptor.h"

void internal_semOpen(){
	int id = running->syscall_args[0];	
	int count = running->syscall_args[1];
	
	//controllo se l'id del semaforo esiste gia
	if(SemaphoreList_byId(semaphores_list,id) != 0)
		running->syscall_retvalue = -1;
	Semaphore* s = Semaphore_alloc(id,count);
	if(!s)
		running->syscall_retvalue;
	SemDescriptor_alloc(1,s,running);
}

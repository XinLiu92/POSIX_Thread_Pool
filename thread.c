/*
 * thread.c - C portion of the thread library implementation.
 *
 *          This just provides a stub for the visible functions!
 */

#include "thread.h"



//assmebly function declare
extern void asm_yield(TCB *cur, TCB *next);


//not write cleanup yet
void thread_cleanup(){
	//remember the pointer to the base of the stack for the current thread so that it can be freed at the next call to thread_cleanup
	//uint64_t * s_tmp = readyList_head->stack;
	
	//free the stack for the last thread that called thread_cleanup
	free(readyList_head->stack);

	//remove the tcb from the front of the ready list
	if(readyList_head->next != NULL){
		TCB *tmp = readyList_head;
		readyList_head = readyList_head->next;
		tmp->next = NULL;
		free(tmp);
	}else{
		free(readyList_head);
	}

	asm_yield(NULL, readyList_head);
	
	return;
}

//thread start function
void thread_start(void (*work)(void *),void *info){
	work(info);
	thread_cleanup();
}


//thread create 
long thread_create(void (*func)(void*), void*info)
{	
	//create a new tcb if the readylist is empty
	if(readyList_head == NULL){
		
		TCB *tcb_main_thread =(TCB *)malloc(80);

		if(tcb_main_thread == NULL ) return 0;
		
		readyList_head = tcb_main_thread;

		readyList_tail = tcb_main_thread;

		readyList_tail->next = NULL;
	}	

	// create another tcb linked it behind the one created before
	TCB *tcb =(TCB *)malloc(80);

	if(tcb == NULL) 
		return 0;
	
	readyList_tail-> next = tcb;
	readyList_tail = tcb;
	
	readyList_tail->next = NULL;
	
	//malloc a let the second node's   pointer point to it
	readyList_tail->stack = malloc(65536);

	if(readyList_tail->stack == NULL) return 0;

	//save the thread_create function's arguement
	readyList_tail->rdi = (uint64_t)func;
	readyList_tail->rsi = (uint64_t)info;
	// save the third to last into rsp
	readyList_tail->rsp = (uint64_t*)readyList_tail->stack+8189;
	//thread start function address to second to last of stack
	readyList_tail->stack[8190] = (uint64_t)thread_start;

  	return (long)readyList_head;
}




void thread_yield(void)
{
	
	//if ready list is empty, malloc tcb
	if(readyList_head == NULL){

		TCB *tcb_main_thread =(TCB *)malloc(80);

		if(tcb_main_thread == NULL ) return;
		
		readyList_head = tcb_main_thread;

		readyList_tail = tcb_main_thread;

		readyList_tail->next = NULL;
		asm_yield(readyList_tail, readyList_head);

	}

	else if( readyList_head == readyList_tail){
		asm_yield(readyList_tail, readyList_head);
	}

	else{
		//rotate TCB 
		TCB *tmp = readyList_head->next;
		
		readyList_tail->next = readyList_head;

		readyList_head->next = NULL;

		readyList_head = tmp;

		readyList_tail = readyList_tail->next;
		asm_yield(readyList_tail, readyList_head);

	}
	
	//call assembly function the argument *cur take rdi, *next take rsi
	
	//asm_yield(readyList_tail, readyList_head);
	
}





int thread_mutex_init(thread_mutex_t *mutex)
{	
  //create TCB for main thread
	if(mutex == NULL) return 0;
	if(readyList_head == NULL){
		
		TCB *tcb_main_thread =(TCB *)malloc(80);

		if(tcb_main_thread == NULL ) return 0;
		
		readyList_head = tcb_main_thread;

		readyList_tail = tcb_main_thread;

		readyList_tail->next = NULL;
	}	
  //initial mutex
	mutex -> owner = 0;
	queue_head =NULL;
	queue_tail = NULL;
	mutex -> locked = false;
	return 1;
}


int thread_mutex_lock(thread_mutex_t *mutex)
{	

	if(mutex == NULL) return 0;
	//check locked or unlocked
    if(mutex-> locked == false){
    	mutex -> owner = (long)readyList_head;
    	mutex -> locked = true;
    }


    //add the caller to the end of the mutx queue 
	else if(mutex -> owner == (long)readyList_head) return 0;
	
	else{

		if(readyList_head != NULL && readyList_tail != NULL){
			TCB* tmp = readyList_head;
	
			if(readyList_head -> next != NULL) readyList_head = readyList_head->next;
			tmp->next = NULL;

			if(queue_head == NULL && queue_tail == NULL){
				queue_head = (thread_mutex_t *)tmp;
				queue_tail = (thread_mutex_t *)tmp;
				queue_tail->next = NULL;
				queue_tail->prev = NULL;
				queue_head->prev = NULL;
				queue_head->next = NULL;
			}
			
			
			queue_head-> prev = (thread_mutex_t*)tmp;
			queue_head=(thread_mutex_t*)tmp -> next;

			queue_head = (thread_mutex_t*)tmp;
			
			
			queue_head = (thread_mutex_t *)tmp;


			asm_yield((TCB *)queue_head, readyList_head);
		}
	}

  return 1;
}

int thread_mutex_unlock(thread_mutex_t *mutex)
{	
  if(mutex == NULL) return 0;
  if(queue_head == NULL && queue_tail == NULL){
  	//queue is empty
  	mutex -> owner = 0;
  	//fprintf(stderr, "deadlock\n");

  	return 0;
  }	
  	//take the queue element which need to be pop out
  	thread_mutex_t *tmp = queue_tail;
  	if(queue_tail -> prev != NULL){
  		queue_tail = queue_tail-> prev;
  		queue_tail->next = NULL;
  	} 

  	
  	
  	readyList_tail->next = (TCB *) tmp;
  	readyList_tail = readyList_tail->next;
  	mutex->locked = false;
  	 return 1;
  
}


int thread_cond_init(thread_cond_t *condvar)
{
  return 1;
}

int thread_cond_wait(thread_cond_t *condvar, thread_mutex_t *mutex)
{
  return 1;
}

int thread_cond_signal(thread_cond_t *condvar)
{
  return 1;
}



// interface for CS520 threads library
//
// Note: thread_mutex_t and thread_cond_t definitions are incomplete!
//
#ifndef THREAD_H
#define THREAD_H
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

//TCB struct
typedef struct TCB{
	
	uint64_t * rsp;
	uint64_t rbx;
	uint64_t r12;
	uint64_t r13;
	uint64_t r14;
	uint64_t r15;
	uint64_t rdi;
	uint64_t rsi;
	struct TCB * next;
	uint64_t * stack;	

}TCB;


//define two TCB pointer
TCB *readyList_head;
TCB *readyList_tail;



long thread_create(void (*)(void*), void*);
void thread_yield(void);

typedef struct thread_mutex_t { 

bool locked;
long owner;
struct thread_mutex_t * next; 
struct thread_mutex_t * prev; 


}thread_mutex_t;

thread_mutex_t * queue_head;
thread_mutex_t * queue_tail;


int thread_mutex_init(thread_mutex_t *);
int thread_mutex_lock(thread_mutex_t *);
int thread_mutex_unlock(thread_mutex_t *);

typedef struct { 


} thread_cond_t;


int thread_cond_init(thread_cond_t *);
int thread_cond_wait(thread_cond_t *, thread_mutex_t *);
int thread_cond_signal(thread_cond_t *);

#endif
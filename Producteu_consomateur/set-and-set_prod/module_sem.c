#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdbool.h>
#include "semaphore.h"



void init_sem(str_sem *my_struct_sem, int nb_count){
    my_struct_sem = malloc(sizeof(str_sem));
    my_struct_sem->lock=0;
    my_struct_sem->count=nb_count;
}


void lock_sem(int *bool_lock){
    __asm__ ( "enter:"
              "movl $1, %%eax;"
              "xchgl %%eax, %0;"
              "testl %%eax, %%eax;"   //set ZF to 1 if eax == 0
              "jnz enter;"           //saute si eax n'est pas 0  
              :"=m"(*bool_lock) /* related to %0*/
              :
              :"%eax");         
}
     
     
void unlock_sem(int *bool_lock){          
    __asm__ ( "movl $0, %%eax;"
              "xchgl %%eax, %0;"
              :"=m"(*bool_lock) /* related to %0*/
              :
              :"%eax"); 
}

void wait_sem(str_sem *my_struct_sem){
     int *count= &(my_struct_sem->count);  
     while(*count <0){
     }
     lock_sem(&(my_struct_sem->lock));
     *count = *count-1;
     unlock_sem(&(my_struct_sem->lock));  
}

void post_sem(str_sem *my_struct_sem){
     int *count= &(my_struct_sem->count);  
     lock_sem(&(my_struct_sem->lock));
     *count = *count+1;
     unlock_sem(&(my_struct_sem->lock));  
}

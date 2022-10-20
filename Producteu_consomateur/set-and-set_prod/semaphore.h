#ifndef SEMAPHORE_H
#define SEMAPHORE_H

typedef  struct _str_sem {
       int lock;
       int count;
 }str_sem;


void lock_sem(int *bool_lock);   

void unlock_sem(int *bool_lock);
            
void init_sem(str_sem *my_struct_sem, int nb_count);

void wait_sem(str_sem *my_struct_sem);

void post_sem(str_sem *my_struct_sem);

#endif
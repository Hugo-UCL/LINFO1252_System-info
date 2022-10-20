#ifndef TESTANDSET_H
#define TESTANDSET_H

typedef  struct _str {
       int lock;
 }str_lock;

void lock(str_lock *my_struct_lock);
     
void unlock(str_lock *my_struct_lock);
            
void init(str_lock *my_struct_lock);

#endif
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <errno.h>
#include "testandset.h"

int nb_phil; 			  

void mange(int id) {
        // philosophe mange
        printf("philosophe %d mange\n",id);
}

void* philosophe(void* arg)
{
    int *id=(int *) arg;
    int left = *id;
    int right = (left + 1) % nb_phil;   
    for(int i=0; i<10; i++) {
        // philosophe pense
        if(left<right) {
            lock(&baguette[left]);
            lock(&baguette[right]);
        }
        else {
            lock(&baguette[right]);
            lock(&baguette[left]);
        }
        mange(*id);
        unlock(&baguette[left]);
        unlock(&baguette[right]);        
    }
    return (NULL);
}

int main (int argc, char *argv[])  {
    // tabargs[1] => nombre de philosophes
    // reception des arguments
    char *p;
    errno =0;
    long arg = strtol(argv[1], &p, 10);
    if (*p != '\0' || errno != 0){
        return 1;
        }
    int err;
    nb_phil = arg;
    
    // création du tableau contenant les mutexs et les philosophes
    pthread_t *phil = (pthread_t *)malloc(sizeof(pthread_t)*arg);
    str_lock *baguette = (str_lock *)malloc(sizeof(str_lock)*arg);
    
    
    // initialisation de nos propres mutexs 
    int tab[arg];
    for(int i=0; i<arg; i++){
        tab[i]=i; 
        init(&baguette[i]);  
    }
    
    //creation threads
    for(int i=0; i<arg; i++){
        err=pthread_create(&(phil[i]),NULL,&philosophe,&(tab[i]));
        if(err!=0)
            printf("error_threads_create%d\n",i);
          }
    //threads join     
    for(int i=0; i<arg; i++){
        err=pthread_join(phil[i],NULL);
        if(err!=0)
           printf("error_join%d\n",i);
           
         }  
    
    free(phil);
    free(baguette);
    printf("FIN");
    return(EXIT_SUCCESS);
    
}

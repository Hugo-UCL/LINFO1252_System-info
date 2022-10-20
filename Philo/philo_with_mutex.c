#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <errno.h>

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
    for(int i=0; i<100000; i++) {
        // philosophe pense
        if(left<right) {
            pthread_mutex_lock(&baguette[left]);
            pthread_mutex_lock(&baguette[right]);
        }
        else {
            pthread_mutex_lock(&baguette[right]);
            pthread_mutex_lock(&baguette[left]);
        }
        mange(*id);
        pthread_mutex_unlock(&baguette[left]);
        pthread_mutex_unlock(&baguette[right]);
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
    pthread_mutex_t *baguette = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t)*arg);
    
    //tab ref aux baguettes 
    int tab[arg];
    for(int i=0; i<arg; i++){
        tab[i]=i;   
    }
    
    // creation des mutexs
    for(int i=0; i<arg; i++){
        err=pthread_mutex_init(&baguette[i],NULL);

    }
    // creation des threads
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

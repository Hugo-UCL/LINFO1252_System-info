#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include "testandsetandset.h"

//global
str_lock mutex;  

void* cons(void* arg){
  int *loop=(int *) arg;
  for(int i=0; i <*loop; i++){
    lock(&mutex); 
    while (rand() > RAND_MAX / 10000);
    unlock(&mutex); 
  }
    return NULL;
}



int main (int argc, char *argv[]){
    // tabargs[1] => NB threads 
    // reception des arguments
    int tabargs[argc];
    char *p;
    errno =0;
    for(int i=1; i<argc; i++){
        long arg = strtol(argv[i], &p, 10);
        if (*p != '\0' || errno != 0){
            return 1;
            }
        tabargs[1]=(int)arg;
    }
    
    // initialisation de notre propre mutex 
    init(&mutex);

    // création du tabl contenant le nombre de threads choisis par l'utilisateur
    pthread_t *tabthread = (pthread_t *)malloc(sizeof(pthread_t)*tabargs[1]);

    // division des threads 
    int modulo = 64000 % tabargs[1];
    int nb_loop = (6400- modulo) / tabargs[1];
    int loop_supp = modulo + nb_loop;
    int err;
    
    // creation threads 
    for(int i=0; i<tabargs[1]; i++){
    	if(i ==0){
    	printf("pthread_create n°%d\n",i);
        err=pthread_create(&(tabthread[i]),NULL,&cons,&loop_supp);
    	}
    	else{
        printf("pthread_create n°%d\n",i);
        err=pthread_create(&(tabthread[i]),NULL,&cons,&nb_loop);
        }
        if(err!=0){
            printf("%derror_product_pthread_create",i);
            return 1;
            }
     }
     
     // join threads 
     for(int i=0; i<tabargs[1]; i++){
         err=pthread_join(tabthread[i],NULL);
         if(err!=0){
            printf(" thread n°%d : error_pthread_not_join\n",i);
            }
          else{
          printf("FIN trhead");
          }
     }
     
     free(tabthread);
     printf("FIN");
     return(EXIT_SUCCESS);
}



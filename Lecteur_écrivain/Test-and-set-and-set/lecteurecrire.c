#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <errno.h>
#include "semaphore.h"
#include "testandsetandset.h"

int writecount=0;
int readcount=0;

//déclaration de la sémaphore et du mutex et des threads
str_lock z;
str_lock mutex_writecount;
str_lock mutex_readcount;
        
str_sem rsem;   
str_sem wsem;


void* reader(){
 for(int i=0; i<2560; i++) {
   lock(&z);        
   wait_sem(&rsem);                  
   lock(&mutex_readcount);    
     readcount++;
     if (readcount==1)      
     {  
     printf("thread read attends\n");
     wait_sem(&wsem);         
     }
   unlock(&mutex_readcount);    
   printf("thread read lit la base de donneé\n");
   //read_database();           
   post_sem(&rsem);             
   unlock(&z);      
     // section critique        
     readcount--;
     if(readcount==0){           
       post_sem(&wsem);
     }
 }
 return (NULL);
}

void* writer(){
 for(int i=0; i<640; i++) { 
   lock(&mutex_writecount);
   writecount = writecount+1;
   if (writecount==1){
   //prepare_data();
   printf("thread écrivain attends\n");
   wait_sem(&rsem);
   }
   printf("thread écrivain écris\n");
   //sem_post(&rsem); //     supp? //new???
   unlock(&mutex_writecount);
   writecount--;
     if(writecount==0){
       post_sem(&rsem);
     }   
   }
   return (NULL);
 }



int main (int argc, char *argv[])  { 
    // tabargs[1] => écrivains
    // tabargs[2] => lecteurs
    int tabargs[argc];
    char *p;
    errno =0;
    for(int i=1; i<argc; i++){
        long arg = strtol(argv[i], &p, 10);
        if (*p != '\0' || errno != 0){
            return 1;
            }
        tabargs[i]=(int)arg;
    }
    int nb_threads = tabargs[1]+ tabargs[2];
    int err;

    // création du tabl contenant le nombre de threads
    pthread_t *threads = (pthread_t *)malloc(sizeof(pthread_t)*nb_threads);
    
    // creation mutex
    init(&mutex_readcount);
    init(&z);
    init(&mutex_writecount);

    // création sémaphore 
    init_sem(&rsem,1);
    init_sem(&wsem,1);

    //creation threads écrivains 
    for(int i=0; i<tabargs[1]; i++){
        printf("ecrivain_pthread_create n°%d\n",i);
        err=pthread_create(&(threads[i]),NULL,&writer,NULL);
        if(err!=0){
            printf("%derror_ecrivain_pthread_create",tabargs[1]);
            return 1;
            }
     }
      
    //creation threads lecteurs 
    for(int i=0; i<tabargs[2]; i++){
        printf("lecteur_pthread_create n°%d\n",i);
        err=pthread_create(&(threads[i+tabargs[1]]),NULL,&reader,NULL);
        if(err!=0){
            printf("%derror_lecteur_pthread_create",tabargs[1]);
            return 1;
            }
     }
    //threads join     
    for(int i=0; i<nb_threads; i++){
        err=pthread_join(threads[i],NULL);
        if(err!=0){
            printf("%derror_pthread_not_join\n",nb_threads);
            }
     }
  return(EXIT_SUCCESS);
}

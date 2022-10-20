#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <errno.h>
#include <semaphore.h>


int writecount=0;
int readcount=0;

//déclaration de la sémaphore et du mutex et des threads
pthread_mutex_t z;              //new
pthread_mutex_t mutex_writecount; //new
pthread_mutex_t mutex_readcount; //new

pthread_t *threads;		//decla struct qui vont acceuillir les threads representera soit un lecteur ou un écrvain
sem_t rsem;			//decla variable de type sémaphore
sem_t wsem;


void* reader()
{
 for(int i=0; i<2560; i++) {//2560
   pthread_mutex_lock(&z); 		
   sem_wait(&rsem);                    //new
   pthread_mutex_lock(&mutex_readcount);          //bloque l'entrée pour un seul thread (readers)
     // section critique
     readcount++;
     if (readcount==1) 			// si il y a déjà des threads readers alors saute la condition
     { 	
     printf("thread read attends\n");
       sem_wait(&wsem);         
     }
   pthread_mutex_unlock(&mutex_readcount);	// le threads readers unlock pour qu'un prochain threads passe 
   printf("thread read lit la base de donneé\n");
   //read_database();			
   sem_post(&rsem); 			
   pthread_mutex_unlock(&z);		
     // section critique		
     readcount--;
     if(readcount==0)			 
     { 
       sem_post(&wsem);
     }

 }
  return (NULL);
}

void* writer()
{
 for(int i=0; i<640; i++) { 
   pthread_mutex_lock(&mutex_writecount);
   //section critique
   writecount = writecount+1;
   if (writecount==1){
   // premier writer arrive
   //prepare_data();
   printf("thread écrivain attends\n");
   sem_wait(&rsem);
   }
   printf("thread écrivain écris\n");
   //sem_post(&rsem); //	 supp? //new???
   pthread_mutex_unlock(&mutex_writecount);
   writecount--;
     if(writecount==0){	
       sem_post(&rsem);
     }
   
   }
    return (NULL);
 }



int main (int argc, char *argv[])  {
    // récupérer la variable en long puis conversion int 
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
    for(int i=1; i<argc; i++){
        printf("%d",tabargs[i]);
        printf("%d\n",nb_threads);
    }
    // fin de récupération des arguments 
    int err;

    // création du tabl contenant le nombre de threads
    threads = (pthread_t *)malloc(sizeof(pthread_t)*nb_threads);
    
    //creation mutex
    err=pthread_mutex_init(&mutex_readcount,NULL);
    err=pthread_mutex_init(&z,NULL);
    err=pthread_mutex_init(&mutex_writecount,NULL);

     //création sémaphore 
     sem_init(&rsem, 0, 1);	//Initialisation de la sémaphore à 1
     sem_init(&wsem, 0, 1);


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

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <semaphore.h>
#include <limits.h>

int global_prod=0;
int global_cons=0;
long max;
int tabbuffer[8];
//déclaration des 2 sémaphore et du mutex 
pthread_mutex_t mutex_global;   
sem_t empty; 			
sem_t full;			


int produce(){
    int randomNum;
    return randomNum = rand() % max + (INT_MIN);
}

// Producteur
void* producer( void* arg ){
  int item;
  int *loop=(int *) arg;
  for(int i=0; i <*loop; i++){
    while(rand() > RAND_MAX/10000){
    }
    item=produce();
    sem_wait(&empty); // attente qu’une place se libère 
    pthread_mutex_lock(&mutex_global);
    // section critique
    tabbuffer[global_prod] = item;
    printf("producteur n°x travail met item = %d dans l'emplacement n°%d\n",item,global_prod);
    global_prod = (global_prod +1) % 8;
    // section critique 
    pthread_mutex_unlock(&mutex_global);
    sem_post(&full);
  }
    return (NULL);
}

// Consommateur
void* consumer( void* arg ) {
 int item;
 int *loop=(int *) arg; // a supp
 for(int i=0; i <*loop; i++){
    while(rand() > RAND_MAX/10000){
    }
   sem_wait(&full); 
   pthread_mutex_lock(&mutex_global);
   // section critique
   item=tabbuffer[global_cons];
   printf("consommateur n°x travail enleve item = %d dans l'emplacement n°%d\n",item,global_cons);
   global_cons = (global_cons +1) % 8;
   // section critique
   pthread_mutex_unlock(&mutex_global);
   sem_post(&empty); 
 }
   return (NULL);
}


int main (int argc, char *argv[])  {
    // réception des arguments
    // tabargs[1] => producteur
    // tabargs[2] => consomateur
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
    // initialisation des variables pour MIN&MAX
    long c = INT_MAX;
    max = c + c +1; 
    int err;
    
    // création du tabl contenant le nombre de travailleurs
    pthread_t *produ = (pthread_t *)malloc(sizeof(pthread_t)*tabargs[1]);
    pthread_t *consu = (pthread_t *)malloc(sizeof(pthread_t)*tabargs[2]);
    
    // creation mutex
    err=pthread_mutex_init(&mutex_global,NULL);

    // création sémaphores 
    sem_init(&empty, 0 , 8);	//Initialisation de la sémaphore à N
    sem_init(&full, 0 , 0);    //Initialisation de la sémaphore à 0
     

    //divise le 1024
    int modulo_prod = 1024 % tabargs[1];
    int modulo_cons = 1024 % tabargs[2];
    int nb_loop_prod = (1024- modulo_prod) / tabargs[1];
    int nb_loop_cons = (1024- modulo_prod) / tabargs[2];
    int loop_supp_prod = modulo_prod + nb_loop_prod;
    int loop_supp_cons = modulo_cons + nb_loop_cons;

    
    //creation threads producteur 
    for(int i=0; i<tabargs[1]; i++){
    	if(i ==0){
        err=pthread_create(&(produ[i]),NULL,&producer,&loop_supp_prod);
    	}
    	else{
        err=pthread_create(&(produ[i]),NULL,&producer,&nb_loop_prod);
        }
        if(err!=0){
            printf("%derror_product_pthread_create",i);
            return 1;
            }
     }
      
    // creation threads consommateur 
    for(int i=0; i<tabargs[2]; i++){
        if(i ==0){
        err=pthread_create(&(consu[i]),NULL,&consumer,&loop_supp_cons);
    	}
    	else{
        err=pthread_create(&(consu[i]),NULL,&consumer,&nb_loop_cons);
        }
        if(err!=0){
            printf("%derror_cons_pthread_create",i);
            return 1;
            }
     }
     
     // join threads producteur
     for(int i=0; i<tabargs[1]; i++){
         err=pthread_join(produ[i],NULL);
         if(err!=0){
            printf(" thread n°%d : error_pthread_not_join\n",i);
            }
     }
     
     // join threads cosomateur
     for(int i=0; i<tabargs[2]; i++){
         err=pthread_join(consu[i],NULL);
         if(err!=0){
            printf(" thread n°%d : error_pthread_not_join\n",i);
            }
     }
     
    
    //destruction mutex
    err=pthread_mutex_destroy(&mutex_global);
    if(err!=0){
        printf("%derror_cons_pthread_create",err);
        }
        
    free(produ);
    free(consu);
    printf("FIN");
    return(EXIT_SUCCESS);
  
}


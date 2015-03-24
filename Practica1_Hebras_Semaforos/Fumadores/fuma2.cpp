#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <iostream>
using namespace std;
z
sem_t s_tabaco,s_papel,s_cerillas,s_estanquero;


void fumar(){
  const unsigned miliseg= 100U + (rand()%1900U);
  usleep(1000U*miliseg);
  }
  int despachar(){
    return rand()%3;
  }
  
  void * estanquero(void *){
    while (true){
      sem_wait(&s_estanquero);
      int saco=despachar();
      if (saco==0){
	cout<<"Saco tabaco"<<endl<<flush;
	sem_post(& s_tabaco);	  
      }
      else if (saco==1){
	cout<<"Saco papel"<<endl<<flush;
	sem_post(& s_papel);		
      }
      else{
	cout<<"Saco cerilla"<<endl<<flush;
	sem_post(& s_cerillas);		
      }
	  
    }
    
  }
  void * fumador(void * arg_ptr){
    long unsigned arg=(long unsigned ) arg_ptr ;
    
    if (arg==0){
      while (true){     
	sem_wait(&s_tabaco);
	  cout <<"Soy A, Ingrediente consumido: tabaco"<<endl;
	  sem_post(&s_estanquero);
	  fumar();
	  cout <<"Soy A y he fumado... "<<endl<<endl;
    }  
      }
      else if (arg==1){
      while (true){    
	sem_wait(&s_papel);
	  cout <<"Soy B, Ingrediente consumido: papel"<<endl;
	sem_post(&s_estanquero);
	fumar();
	cout <<"Soy B ya he fumado... "<<endl<<endl;
     }
      }
      else{
	while (true){     
	  sem_wait(&s_cerillas);
	    cout <<" Soy C, Ingrediente consumido: cerillas"<<endl;
	  sem_post(&s_estanquero);
	  fumar();
	  cout <<"Soy C ya he fumado... "<<endl<<endl;
	}
      }
    

  }
  
  int main(){
    srand(time(NULL));
    pthread_t f[3],e ;

    sem_init(&s_tabaco,0,0);
    sem_init(&s_papel,0,0);
    sem_init(&s_cerillas,0,0);
    sem_init(&s_estanquero,0,1);
    
    pthread_create(&e,NULL,estanquero,NULL);
    for(unsigned i=0;i<3;i++){
      void * arg_ptr=(void *)i;
      pthread_create(&(f[i]),NULL,fumador,arg_ptr);
    }

    for(unsigned i=0;i<3;i++){
      void * arg_ptr=(void *)i;
      pthread_join(f[i],NULL);
    }
    pthread_join(e,NULL);
    cout << "hebras terminadas." << endl<<"FIN" <<endl;
   
    sem_destroy(&s_estanquero);
    sem_destroy(&s_cerillas);  
    sem_destroy(&s_papel);
    sem_destroy(&s_tabaco);  
  }
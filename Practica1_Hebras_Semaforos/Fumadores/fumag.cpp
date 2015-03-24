/******************************************************************************************************
 * 
 * Version Fumadores con multiples hebras y un estanquero
 * Finaliza las hebras cuando los 15 primeros fumadores fumen no los primeros en terminar de fumar.
 * 
 * g++ ./fumag.cpp -o fumag -lpthread
 * ./fumag numfumadoresSinTabaco numfumadoresSinPapel numfumadoresSinCerillas
 *//////////////////////////////////////////////////////////////////////////////////////////////////////

#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <iostream>
using namespace std;
sem_t s_tabaco,s_papel,s_cerillas,s_estanquero,s_fin;
int contador=15;

void fumar(){
  contador --;
  const unsigned miliseg= 100U + (rand()%1900U);
  usleep(1000U*miliseg);
  }
  int despachar(){
    return rand()%3;
  }
  
  void * estanquero(void *){
    while (true){
      sem_wait(&s_estanquero);
      	  sem_wait(&s_fin);
	    if (contador<=0){  
	      sem_post(&s_cerillas);sem_post(&s_tabaco);sem_post(&s_papel);sem_post(&s_fin);	//ABRO TODOS LOS SEMAFOROS PARA QUE TODAS LAS HEBRAS FINALICEN
	      cout<<"Muere el estanquero"<<endl;
	      pthread_exit(NULL);
	    }
	  sem_post(&s_fin);  
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
  void * fumador0(void * arg_ptr){
    long unsigned arg=(long unsigned ) arg_ptr ;
      while (true){     

	sem_wait(&s_tabaco);
	  cout <<"Soy tipo 0 hebra "<<((unsigned long) arg_ptr)<<" Ingrediente consumido: tabaco"<<endl;
	  sem_post(&s_estanquero);
	  sem_wait(&s_fin);
	    if (contador<=0){  
	      sem_post(&s_cerillas);sem_post(&s_tabaco);sem_post(&s_papel);sem_post(&s_fin);	//ABRO TODOS LOS SEMAFOROS PARA QUE TODAS LAS HEBRAS FINALICEN
	      cout<<"muere "<<((unsigned long) arg_ptr)<<endl;
	      pthread_exit(NULL);
	    }
	  sem_post(&s_fin);
	  fumar();
	  cout <<"Soy tipo 0 hebra"<<arg<<" y he fumado... "<<endl<<endl;  
      }
    
  }
  void * fumador1(void * arg_ptr){
      while (true){    
	sem_wait(&s_papel);
	  cout <<"Soy tipo 1 hebra "<<((unsigned long) arg_ptr)<<" Ingrediente consumido: papel"<<endl;
	sem_post(&s_estanquero);
	sem_wait(&s_fin);
	  if (contador<=0){  
	    sem_post(&s_cerillas);sem_post(&s_tabaco);sem_post(&s_papel);sem_post(&s_fin);	//ABRO TODOS LOS SEMAFOROS PARA QUE TODAS LAS HEBRAS FINALICEN
	    cout<<"muere "<<((unsigned long) arg_ptr)<<endl;
	    pthread_exit(NULL);
	  }
	sem_post(&s_fin);
	fumar();
	  cout <<"Soy tipo 1 hebra"<<((unsigned long) arg_ptr)<<" y he fumado... "<<endl<<endl;  
     }
      }
  void * fumador2(void * arg_ptr){
	while (true){     
	  sem_wait(&s_cerillas);
	    cout <<" Soy tipo 2 hebra: "<<((unsigned long) arg_ptr)<<" Ingrediente consumido: cerillas"<<endl;
	  sem_post(&s_estanquero);
	  sem_wait(&s_fin);
	    if (contador<=0){  
	      sem_post(&s_cerillas);sem_post(&s_tabaco);sem_post(&s_papel);sem_post(&s_fin);	//ABRO TODOS LOS SEMAFOROS PARA QUE TODAS LAS HEBRAS FINALICEN
	      cout<<"muere "<<((unsigned long) arg_ptr)<<endl;
	      pthread_exit(NULL);
	    }
	  sem_post(&s_fin);
	  fumar();
	  cout <<"Soy tipo 2 hebra"<<((unsigned long) arg_ptr)<<" y he fumado... "<<endl<<endl;  
	}
      }
    

  
  
int main(int argc, char *argv[]){
     if(argc!=4){
      cout<<"numero de argumentos incorrecto agregar el numero de hebras fumadoras sin tabaco,"<<
      " el numero de hebrasfumadoras sin papel y numero de hebras fumadoras sin cerillas "<<
      endl<<"./name numfumta numfumpa numfumce "<<endl;
      exit(1);
     }
    srand(time(NULL));
    int a=atoi(argv[1]),b=atoi(argv[1]),c=atoi(argv[1]);
    int t=0;
    pthread_t f[a+b+c],e ;

    
    sem_init(&s_tabaco,0,0);
    sem_init(&s_papel,0,0);
    sem_init(&s_cerillas,0,0);
    sem_init(&s_estanquero,0,1);
    sem_init(&s_fin,0,1);
    
    cout <<"Vamos a crear hebras fumadoras tipo 0,1,2 segun le falten tabaco,papel o cerillas."<<endl;
      for(unsigned j=0;j<atoi(argv[1]);j++){
	void * arg_ptr=(void *)t;
	pthread_create(&(f[t]),NULL,fumador0,arg_ptr);
	cout <<"hebra "<<t<<" tipo 0 creada"<<endl;
		t++;
      }
          for(unsigned j=0;j<atoi(argv[1]);j++){
	void * arg_ptr=(void *)t;
	pthread_create(&(f[t]),NULL,fumador1,arg_ptr);
	cout <<"hebra "<<t<<" tipo 1 creada"<<endl;
		t++;
      }
            for(unsigned j=0;j<atoi(argv[1]);j++){
	void * arg_ptr=(void *)t;
	pthread_create(&(f[t]),NULL,fumador2,arg_ptr);
	cout <<"hebra "<<t<<" tipo 2 creada"<<endl;
		t++;
      }
        pthread_create(&e,NULL,estanquero,NULL);			//CONVENIENTE CREAR EL ESTANQUERO DESPUES DE LOS FUMADORES
									//PARA QUE NO SE EMPIEZE A FUMAR HASTA CREAR A TODOS

    for(unsigned i=0;i<t;i++){
      void * arg_ptr=(void *)i;
      pthread_join(f[i],NULL);
    }
    pthread_join(e,NULL);
    cout << "hebras terminadas." << endl<<"FIN" <<endl;
   
    sem_destroy(&s_estanquero);
    sem_destroy(&s_cerillas);  
    sem_destroy(&s_papel);
    sem_destroy(&s_tabaco);
    sem_destroy(&s_fin);
  }
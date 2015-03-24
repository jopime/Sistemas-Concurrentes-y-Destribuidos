/******************************************************************************************************
 * 
 * Version Productor Consumidor concurrente con multiples hebras accediendo a un LIFO
 * 
//  * g++ ./pcfifog.cpp -o pclifog -lpthread
//  * ./pcfifog numpro numcons
 *//////////////////////////////////////////////////////////////////////////////////////////////////////


#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <pthread.h>
#include <semaphore.h>
using namespace std;
sem_t puede_escribir,puede_leer,mutex;
const unsigned num_items = 30;
const unsigned tam_vec = 10;
unsigned buffer[tam_vec];
unsigned primera_libre=0;
unsigned primera_ocupada=0;
bool fin=false;

int producir_dato(){
  static int contador=1;
  return contador ++;
}
void consumir_dato(int dato){
  sem_wait(&mutex);
    cout <<"dato consumido: "<<dato<<endl;
  sem_post(&mutex);
}

void * productor(void * num_h){
  for (unsigned i=0; i<num_items ; i++){

    int dato=producir_dato();
    sem_wait(&puede_escribir);     
      if (dato>num_items){	  
	sem_wait(&mutex);
	  cout<<"muera HebraProductora::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: "<<((unsigned long) num_h)<<endl;
	sem_post(&mutex);
	sem_post(&puede_escribir);		//abro semaforo de escritura antes de morir
	pthread_exit(NULL);
    }      
      buffer[primera_libre]=dato;
      primera_libre=(primera_libre+1)%tam_vec;
      sem_wait(&mutex);
	cout<<"Tras esta escritura echa por "<<((unsigned long) num_h)<<" lectura ocupadas desde: "<<primera_ocupada<<" a "<<primera_libre<<endl;
      sem_post(&mutex);
    sem_post(&puede_leer);
    }
    return NULL;
}

void * consumidor(void * num_h){
int dato;
  for (unsigned i=0; i<num_items; i++){
    sem_wait(&puede_leer);
      dato=buffer[primera_ocupada];
      if(fin){
	  sem_wait(&mutex);
	    cout<<"muera HebraConsumidora::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: "<<((unsigned long) num_h)<<endl;
	  sem_post(&mutex);
	  sem_post(&puede_leer);		//abro semaforo de escritura antes de morir
	  pthread_exit(NULL);
	 }	 
      primera_ocupada=(primera_ocupada+1)%tam_vec;
      consumir_dato(dato);
      sem_wait(&mutex);
	cout<<"Tras esta lectura ocupadas desde: "<<primera_ocupada<<" a "<<primera_libre<<endl;
      sem_post(&mutex);
      if(dato==num_items){
	fin=true;
	sem_post(&puede_leer);
      }
      sem_post(&puede_escribir);
  }
    return NULL;
}

int main(int argc, char *argv[]){
   if(argc!=3){
      cout<<"numero de argumentos incorrecto agregar el numero de hebras productoras y el numero de hebras consumidoras "<<endl<<"(./name numpro numcons)"<<endl;
      exit(1);
     }
   
int np=atoi(argv[1]);
int nc=atoi(argv[2]);
  for(int i=0;i<tam_vec;i++){
    buffer[i]=100;   				//Para asegurarme de que no lee una posicion no escrita
						// por el productor;
  }
   pthread_t consumidora[nc], productora[np];
   sem_init(&puede_leer,0,0);
   sem_init(&puede_escribir,0,tam_vec);  //Verion con el semaforo de escritura tan grande como el vector
   sem_init(&mutex,0,1);

for(unsigned i = 0 ; i < nc ; i++ ){
void *arg_ptr = (void*) i ;//convertir entero a puntero
pthread_create( &(consumidora[i]), NULL,consumidor, arg_ptr );    
}
for(unsigned i = 0 ; i < np ; i++ ){  
void *arg_ptr = (void*) i ;//convertir entero a puntero
pthread_create( &(productora[i]), NULL,productor, arg_ptr );   
}


for(unsigned i = 0 ; i < nc ; i++ ){
pthread_join(consumidora[i], NULL);		
}
for(unsigned i = 0 ; i < np ; i++ ){
pthread_join(productora[i], NULL);		
}
cout << "hebras terminadas." << endl<<"FIN" <<endl;
   

   sem_destroy(&puede_escribir);
   sem_destroy(&puede_leer);
   sem_destroy(&mutex);

   return 0;
}
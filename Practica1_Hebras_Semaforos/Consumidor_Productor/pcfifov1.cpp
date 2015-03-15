/*
*EN ESTA VERSION USO 2 SEMAFOROS SIMPLES
* 
* 
*/
#include <iostream>
#include <pthread.h>
#include <semaphore.h>
using namespace std;

sem_t puede_escribir,puede_leer;
const unsigned num_items = 30;
const unsigned tam_vec = 10;
unsigned buffer[tam_vec];
unsigned primera_libre=0;
unsigned primera_ocupada=0;


int producir_dato(){
  static int contador=1;
  return contador ++;
}
void consumir_dato(int dato){
  cout <<"dato consumido: "<<dato<<endl;
}

void * productor(void *){
  for (unsigned i=0; i<num_items ; i++){

    int dato=producir_dato();
    sem_wait(&puede_escribir);
      buffer[primera_libre]=dato;
      primera_libre=(primera_libre+1)%tam_vec;
      cout<<"Tras esta escritura lectura ocupadas desde: "<<primera_ocupada<<" a "<<primera_libre<<endl;  
    sem_post(&puede_leer);
    }
    return NULL;
}

void * consumidor(void *){
  for (unsigned i=0; i<num_items; i++){
    sem_wait(&puede_leer);
      int dato=buffer[primera_ocupada];
      primera_ocupada=(primera_ocupada+1)%tam_vec;
      consumir_dato(dato);
      cout<<"Tras esta lectura lectura ocupadas desde: "<<primera_ocupada<<" a "<<primera_libre<<endl;
    sem_post(&puede_escribir);
  }
    return NULL;
}

int main()
{   
  for(int i=0;i<tam_vec;i++){
    buffer[i]=100;   				//Para asegurarme de que no lee una posicion no escrita
						// por el productor;
  }
   pthread_t consumidora, productora ;
   sem_init(&puede_leer,0,0);
   sem_init(&puede_escribir,0,1);  //Verion con el semaforo simple
  
   pthread_create(&consumidora,NULL,consumidor,NULL);
   pthread_create(&productora,NULL,productor,NULL);

   pthread_join(consumidora,NULL);
   pthread_join(productora,NULL);
   cout << "hebras terminadas." << endl<<"FIN" <<endl;
   
   sem_destroy(&puede_escribir);
   sem_destroy(&puede_leer);
   
   return 0;
}
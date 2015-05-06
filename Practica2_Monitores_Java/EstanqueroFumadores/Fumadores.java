import monitor.* ;
import java.util.Random ;

class aux
{
  static Random genAlea = new Random() ;
  static void dormir_max( int milisecsMax )
  { try
    { Thread.sleep( genAlea.nextInt( milisecsMax ) ) ;
    } 
    catch( InterruptedException e )
    { System.err.println("sleep interumpido en 'aux.dormir_max()'");
    }
  }
}

class Estanco extends AbstractMonitor{ 
	private Condition colaE=makeCondition();
	private Condition colaF0=makeCondition();
	private Condition colaF1=makeCondition();
	private Condition colaF2=makeCondition();

	private int ingredienteActual=-1;
// invocado por cada fumador, indicando su ingrediente o numero
	public void obtenerIngrediente( int miIngrediente, Thread thr){
		enter();
		if (ingredienteActual!=miIngrediente){
			switch(miIngrediente) {
				case 0: colaF0.await(); break;
				case 1: colaF1.await(); break;
				case 2: colaF2.await();	break;	
			}		
		}
			System.out.println(thr.getName()+", Comienza a fumar " + miIngrediente+miIngrediente);
			ingredienteActual=-1;
			colaE.signal();
		leave();
	}
// invocado por el estanquero, indicando el ingrediente que pone
	public void ponerIngrediente( int ingrediente , Thread thr){
		enter();
			ingredienteActual=ingrediente;
			System.out.println(thr.getName()+", SACO " + ingrediente);
			switch(ingredienteActual) {
				case 0: colaF0.signal(); break;
				case 1: colaF1.signal(); break;
				case 2: colaF2.signal();	break;	
			}
			
		leave();
	}
// invocado por el estanquero
	public void esperarRecogidaIngrediente(Thread thr){
		enter();
			if(ingredienteActual!=-1) {
				System.out.println(thr.getName()+", SIN INGREDIENTES ");
				colaE.await();
			}
		leave();
	}
}

//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**

class Fumador implements Runnable{
	private Estanco miestanco;
	int miIngrediente;
	public Thread thr ;

	public Fumador( Estanco estanco ,int p_miIngrediente ){ 
		miIngrediente=p_miIngrediente;
		miestanco=estanco;
		thr   = new Thread(this,"Fumador "+p_miIngrediente);
	}
	public void run(){
		while ( true ){
			miestanco.obtenerIngrediente( miIngrediente ,thr);
			aux.dormir_max( 2000 );
			System.out.println(thr.getName()+", TERMINA de fumar " + miIngrediente);
		}
	}
}

//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**
class Estanquero implements Runnable{
	 public Thread thr ;
	private Estanco miestanco;

	public Estanquero( Estanco estanco ){ 
		miestanco=estanco;
		thr   = new Thread(this,"Estanquero ");
	}
	
	public void run(){
		int ingrediente ;
		while (true){
			ingrediente = (int) (Math.random () * 3.0); // 0,1 o 2
			miestanco.ponerIngrediente( ingrediente,thr );
			miestanco.esperarRecogidaIngrediente(thr) ;
		}
	}
}


//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**

class EjemploFumador{
	public static void main( String[] args ) {


    // leer parametros, crear vectores y buffer intermedio
		Fumador[] fumadores = new Fumador[3] ;
		Estanco estanco = new Estanco();
    		Estanquero  estanquero = new Estanquero(estanco) ;

	  // crear hebras

		for (int i=0;i<3;i++)
		    fumadores[i] = new Fumador(estanco,i);

	  // poner en marcha las hebras
		estanquero.thr.start();
		for (int i=0;i<3;i++)
			fumadores[i].thr.start();
	}
}



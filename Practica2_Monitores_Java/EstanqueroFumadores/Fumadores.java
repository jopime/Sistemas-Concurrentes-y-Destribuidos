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
	private int contador=0;
	private boolean sigo=true;

// invocado por cada fumador, indicando su ingrediente o numero
	public boolean obtenerIngrediente( int miIngrediente, Thread thr){
		enter();
		if (ingredienteActual!=miIngrediente){
			switch(miIngrediente) {
				case 0: colaF0.await(); break;
				case 1: colaF1.await(); break;
				case 2: colaF2.await();	break;	
			}		
		}
			if(sigo){
			System.out.println(thr.getName()+", Comienza a fumar " + miIngrediente+miIngrediente);
			ingredienteActual=-1;
			colaE.signal();
			}
		leave();
		return sigo;
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
	public boolean esperarRecogidaIngrediente(Thread thr){
		enter();
			if(ingredienteActual!=-1) {
				System.out.println(thr.getName()+", SIN INGREDIENTES ");
				colaE.await();
			}
			contador++;
			System.out.println(thr.getName()+", vez "+contador);
			if(contador>14) sigo=false;
		leave();
		return sigo;
	}
	public void acabarConFumadores(){
		enter();
			while(!colaF0.isEmpty()) colaF0.signal();
			System.out.println(", FINALIZANDOOO F0 "+colaF0.isEmpty());
			while(!colaF1.isEmpty()) colaF1.signal();
			System.out.println(", FINALIZANDOOO F0 "+colaF1.isEmpty());
			while(!colaF2.isEmpty()) colaF2.signal();
			System.out.println(", FINALIZANDOOO F0 "+colaF2.isEmpty());
		leave();
	}
	public int contador(){return contador;}
}

//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**

class Fumador implements Runnable{
	private Estanco miestanco;
	int miIngrediente;
	public Thread thr ;
	boolean cont=true;

	public Fumador( Estanco estanco ,int p_miIngrediente ){ 
		miIngrediente=p_miIngrediente%3;
		miestanco=estanco;
		thr   = new Thread(this,"Fumador "+p_miIngrediente+"tipo "+miIngrediente);
	}
	public void run(){
		while ( cont){
			cont=miestanco.obtenerIngrediente( miIngrediente ,thr);
			aux.dormir_max( 2000 );
			System.out.println(thr.getName()+", TERMINA de fumar " + miIngrediente+"cont? "+cont);
			if(miestanco.contador()>14) cont=false;
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
		boolean fin=true;
		while (fin){
			ingrediente = (int) (Math.random () * 3.0); // 0,1 o 2
			miestanco.ponerIngrediente( ingrediente,thr );
			fin=	miestanco.esperarRecogidaIngrediente(thr) ;
			if(!fin)
				miestanco.acabarConFumadores();
			}
		}
	}



//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**

class EjemploFumador{
	public static void main( String[] args ) {
		int tam=25;

    // leer parametros, crear vectores y buffer intermedio
		Fumador[] fumadores = new Fumador[tam] ;
		Estanco estanco = new Estanco();
    		Estanquero  estanquero = new Estanquero(estanco) ;

	  // crear hebras

		for (int i=0;i<tam;i++)
		    fumadores[i] = new Fumador(estanco,i);

	  // poner en marcha las hebras
		estanquero.thr.start();
		for (int i=0;i<tam;i++)
			fumadores[i].thr.start();
	}
}



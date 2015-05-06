import monitor.* ;
import java.util.Random ;
//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**
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

//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**

class Barberia extends AbstractMonitor{
// invcado por los clientes para cortarse el pelo
	private Condition salaEspera=makeCondition();
	private Condition salaSueño=makeCondition();
	private Condition sillaBarberia=makeCondition();

	public void cortarPelo (Thread thr){
	enter();
		if(!(salaSueño.isEmpty()) && sillaBarberia.isEmpty()){															//NO HAY NADIE PELANDOSE , DESPIERTO AL BARBERO
						System.out.println("barberia vacia "+thr.getName());			
			salaSueño.signal();
			System.out.println(thr.getName()+", Despierta al barbero ");
		}
		else{
			System.out.println(thr.getName()+", Entra a sala de espera ");
			salaEspera.await();															//HAY ALGUIEN PELANDOSE , ME VOY A LA SALA DE ESPERA

		}
			System.out.println(thr.getName()+", Se sienta en lea silla para pelarse ");
			sillaBarberia.await();														// SE SIENTA EN LA SILLA	
 			System.out.println(thr.getName()+", Acaba de ser pelado ");															
	leave();	
	}
// invocado por el barbero para esperar (si procede) a un nuevo cliente y sentarlo para el corte
	public void siguienteCliente (){
	enter();
		if(salaEspera.isEmpty()){
			System.out.println("Barbero se duerme");
			salaSueño.await();
		}
		else{
			System.out.println("Barbero Intenta llama a cliente ");
			salaEspera.signal();
		}
	leave();	
	}	
// invocado por el barbero para indicar que ha terminado de cortar el pelo
	public void finCliente (){ 
	enter();
		System.out.println("Fin Pelado ");
		sillaBarberia.signal();
	leave();
	}
}

//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**

class Cliente implements Runnable{
	public Thread thr ;
	private Barberia mibarberia;
	int num=-1;

	public Cliente( Barberia barberia,int nCliente ){ 
		num=nCliente;
		mibarberia=barberia;
		thr   = new Thread(this,"Cliente "+num);
	}
	public void run (){
		while (true) {
			mibarberia.cortarPelo (thr); // el cliente espera (si procede) y se corta el pelo
			System.out.println(thr.getName()+", Sale de la peluqueria ");
			aux.dormir_max( 2000 ); // el cliente est´a fuera de la barberia un tiempo
		}
	}
}

//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**
class Barbero implements Runnable{
	public Thread thr ;
	private Barberia mibarberia;

	public Barbero( Barberia barberia ){ 
		mibarberia=barberia;
		thr   = new Thread(this,"Barbero ");
	}
public void run (){
	while (true) {
		mibarberia.siguienteCliente ();
		aux.dormir_max( 2500 ); // el barbero est´a cortando el pelo
		mibarberia.finCliente ();
		}
	}
}

//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**

class EjemploBarbero{
	public static void main( String[] args ) {


    // leer parametros, crear vectores y buffer intermedio
		Cliente[] cl = new Cliente[3] ;
		Barberia barberia = new Barberia();
    		Barbero  barbero = new Barbero(barberia) ;

	  // crear hebras

		for (int i=0;i<3;i++)
		    cl[i] = new Cliente(barberia,i);

	  // poner en marcha las hebras
		barbero.thr.start();
		for (int i=0;i<3;i++)
			cl[i].thr.start();
	}
}

import monitor.* ;

class MonitorPC extends AbstractMonitor {
 	private int numSlots = 0, cont = 0;   
	private double[] buffer = null;
	
	private Condition colaC=makeCondition();
	private Condition colaP=makeCondition();

	public MonitorPC( int p_numSlots ) {
		numSlots = p_numSlots ; 
	    	buffer = new double[numSlots] ;
	  	}
	  public void depositar( double valor )  {
		enter(); 
			if( cont == numSlots ) colaP.await();              
			buffer[cont] = valor; 
			cont++;
      		colaC.signal();
		leave();  
	  	}

	  public double extraer(){
		double valor;
		enter();  
		if( cont == 0 ) colaC.await() ;
			cont--; 
			valor = buffer[cont] ;
			colaP.signal();
		leave();  		
			return valor;

	  	}
	}

class Productor implements Runnable 
{ 
	private MonitorPC bb ;
	private int veces; 
  	int numP ;
  	public Thread thr ;
  
	public Productor( MonitorPC pbb, int pveces, int pnumP ) {
		bb = pbb;
    		veces = pveces;
    		numP = pnumP;
    		thr = new Thread(this,"productor "+numP);
  	}
	public void run() {
		double item = 100*numP ;
		for( int i=0 ; i<veces ; i++ ){
			System.out.println(thr.getName()+", produciendo " + item);
        		bb.depositar( item++ );
      	}
	}
}
class Consumidor implements Runnable 
{ private MonitorPC bb ;
  int veces; 
  int numC ;
  Thread thr ;
  public Consumidor( MonitorPC pbb, int pveces, int pnumC ) 
  { bb    = pbb;
    veces = pveces;
    numC  = pnumC ;
    thr   = new Thread(this,"consumidor "+numC);
  }
  public void run() 
  {  
     for( int i=0 ; i<veces ; i++ )
      { double item = bb.extraer ();
        System.out.println(thr.getName()+", consumiendo "+item);
      } 
  }
}


class EjemploProductorConsumidor 
{ public static void main( String[] args ) 
  { if ( args.length != 5 ) 
    {  System.err.println("Uso: ncons nprod tambuf niterp niterc");
       return ;
    }
    // leer parametros, crear vectores y buffer intermedio
    Consumidor[] cons = new Consumidor[Integer.parseInt(args[0])] ;
	  Productor[]  prod = new Productor[Integer.parseInt(args[1])] ;
	  MonitorPC buffer = new MonitorPC(Integer.parseInt(args[2]));
	  int iter_cons = Integer.parseInt(args[3]);
	  int iter_prod = Integer.parseInt(args[4]);
	  if ( cons.length*iter_cons != prod.length*iter_prod )
	  { System.err.println("no coinciden número de items a producir con a cosumir");
      return ;
    }    
	  // crear hebras
	  for(int i = 0; i < cons.length; i++) 
	    cons[i] = new Consumidor(buffer,iter_cons,i) ;
	  for(int i = 0; i < prod.length; i++)
	    prod[i] = new Productor(buffer,iter_prod,i) ;
	  // poner en marcha las hebras
	  for(int i = 0; i < prod.length; i++) prod[i].thr.start();
	  for(int i = 0; i < cons.length; i++) cons[i].thr.start();
  }
}


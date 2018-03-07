
package task2;

import java.util.*;

public class Bee extends Thread{
    
    private Pot pot ;
    private int id ;
    private boolean done = false;
    
    public Bee (int id, Pot pot){
        this.id =id ;
        this.pot = pot ;
    }
    
   public void run() {
    while(!done) {
      try {
        sleep(1000);
      } catch (java.lang.InterruptedException e) {}
      if (done) break;
      
      pot.FillThePot();
      //System.out.println("bobo");
    }
  }
  public void setDone() {
    done = true;
  }
    
}

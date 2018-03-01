
package task2;

import java.util.logging.Level;
import java.util.logging.Logger;


public class Pot {
    private int HoneyPortions = 0 ;
    private int H ;
    
    
    public Pot(int H){
        this.H = H  ;
    }
    
    public synchronized void BearEat(){
        //System.out.println("bio");
        while (HoneyPortions < H) {
            try {
                wait();
            } 
            catch (InterruptedException ex) {
               
            }
        }
       
      
        //System.out.println("bbobo");
        HoneyPortions = 0 ;
        notifyAll();
        System.out.println("Bear: The pot is now empty.");
       
    }
    
    public synchronized void FillThePot(){
       
        while (HoneyPortions == H) {
      try {
        wait();
      } catch (java.lang.InterruptedException e) {}
    } 
           // System.out.println("bio");
            HoneyPortions ++ ;
             System.out.println("Bee "
                + Thread.currentThread().getId()
                + " added a honey portion and there are now in total " + HoneyPortions + " honey portions.");
            if (HoneyPortions == H) {
                notifyAll();
             try {
                 wait();
             } catch (InterruptedException ex) { }
            }
           

        
    }
    
   
    
}


package task1;
import java.util.*;
public class Dish {
    private int worms;
    private int w;

// set the worms intiially to w 
    public Dish(int worms) {
        this.worms = w = worms;
    }

    public synchronized void eat()  {
        while (worms == 0) {
            
             
             System.out.println("BabyBird "
                + Thread.currentThread().getId()
                + " CHIRPS " );
    
            notifyAll();
            
            try {
                wait();
            } catch (InterruptedException ex) {
             
            }

        }
        worms--;
        System.out.println("BabyBird "
                + Thread.currentThread().getId()
                + " ate a worm and worms left are " + worms);
    }

    // for the parent to wait for the dish to be empty 
    public synchronized void parentWait()  {

        while (worms > 0) {
            try {
                wait();
            } catch (InterruptedException ex) {
               
            }
        }

    }

    // 
    public synchronized void fillTheDish() {
      
      /*  try{
        Thread.sleep(10000);
        }
        catch(Exception e){
            
        } */
        
        worms = w;
        System.out.println("The dish is now filled with " + w + " worms by the parent.");
        notifyAll();
    }
}

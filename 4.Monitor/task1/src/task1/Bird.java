
package task1;
import java.util.*;

public class Bird extends Thread {
    
  private Dish dish ;
  private int ID ;
  
  public Bird(int ID , Dish dish){
      this.ID = ID ;
      this.dish =dish ;
      
  }
  
 
  public void run(){
      while(true){
         try {
        sleep(1000);
      } 
         catch (java.lang.InterruptedException e) {}
      dish.eat();
      }
      
  }
}
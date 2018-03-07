
package task1;

import java.util.*;


public class Task1 {

    public Task1 (String[] args) {
    int worms = 5;
   int birds  = 10 ;
   Dish dish = new Dish(worms);
 
    Parent parent = new Parent(dish);
     parent.start();
     
     
    Bird[] baby = new Bird[birds];
      for (int i = 0; i < birds; i++) {
        baby[i] = new Bird(i, dish);
        baby[i].start();
      }
      
      for (int i = 0; i <= birds-1; i++) {
        try {
          baby[i].join();
        } 
        catch (java.lang.InterruptedException e) {}
      }
      
       parent.setDishStatus();
      parent.interrupt();
      System.out.println("done!");
    } 
   
    
    public static void main(String[] args) {
          
   new Task1(args);
      
    }
    
}

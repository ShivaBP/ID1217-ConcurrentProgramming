
package task2;


public class Task2 {

   public Task2(String[] args){
       int honey = 7 ;
       int bees = 3 ;
       Pot pot = new Pot(honey);
       
       Bear bear = new Bear(pot);
       bear.start();
       
        Bee[] honeybee = new Bee[bees];
      for (int i = 0; i < bees; i++) {
          //System.out.println("done!");
        honeybee[i] = new Bee(i, pot);
        honeybee[i].start();
      }
      
      
     try {
        bear.join();
      } 
     catch (Exception e) {}
      for (int i = 0 ; i <= bees-1 ; i++) {
        honeybee[i].setDone();
        honeybee[i].interrupt();
      }
      System.out.println("done!");
       
   }
    public static void main(String[] args) {
       // System.out.println("done!");
       new Task2(args);
    }
    
}


package task1;


import java.util.*;
public class Parent extends Thread {
    
    private Dish dish;
  private boolean emptyDish = false;
  
  public Parent(Dish dish) {
    this.dish = dish;
  }
  
  public void run (){
    while (!emptyDish) {
      dish.parentWait();
      
      if (emptyDish) {
          break;
      }
      
      dish.fillTheDish();
    }
  }
  public void setDishStatus() {
    this.emptyDish = true;
  }
}

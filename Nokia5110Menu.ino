#include <PCD8544.h>
  PCD8544 lcd;
  int ground = 47;
  int load = 100;
  uint8_t data[] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
                    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};

 class Flake{
  public:
     unsigned int y1;
     unsigned int y2;
     unsigned int y3;
      unsigned int y4;
      
     void fall(){
        y1 += 1;
        y2 += 1;
        y3 += 1;
        y4 +=1;
        if (y1>=48) y1 = 0;
        if (y2>=48) y2 = 0;
        if (y3>=48) y3 = 0;
        if (y4>=48) y4 = 0;
     }

     void draw(){
        
        lcd.setCursor(50, y1);
        lcd.print("*");
        lcd.setCursor(70,y2);
        lcd.print("*");
        lcd.setCursor(10, y3);
        lcd.print("*");
        lcd.setCursor(30, y4);
        lcd.print("*");
     }

     Flake(){
        y1 = 0;
        y2 = 48;
        y4 = 30;
        y3 = 10;
     }
};

  Flake flake;
  
void setup() {
   lcd.begin(84, 48);
   lcd.setInverse(true);        // ...whole display
   lcd.setInverseOutput(false);  // ...future writes
}

void loop() {
  //clear screen
  lcd.clear();
  flake.fall();
  flake.draw();
  
  //draw ground
  int mground = load/10;
  int i = 0;
  do {
      lcd.setCursor(0,ground-i);
      lcd.drawBitmap(data, 84,1);
      i++;
  } while (i <= 10 - mground);
  
  load --;
  delay(200);
 }

// include the library code:
#include <LiquidCrystal.h>
#include <Wire.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
int switchState2;
int prevSwitchState2 = 1;
int counting = 0;
int cd = 30;
int button2 = 8;

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  lcd.print("Countdown: ");
  pinMode(button2, INPUT);
}

void loop() {
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 1);
  
  switchState2 = digitalRead(button2);
  
  if((prevSwitchState2 == 0) && (switchState2 == 1)){
    if(counting == 0){
      counting = 1;
    }else{
      counting = 0;
    }
  }
  
  prevSwitchState2 = switchState2;
  
  if (cd >= 0) {
  
    if (cd / 3600 >= 10) {
      lcd.print(cd / 3600);
    }else if (cd / 3600 > 0){
      lcd.print("0");
      lcd.print(cd / 3600);
    }else {
      lcd.print("00");
    }
  
    lcd.print(":");
  
    if ((cd - (cd / 3600) * 3600) / 60 >= 10) {
      lcd.print((cd - (cd / 3600) * 3600) / 60);
    }else if ((cd - (cd / 3600) * 3600) / 60 > 0){
      lcd.print("0");
      lcd.print((cd - (cd / 3600) * 3600) / 60);
    }else {
      lcd.print("00");
    }
  
    lcd.print(":");
  
    if (cd - (cd / 3600) * 3600 - ((cd - cd / 3600 * 3600) / 60) * 60 >= 10) {
      lcd.print(cd - (cd / 3600) * 3600 - ((cd - cd / 3600 * 3600) / 60) * 60);
    }else if (cd - (cd / 3600) * 3600 - ((cd - cd / 3600 * 3600) / 60) * 60 > 0) {
      lcd.print("0");
      lcd.print(cd - (cd / 3600) * 3600 - ((cd - cd / 3600 * 3600) / 60) * 60);
    }else {
      lcd.print("00");
    }
    
    if (counting == 1) {
      cd = cd - 1;
    }
  
    if (cd == -1) {
      lcd.setCursor(0, 1);
      lcd.print("Time's Up");
    }
  
    delay(1000);
  }
}

#include <LiquidCrystal.h>

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
const int buttonPin = 2;
int countdown= 100;

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.print("Time remaining:");
}

void loop() {
  int hour=countdown/3600;
  int minute=(countdown-hour*3600)/60;
  int second=countdown-hour*3600-minute*60;
  lcd.setCursor(4,1);
  if (hour < 10)
    lcd.print(' ');  // alternatively: '0'
  lcd.print(hour);
  lcd.print(":");
  if (minute < 10)
    lcd.print(' ');  // alternatively: '0'
  lcd.print(minute);
  lcd.print(":");
  if (second < 10)
    lcd.print(' ');  // alternatively: '0'
  lcd.print(second);
  delay(1000);
  if (hour!=0 || minute !=0 || second !=0)
    countdown=countdown-1;
  //else
    //lcd.setCursor(0,1);
    //lcd.print("WELL DONE");

}
 

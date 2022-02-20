#include <LiquidCrystal.h>
#include <TimeLib.h>
#include <Wire.h>

#define TIME_HEADER  "T"   // Header tag for serial time sync message
#define TIME_REQUEST  7    // ASCII bell character requests a time sync message 


// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int switchState;
int prevSwitchState = 1;
int button1 = 7;
int dclock = 1;

int switchState2;
int prevSwitchState2 = 1;
int counting = 0;
int cd = 30;
int button2 = 8;

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  Serial.begin(9600);
  while (!Serial) ; // Needed for Leonardo only

  pinMode(13, OUTPUT);
  pinMode(button2, INPUT);
  pinMode(button1, INPUT);

  setSyncProvider( requestSync);  //set function to call when sync required
  Serial.println("Waiting for sync message");
  lcd.print("Loading.....");
  //lcd.print("hello, world!");
}

void loop() {

//time counting with botton//

switchState2 = digitalRead(button2);
  
  if((prevSwitchState2 == 0) && (switchState2 == 1)){
    if(counting == 0){
      counting = 1;
    }else{
      counting = 0;
    }
  }
  
  prevSwitchState2 = switchState2;

//clock set up//
if (Serial.available()) {
    processSyncMessage();
  }

//switch between timer & clock//

 switchState = digitalRead(button1);
  
  delay(1);
  
  if((prevSwitchState == 0) && (switchState == 1)){
    
    if(dclock == 1){

      	dclock = 0;
      
    }else{

      	dclock = 1;
    }
    
  }
  
  prevSwitchState = switchState;  

if(clock == 1){
//display clock//

  if (timeStatus()!= timeNotSet) {
    digitalClockDisplay();  
  }
  if (timeStatus() == timeSet) {
    digitalWrite(13, HIGH); // LED on if synced
  } else {
    digitalWrite(13, LOW);  // LED off if needs refresh
  }
  delay(1000);
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 0);
}else{
//display timer//
    lcd.begin(16, 2);
    lcd.print("Countdown: ");

    lcd.setCursor(0, 1);

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
    }
    if (cd == -1) {
      lcd.setCursor(0, 1);
      lcd.print("Time's Up");
    }
  
    delay(1000);
}

if (counting == 1) {
      cd = cd - 1;
    }


}

void digitalClockDisplay(){
  // digital clock display of the time
  lcd.print("Time ");
  lcd.print(hour());
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  lcd.setCursor(0, 1);
  lcd.print("Date ");
  lcd.print(day());
  lcd.print("/");
  lcd.print(month());
  lcd.print("/");
  lcd.print(year()); 
   
  Serial.print(" ");
  Serial.print(day());
  Serial.print(" ");
  Serial.print(month());
  Serial.print(" ");
  Serial.print(year()); 
  Serial.println(); 
}

void printDigits(int digits){
  // utility function for digital clock display: prints preceding colon and leading 0
  lcd.print(":");
  if(digits < 10)
    lcd.print('0');
  lcd.print(digits);
}


void processSyncMessage() {
  unsigned long pctime;
  const unsigned long DEFAULT_TIME = 1357041600; // Jan 1 2013

  if(Serial.find(TIME_HEADER)) {
     pctime = Serial.parseInt();
     if( pctime >= DEFAULT_TIME) { // check the integer is a valid time (greater than Jan 1 2013)
       setTime(pctime); // Sync Arduino clock to the time received on the serial port
     }
  }
}

time_t requestSync()
{
  Serial.write(TIME_REQUEST);  
  return 0; // the time will be sent later in response to serial mesg
}
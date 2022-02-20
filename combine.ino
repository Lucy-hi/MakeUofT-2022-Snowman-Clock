#include <TimeLib.h>
#include <LiquidCrystal.h>
#define TIME_HEADER  "T"   // Header tag for serial time sync message
#define TIME_REQUEST  7    // ASCII bell character requests a time sync message


// initialize the library with the numbers of the interface pins
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int switchState;
int prevSwitchState = 1;
int hello = 0;

void setup() {
  // set up the LCD's number of columns and rows:
  Serial.begin(9600);
  
  lcd.begin(16, 2);
  // Print a message to the LCD.
  //lcd.print("hello, world!");
  
  //delay(2500);
  pinMode(7, INPUT);
  

  pinMode(13, OUTPUT);
  setSyncProvider( requestSync);  //set function to call when sync required
  Serial.println("Waiting for sync message");
  lcd.print("Loading.....");
  //lcd.print("hello, world!");
  
}

void loop() {
//CASE- COUNTDOWN:
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

  switchState = digitalRead(7);
  
  delay(1);
  
  if((prevSwitchState == 0) && (switchState == 1)){
    
    //delay(250);
    if(hello == 0){
      
      lcd.begin(16,2);
        lcd.clear();
      lcd.print("Time remaining:");
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
      
        hello = 1;
      
    }else{
      lcd.begin(16,2);
      lcd.clear();
        if (Serial.available()) {
          processSyncMessage();
        }
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
        hello = 0;
    }
    
  }
  prevSwitchState = switchState;       
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

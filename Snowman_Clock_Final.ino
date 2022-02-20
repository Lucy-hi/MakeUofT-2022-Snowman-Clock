#include <LiquidCrystal.h>
#include <TimeLib.h>
#include <Wire.h>
#include <Adafruit_PCD8544.h>

#define TIME_HEADER  "T"   // Header tag for serial time sync message
#define TIME_REQUEST  7    // ASCII bell character requests a time sync message 


// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 13, d4 = 9, d5 = 8, d6 = A5, d7 = A4;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int ground = 47;                   //wait controls the falling speed of snow and growth speed of pile, in miliseconds

Adafruit_PCD8544 display = Adafruit_PCD8544(3,4,5,7,6);

int switchState;
int prevSwitchState = 1;
int button1 = 11;
int dclock = 1;

int switchState2;
int prevSwitchState2 = 1;
int counting = 0;
int cd = 30;
int button2 = 10;


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
        
        display.setCursor(50, y1);
        display.println("*");
        display.setCursor(70,y2);
        display.println("*");
        display.setCursor(10, y3);
        display.println("*");
        display.setCursor(30, y4);
        display.println("*");
       // display.display();
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
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  Serial.begin(9600);
  display.begin();
  display.setContrast(50);
  display.invertDisplay(true);
  display.clearDisplay();
  
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
//if (Serial.available()) {
   // processSyncMessage();
 // }

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

if(dclock == 1){
//display clock//

  //lcd.clear();
  if (Serial.available()) {
    processSyncMessage();
  }
  if (timeStatus()!= timeNotSet) {
    digitalClockDisplay();  
  }
  if (timeStatus() == timeSet) {
    digitalWrite(12, HIGH); // LED on if synced
  } else {
    digitalWrite(12, LOW);  // LED off if needs refresh
  }
  delay(1000);
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 0);
}else{
//display timer//
    //lcd.clear();
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
  display.clearDisplay();
  
//  updateButton();
  
  if(counting == 1){

  flake.fall();
  flake.draw();
  display.display();
  }
  
  if (cd<=-1) { //when counter load finished, display a message
    displaySnowMan();
    
    display.setCursor(4,0);
    display.println("Take a break!");
    display.display();
    return;
   }
 
   double upperGround = 20 +floor(cd/2); 

   display.fillRect(0,upperGround,84,48-upperGround,BLACK);

    

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

  if(true) {
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


void displaySnowMan(){
     
    //display.setCursor(30,18);
    
    display.fillCircle(42,38,12, BLACK);
    display.fillCircle(42,20,7, BLACK);
    display.drawPixel(40,18,WHITE);
    display.drawPixel(43,18,WHITE);
    display.drawLine(39,24,40,25,WHITE);
    display.drawLine(41,25,42,24,WHITE);
    display.drawLine(32,31,26,25,BLACK);
    display.drawLine(54,32,57,26,BLACK);
  }

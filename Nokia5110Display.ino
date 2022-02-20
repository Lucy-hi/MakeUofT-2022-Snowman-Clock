

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

// Declare LCD object for software SPI
//Adafruit_PCD8544(CLK,DIN,D/C,CE,RST);


Adafruit_PCD8544 display = Adafruit_PCD8544(3, 4, 5, 7, 6);
int countdownStopPin = A8;
int modePin = A7;

double tmr = 20;
double load = tmr;
double threshold = tmr/10;

bool stopButtonPrev = analogRead(countdownStopPin);
bool stopButtonCur;

//bool modeButtonPrev = analogRead(modePin);
//bool modeButtonCur;

bool renderGraphic = false;

int ground = 47;                   //wait controls the falling speed of snow and growth speed of pile, in miliseconds


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
     }

     Flake(){
        y1 = 0;
        y2 = 48;
        y4 = 30;
        y3 = 10;
     }
};

  Flake flake;

  
void setup()   {
  Serial.begin(9600);
  //Serial.print("in setup");
  
  //Initialize Display
  display.begin();
  display.setContrast(50);
  display.invertDisplay(true);
  display.clearDisplay();
  
  }

void updateButton(){

   stopButtonCur = analogRead(countdownStopPin);
    
   if (stopButtonCur!=stopButtonPrev){
     if (stopButtonCur==1){
      //halt or continue graphics
        renderGraphic = !renderGraphic;
      }
      stopButtonPrev = stopButtonCur;
    }
    
}

void loop() {
  // Display Text
  
  display.clearDisplay();
  
  updateButton();
  
  if(!renderGraphic) return ;

  flake.fall();
  flake.draw();
  
   if (tmr>0)
    tmr-= load/100;

   else { //when counter load finished, display a message
    displaySnowMan();
    
    display.setCursor(4,0);
    display.println("Take a break!");
    display.display();
    return;
   }
 
   double upperGround = 20 +3*floor(tmr/threshold); 

   display.fillRect(0,upperGround,84,48-upperGround,BLACK);
  
   display.display();
   delay(load/100);
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
    display.drawLine(52,34,55,28,BLACK);
  }

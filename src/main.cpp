#include <Arduino.h>
#include <SPI.h>

#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

//#include "avr_debugger.h"
//#include "avr8-stub.h"
//#include "app_api.h"
//#include "deb.h"

#define Debbug
#ifdef Debbug
  //while not in use, comment also Seial.being 
  #define PRINT(t) (Serial.println((t)))
 
#endif

#include "btnPins.h"
#include "da.h"
#include "encoder.h"
#include "dispGra.h"

//git repo test 

//==========================================================================//
//------------------------- DISLPAY SETUP
//==========================================================================//
#define DCONST 100
Adafruit_PCD8544 display = Adafruit_PCD8544(6, 5, 4);


//==========================================================================//
//-------------------------MENU SETUP
//==========================================================================//
// select the enum with all the menu pages option
enum pageType { VOLT_SETTINGS, CURR_SETTINGS };

// holds witch page is currently selected
enum pageType currPage = CURR_SETTINGS;



//declaration of functions
void page_VoltSettings (void);
void page_CurrSettings (void);

int lastCount;


int digit_set(int a);

char buffer[10];
char buffer1[10];



//==========================================================================//
//-------------------------ARDUINO SETUP
//==========================================================================//
void setup() {

  //debbug tool - only one at the time can be choosen Seria monitor or AVR-STUB(REAL DEBBUGER)
  //while using interrupt is not possible use STUB 
  // debug_init();
  Serial.begin(9600);
  Serial.println("DEBBUG BEGIN !!!");

  pinMode(LED_BUILTIN, OUTPUT);

  //SPI setup
  SPI.begin();                                                     // Initialize SPI
  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0)); // Set SPI settings
  pinMode(SS, OUTPUT);                                             // Slave Select pin as OUTPUT

  // nokia display setup
  display.begin();
  display.setContrast(DCONST); // Set contrast to 50
  display.clearDisplay();
 
  // setup the basic I/Os
  pinMode(BTN_DIGIT, INPUT_PULLUP);
  pinMode(BTN_CHANGE, INPUT_PULLUP);
  pinMode(BTN_OUT_EN, INPUT_PULLUP);

  //encoder dependencies
  // Set encoder pins and attach interrupts
  pinMode(ENC_A, INPUT_PULLUP);
  pinMode(ENC_B, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(ENC_A), read_encoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENC_B), read_encoder, CHANGE);

  // sprintf(buffer,"%05.2f",01.23);
  // dtostrf(1.23, 5, 1, buffer1);
  // Serial.println(buffer);
  // Serial.println(buffer1);
}

//==========================================================================//
//-------------------------ARDUINO "MAIN"
//==========================================================================//
void loop() {

  switch (currPage)
  {
  case VOLT_SETTINGS:
    page_VoltSettings();
    break;
  case CURR_SETTINGS:
    page_CurrSettings();
    break;
  }
}

//==========================================================================//
//-------------------------PAGE - VOLT_SETTINGS
//==========================================================================//
void page_VoltSettings(void)
{
  // flag for updete dislay
  boolean updateDisplay = true;
  // track when entered top of loop
  uint32_t loopStartMs;

  // inner loop
  while (true)
  {
    loopStartMs = millis();   // capture start time

      if (updateDisplay)
      {
        updateDisplay = false;

        // display print l
        display.clearDisplay();

        //display.setTextColor(WHITE,BLACK);
        display.setTextSize(2);
        display.setCursor(12, 10);
        display.print(voltVal1);
        //display.print(voltageValue);
        
        display.setTextSize(1);
        display.setCursor(72, 16);
        display.print("mV");

        display.display();
      }
  
    captureButtonDownStates();
    voltageValue = constrain(voltageValue, minVoltage, maxVoltage);
    // looks for the press release action for OK
    if (btn_Digit_WasDown && btnIsUp(BTN_DIGIT))
    {

      updateDisplay = true;
      btn_Digit_WasDown = false;
    }

    //enable OUTPUT
    if (btn_Digit_WasDown && btnIsUp(BTN_OUT_EN))
    {

      updateDisplay = true;
      btn_Digit_WasDown = false;

    }
   
    // go back to settings
    if (btn_Change_WasDown && btnIsUp(BTN_CHANGE))
    {
      currPage = CURR_SETTINGS;

      updateDisplay = true;
      btn_Change_WasDown = false;

      return;
    }
    

    // keep a specific page
    while (millis() - loopStartMs < 25)
    {
      delay(2);
    }
  }
}


//==========================================================================//
//-------------------------PAGE - CURR_SETTINGS
//==========================================================================//
void page_CurrSettings(void)
{
  // flag for updete dislay
  boolean updateDisplay = true;
  // track when entered top of loop
  uint32_t loopStartMs;

  // inner loop
  while (true)
  {
    loopStartMs = millis(); 

    if (updateDisplay)
    {
      updateDisplay = false;
      
      //display print l
      display.clearDisplay();
      // display.setTextColor(WHITE,BLACK);

      display.setTextSize(3);

      if(currVal1 <= 9){
      display.setCursor(0, 0);
      display.print("0");
      display.setCursor(18, 0);
      display.print(currVal1);
      }else{
      display.setCursor(0, 0);
      display.print(currVal1);
      }

      if(currVal2 <= 9){
      display.setCursor(48, 0);
      display.print("0");
      display.setCursor(66, 0);
      display.print(currVal2);
      display.setCursor(0, 0);
      }else{
      display.setCursor(48, 0);
      display.print(currVal2);
      }

      display.setTextSize(2);
      display.setCursor(36, 8);
      display.print(".");
      display.setCursor(60, 32);
      display.print("mA");
      
      if(cursorPosition == 1){
        display.drawLine(18, 24, 33, 24, BLACK);
      }
      else{
        display.drawLine(66, 24, 81, 24 ,BLACK);
      }


      display.display();
    }

    captureButtonDownStates();
      // PRINT(counter);
      // PRINT(lastCount);
      // // PRINT(updateDisplay);
      // PRINT(" ");
      // PRINT(" ");
      // PRINT(" ");

    //setting current values 
    if(lastCount != counter){
      
      if(lastCount < counter){ 
        if(lastCount < counter && (lastCount + 10) < counter){
          cursorPosition == 1 ? currVal1 += 10 : currVal2 += 10;
            
        }else{cursorPosition == 1 ? currVal1++ : currVal2++;}     
  
      }
      else { 
        if(lastCount > counter && (lastCount -10) > counter){
          cursorPosition == 1 ? currVal1 -= 10 : currVal2 -= 10;

        }else{cursorPosition == 1 ? currVal1-- : currVal2--;} 
      }

      
      

      // Ensure the value stays within the specified limits
      currVal1 = constrain(currVal1, minCurrent, maxCurrent);
      currVal2 = constrain(currVal2, minCurrent2, maxCurrent2);
      lastCount = counter;
      updateDisplay = true;
    }

    // looks for the press release action for OK
    if (btn_Digit_WasDown && btnIsUp(BTN_DIGIT))
    {
      cursorPosition == 1 ? cursorPosition++ : cursorPosition--;
      updateDisplay = true;
      btn_Digit_WasDown = false;
    }

    if (btn_EnOut_WasDown && btnIsUp(BTN_OUT_EN))
    {
      



      updateDisplay = true;
      btn_EnOut_WasDown = false;
    }

    //switch signal source 
    if (btn_Change_WasDown && btnIsUp(BTN_CHANGE))
    {
      currPage = VOLT_SETTINGS;

      updateDisplay = true;
      btn_Change_WasDown = false;

      return;
    }
    

    // keep a specific page
    while (millis() - loopStartMs < 25)
    {
      delay(2);
    }
  }
}


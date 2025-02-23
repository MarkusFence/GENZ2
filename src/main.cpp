#include <Arduino.h>
#include <SPI.h>

#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

//#include "avr_debugger.h"
//#include "avr8-stub.h"
//#include "app_api.h"
#include "deb.h"

#define Debbug
#ifdef Debbug
  //while not in use, comment also Seial.being 
  #define PRINT(t) (Serial.println((t)))
  boolean debbug = true;
#else
  boolean debbug = false;
#endif

#include "btnPins.h"
#include "da.h"
#include "encoder.h"
#include "dispGra.h"


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
        display.print(voltage_value);
        //display.print(voltageValue);
        
        display.setTextSize(1);
        display.setCursor(72, 16);
        display.print("mV");

        display.display();
      }
  
    captureButtonDownStates();
    //voltageValue = constrain(voltageValue, minVoltage, maxVoltage);
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
  boolean enable_output = false;
  // track when entered top of loop
  uint32_t loopStartMs;


  // inner loop
  while(true)
  {
    loopStartMs = millis(); 
    //display Graphics
    if (updateDisplay)
    {
      updateDisplay = false;
      
      int digit_1;
      float digit_2;

      digit_1 = (int)current_value;
      digit_2 = ((float)current_value - digit_1) * 100; 

      display.clearDisplay();
      //display.setTextColor(WHITE,BLACK);

      display.setTextSize(3);
      
      if(digit_1 <= 9){
      display.setCursor(0, 0);
      display.print("0");
      display.setCursor(18, 0);
      display.print(digit_1);
      }else{
      display.setCursor(0, 0);
      display.print(digit_1);
      }

      if(digit_2 <= 9){
      display.setCursor(48, 0);
      display.print("0");
      display.setCursor(66, 0);
      display.print((int)digit_2);
      display.setCursor(0, 0);
      }else{
      display.setCursor(48, 0);
      display.print((int)digit_2);
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

    
    int last_value_change = true; //memory, previous value capture
    if (enable_output && last_value_change){

      calculate_hex();
      signal_output(set_current_mode[1], &current_value_hex);


    }


    captureButtonDownStates();

    //setting current values 
    if(lastCount != counter){
      
      if(lastCount < counter){ 
        if(lastCount < counter && (lastCount + 10) < counter){
          cursorPosition == 1 ? current_value += 10 : current_value += 0.1;
            
        }else{cursorPosition == 1 ? current_value++ : current_value += 0.01;}     
  
      }
      else { 
        if(lastCount > counter && (lastCount -10) > counter){
          cursorPosition == 1 ? current_value -= 10 : current_value -= 0.1;

        }else{cursorPosition == 1 ? current_value-- : current_value -= 0.01;} 
      }
      // Ensure the value stays within the specified limits
      current_value = constrain(current_value, minCurrent, maxCurrent);
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
      enable_output ? enable_output = false : enable_output = true;



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
      delay(200);
    }

    //PRINT debbug section 
    PRINT(enable_output);
    PRINT(current_value);
    PRINT(current_value_hex);

    if(debbug){
      for(int i = 0; i <= 2; i++){
        PRINT(" ");
      }
    }
  }
}


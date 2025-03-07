#include <Arduino.h>
#include <SPI.h>

// #include <Adafruit_GFX.h>
// #include <Adafruit_PCD8544.h>

#include "btnPins.h"
#include "da.h"
#include "encoder.h"
#include "dispGra.h"
#include "sense.h"

#include "deb.h"

//==========================================================================//
//------------------------- DISLPAY SETUP
//==========================================================================//
// #define DCONST 100
// Adafruit_PCD8544 display = Adafruit_PCD8544(6, 5, 4);


//==========================================================================//
//-------------------------MENU SETUP
//==========================================================================//
// select the enum with all the menu pages option
enum pageType { VOLT_SETTINGS, CURR_SETTINGS };

// holds witch page is currently selected
enum pageType currPage = VOLT_SETTINGS;

//declaration of functions
void page_VoltSettings (void);
void page_CurrSettings (void);

//==========================================================================//
//-------------------------ARDUINO SETUP
//==========================================================================//
void setup() {

  //PSEUDO DEBBUG TOOL
  Serial.begin(9600);
  Serial.println("DEBBUG BEGIN !!!");
  //short version of serial print
  PRINT("YEEEEEET");

  pinMode(LED_BUILTIN, OUTPUT);

  //SPI setup
  SPI.begin();                                                     // Initialize SPI
  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0)); // Set SPI settings
  pinMode(SS, OUTPUT);                                             // Slave Select pin as OUTPUT

  // NOKIA display setup
  display.begin();
  display.setContrast(DCONST); // Set contrast to 50
  display.clearDisplay();
 
  // setup the basic I/Os
  pinMode(BTN_DIGIT, INPUT_PULLUP);
  pinMode(BTN_CHANGE, INPUT_PULLUP);
  pinMode(BTN_OUT_EN, INPUT_PULLUP);

  //ENCODER dependencies
  //Set encoder pins and attach interrupts
  pinMode(ENC_A, INPUT_PULLUP);
  pinMode(ENC_B, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(ENC_A), read_encoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENC_B), read_encoder, CHANGE);

}

//==========================================================================//
//-------------------------ARDUINO "MAIN"
//==========================================================================//
void loop() {
  //MAIN LOOP for jump to sub-loops
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
  // flags
  boolean first_enable = true;
  boolean updateDisplay = true;
  boolean enable_output = false;

  //for trigger enable
  float previous_value = 0; 
  
  // track when entered top of loop
  uint32_t loopStartMs;

  // inner loop
  while (true)
  {
    loopStartMs = millis();   // capture start time
    previous_value = voltage_value;   

    //DISPLAY GRAPHICS
    if (updateDisplay)
    {
      updateDisplay = false;
      display.clearDisplay();
      //display.setTextColor(WHITE,BLACK);

      
      int v_digit_1;
      float v_digit_2;
      float voltage_value_absolute = voltage_value;

      if(voltage_value < 0){
        voltage_value_absolute *= -1;
        // minus graphic
        display.drawLine(0, 10, 4, 10, BLACK);
        display.drawLine(0, 11, 4, 11, BLACK);
      };

      //digit calculation before print
      v_digit_1 = (int)voltage_value_absolute;
      v_digit_2 = ((float)voltage_value_absolute - v_digit_1) * 100; 
  
      display.setTextSize(3);
      
      if(v_digit_1 < 10){
        display.setCursor(7, 0);
        display.print("0");
        display.setCursor(25, 0);
        display.print(v_digit_1);
      }else{
        display.setCursor(7, 0);
        display.print(v_digit_1);
      }

      if(v_digit_2 < 10){
        display.setCursor(48, 0);
        display.print("0");
        display.setCursor(66, 0);
        display.print((int)v_digit_2);
        display.setCursor(0, 0);
      }else{
        display.setCursor(48, 0);
        display.print((int)v_digit_2);
      } //dot graphic
        display.setTextSize(2);
        display.setCursor(38, 8);
        display.print(".");
        //source option
        display.setCursor(72, 32);
        display.print("V");
      
      if(cursorPosition == 1){
        display.drawLine(24, 24, 39, 24, BLACK);
      }
      else{
        display.drawLine(66, 24, 81, 24 ,BLACK);
      }

      display.setTextSize(1);
      if(enable_output){
        display.fillCircle( 6, 41, 4, BLACK);
        display.drawCircle( 6, 41, 6, BLACK);
      }else{
        display.drawCircle( 6, 41, 6, BLACK);
      }

      for(int i = 0; i > 10; i++){
      PRINT(" ");
      }
      display.display();
    }
    
    //BUTTOM UTILITIES
    captureButtonDownStates();
    //SWAP DECIMAL POSITION
    if(btn_Digit_WasDown && btnIsUp(BTN_DIGIT))
    {
      cursorPosition == 1 ? cursorPosition++ : cursorPosition--;
      updateDisplay = true;
      btn_Digit_WasDown = false;
      
    }

    //SET ENABLE FLAG TRUE
    if(btn_EnOut_WasDown && btnIsUp(BTN_OUT_EN))
    {
      enable_output ? enable_output = false : enable_output = true;
      updateDisplay = true;
      first_enable = true; //any enable press 
      btn_EnOut_WasDown = false;
    }
    
    //CHANGE TO CURRENT SOURCE 
    if(btn_Change_WasDown && btnIsUp(BTN_CHANGE))
    {
      currPage = CURR_SETTINGS;

      disable_output(mode_set_bi_volage);
      Serial.println(mode_set_bi_volage, HEX);

      updateDisplay = true;
      btn_Change_WasDown = false;

      return;
    }
    


    //SET VOLTAGE VALUE 
    if(lastCount != counter){
      float old = voltage_value;

      if(lastCount < counter){ 
        if(lastCount < counter && (lastCount + 10) < counter){
          cursorPosition == 1 ? voltage_value += 10 : voltage_value += 0.1;
            
        }else{
          if(cursorPosition == 1){
            voltage_value++;

            if(old < 0.00 &&  old > -1 && old < voltage_value){
              old *= (-2);
              voltage_value += old;
            }
          }else{voltage_value += 0.01;}     
        }
      }else{
        if(lastCount > counter && (lastCount -10) > counter){
          cursorPosition == 1 ? voltage_value -= 10 : voltage_value -= 0.1;

        }else{  
          if(cursorPosition == 1){
            voltage_value--;

            if(old > 0.00 && old < 1 && old > voltage_value){
              old *= (2);
              voltage_value -= old;
            }
          }else{voltage_value -= 0.01;} 
        } 
      }

      // Ensure the value stays within the specified limits
      voltage_value = constrain(voltage_value, minVoltage, maxVoltage);
      lastCount = counter;
      updateDisplay = true;
    }


    //FINNAL ENABLE
    if(enable_output && (voltage_value != previous_value || first_enable) ){

      uint16_t just_now;
      signal_output(mode_set_bi_volage, &voltage_value_hex, &just_now);
      first_enable = false;
    }

    //ERROR CHECK
    if(enable_output){

      voltage_sense = analogRead(BTN_sense_voltage);



    }


    // keep a specific time 
    while(millis() - loopStartMs < 25)
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
  // flag 
  boolean first_enable = true;
  boolean updateDisplay = true;
  boolean enable_output = false;
  float last_value_change = 0; //memory, previous value capture
  // track when entered top of loop
  uint32_t loopStartMs;


  // inner loop
  while(true)
  {
    loopStartMs = millis(); 
    last_value_change = current_value; //enable flag output, enable change only every 

    //DISPLAY GRAPHICS
    if (updateDisplay)
    {
      updateDisplay = false;
      
      graphisc_print(enable_output);
    }
    
    //BUTTOM UTILITIES
    captureButtonDownStates();
    //CHANGE DIGIT POSITON
    if (btn_Digit_WasDown && btnIsUp(BTN_DIGIT))
    {
      cursorPosition == 1 ? cursorPosition++ : cursorPosition--;
      updateDisplay = true;
      btn_Digit_WasDown = false;
      
    }
    
    //SET ENABLE FLAG
    if (btn_EnOut_WasDown && btnIsUp(BTN_OUT_EN))
    {
      enable_output ? enable_output = false : enable_output = true;
      first_enable = true;
      updateDisplay = true;
      btn_EnOut_WasDown = false;
    }

    //SWITCH TO VOLTAGE SOURSE
    if (btn_Change_WasDown && btnIsUp(BTN_CHANGE))
    {
      currPage = VOLT_SETTINGS;

      updateDisplay = true;
      btn_Change_WasDown = false;

      return;
    }
    
    //SET CURRENT VALUE
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
    
    //FINAL ENABLE
    uint16_t nothing;
    if (enable_output && (current_value != last_value_change || first_enable)){
      
      signal_output(set_current_mode[2], &current_value_hex, &nothing);
      first_enable = false;
    }

    //ERROR CHECK
    if(enable_output){

      current_sense = analogRead(BTN_sense_current);



    }
    

    // keep a specific page
    while (millis() - loopStartMs < 25)
    {
      delay(2); //DELAY is created in argument of this while loop (25ms) 
    }

  }
}


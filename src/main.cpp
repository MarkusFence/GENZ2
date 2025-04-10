#include <Arduino.h>
#include <SPI.h>

// #include <Adafruit_GFX.h>
// #include <Adafruit_PCD8544.h>

#include "buttons.h"
#include "dac.h"
#include "encoder.h"
#include "graphics.h"
#include "measure.h"
#include "power_save.h"

#include "deb.h"

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

  //SPI setup
  SPI.begin();                                                     // Initialize SPI
  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0)); // Set SPI settings
  pinMode(SS, OUTPUT);                                             // Slave Select pin as OUTPUT

  //  NOKIA display setup
  display.begin();
  display.setContrast(DCONST); // Set contrast to 50
  display.clearDisplay();
 
  // I/Os SETUP
  pinMode(BTN_DIGIT, INPUT_PULLUP);
  pinMode(BTN_CHANGE, INPUT_PULLUP);
  pinMode(BTN_OUT_EN, INPUT_PULLUP);

  pinMode(display_light, OUTPUT);
  pinMode(power_out, OUTPUT);
  //pinMode();

  //  ENCODER setup
  //  Set encoder pins and attach interrupts
  pinMode(ENC_A, INPUT_PULLUP);
  pinMode(ENC_B, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(ENC_A), read_encoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENC_B), read_encoder, CHANGE);

  //MAIN POWER DISABLE FOR 1. START
  power(false);
  
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
  boolean enable_output = false;
  //boolean p_output = false;
  boolean error = false;
  boolean updateDisplay = true;
  boolean darkMode = false;

  uint32_t btn_Digit_RepeatCnt = 0;
  unsigned long btn_Digit_LastRepeatMs = 0;

  //for trigger enable
  float previous_value; 
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
      graphics_print_VoltSource(enable_output, error, U_sense);

      //PRINT(U_sense);

    }
    
    //BUTTOM UTILITIES
    captureButtonDownStates();

    //SWAP DECIMAL POSITION
    if(btn_Digit_WasDown && btnIsUp(BTN_DIGIT))
    {
      cursorPosition == 1 ? cursorPosition++ : cursorPosition--;
      updateDisplay = true;
      btn_Digit_WasDown = false;
      btn_Digit_RepeatCnt = 0;
    
    }else if (btnRepeat(btn_Digit_WasDown, &btn_Digit_LastRepeatMs, &btn_Digit_RepeatCnt)){
      if(btn_Digit_RepeatCnt == 5){
        darkMode ? darkMode = false : darkMode = true;
        light(darkMode);
        Serial.print(" LED ");
        Serial.println(darkMode);
      }
    }

    //SET ENABLE FLAG TRUE
    if(btn_EnOut_WasDown && btnIsUp(BTN_OUT_EN))
    {
      enable_output ? enable_output = false : enable_output = true;

      // if(enable_output){
      //   start_time_power = millis();
      //   p_output = true;
      // }

      if(!enable_output){disable_output();}
      
      updateDisplay = true;
      first_enable = true; //any enable press 
      btn_EnOut_WasDown = false;
    }
    
    //CHANGE TO CURRENT SOURCE 
    if(btn_Change_WasDown && btnIsUp(BTN_CHANGE))
    {
      currPage = CURR_SETTINGS;

      disable_output();

      updateDisplay = true;
      btn_Change_WasDown = false;

      return;
    }
    
    // //lights ON
    // if(btn_Digit_WasDown){
    //   down_time += 1;
    //   if(down_time < 100){
    //     down_time = 0;
    //     PRINT("LED ON");
    //   }
    // }

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


    // //POWER ENABLE 
    // if(enable_output && p_output){

    //   power(p_output); // +1s delay in power func.  
    // }

    //POWER SAVING - when to shutdown power
    if(start_time_power - millis() > interval_power){
      start_time_power += interval_power;
      //ower off
    }


    //FINNAL ENABLE
    if(enable_output && (voltage_value != previous_value || first_enable)){

      signal_output(mode_set_bi_voltage, &voltage_value_hex);

      first_enable = false;
    }

    //MEASURE / ERROR CHECK
    if(enable_output && time_to_measure(&start_time_Usense, &U_sense, loopStartMs, pin_Usense)){

      U_convert(&U_sense);

      if( ((voltage_value - toleration) <= U_sense) && ((voltage_value + toleration) >= U_sense)){
        error = false;
        PRINT("NO ERROR");
      } 
      else{
        error = true;
        PRINT("ERROR");
      }

      PRINT(U_sense);
      PRINT("--------------------------");

      updateDisplay = true;
    }

   
  //   uint32_t current_time = millis();
    
  //   if(current_time - start_time >= interval){ //current time needed maybe
  //     start_time = current_time;

  //     PRINT("measure");
  // }


    //keep a specific time 
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
  boolean enable_output = false;
  //boolean old_enable_output = false;
  boolean updateDisplay = true;

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
      
      graphisc_print_CurrSource(enable_output, I_value);
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
    
    //SET ENABLE FLAG               BAD NAMEing
    if (btn_EnOut_WasDown && btnIsUp(BTN_OUT_EN))
    {
      enable_output ? enable_output = false : enable_output = true;

      if(!enable_output){disable_output();}

      first_enable = true;
      updateDisplay = true;
      btn_EnOut_WasDown = false;
    }

    //SWITCH TO VOLTAGE SOURSE
    if (btn_Change_WasDown && btnIsUp(BTN_CHANGE))
    {
      currPage = VOLT_SETTINGS;

      power(false);
      disable_output();

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
    if (enable_output && (current_value != last_value_change || first_enable)){
      
      signal_output(mode_set_current[2], &current_value_hex);
      first_enable = false;
    //  old_enable_output = enable_output;

    // }else if( !enable_output && old_enable_output){

    //   disable_output();
    //   old_enable_output = enable_output;

    //   Serial.println(mode_set_current[0], HEX);
    }

    //ERROR CHECK
    if(enable_output){

      //current_sense = analogRead(BTN_sense_current);
      updateDisplay = true;
    }
    

    // keep a specific page
    while (millis() - loopStartMs < 25)
    {
      delay(2); //DELAY is created in argument of this while loop (25ms) 
    }

  }
}


#include <Arduino.h>

const char BTN_DIGIT = A0;
const char BTN_CHANGE = A1;
const char BTN_OUT_EN = A2;

const char BTN_error = A7;
const char BTN_sense_voltage = A6;
const char BTN_sense_current = A5;


boolean btn_Digit_WasDown = false;
boolean btn_Change_WasDown = false;
boolean btn_EnOut_WasDown = false;

boolean buttonReleased(uint8_t buttonPin, boolean &wasDownFlag)
{
    if (wasDownFlag && digitalRead(buttonPin) == HIGH)
    {
        wasDownFlag = false;
        return true;
    }
    return false;
}

//==========================================================================//
//-------------------------TOOLS - BUTTONS PRESSING
//==========================================================================//
boolean btnIsDown(int btn)
{return digitalRead(btn) == LOW && digitalRead(btn) == LOW;} // buttons tested 2x as a bounce methode
boolean btnIsUp(int btn)
{return digitalRead(btn) == HIGH && digitalRead(btn) == HIGH;}

void captureButtonDownStates()
{
  if (btnIsDown(BTN_DIGIT)){btn_Digit_WasDown = true;}
  if (btnIsDown(BTN_CHANGE)){btn_Change_WasDown = true;}
  if (btnIsDown(BTN_OUT_EN)){btn_EnOut_WasDown = true;}
}
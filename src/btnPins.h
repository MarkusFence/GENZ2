#include <Arduino.h>

const char BTN_DIGIT = A0;
const char BTN_CHANGE = A2; 
const char BTN_OUT_EN = A1;

const char BTN_error = A3;

const char BTN_sense_voltage = A7;
const char BTN_sense_current = A6;

boolean btn_Digit_WasDown = false;
boolean btn_Change_WasDown = false;
boolean btn_EnOut_WasDown = false;

//==========================================================================//
//-------------------------TOOLS - BUTTONS PRESSING
//==========================================================================//

boolean buttonReleased(uint8_t buttonPin, boolean &wasDownFlag)
{
    if (wasDownFlag && digitalRead(buttonPin) == HIGH)
    {
        wasDownFlag = false;
        return true;
    }
    return false;
}

//risign edge detection
boolean btnIsDown(int btn)
{return digitalRead(btn) == LOW && digitalRead(btn) == LOW;} // buttons tested 2x as a bounce methode

boolean btnIsUp(int btn)
{return digitalRead(btn) == HIGH && digitalRead(btn) == HIGH;}


boolean btnRepeat(boolean btnWasDown, unsigned long *lastRepeatMs, uint32_t *repeatCnt)
{
    uint32_t currMs = millis();
    if (btnWasDown && (*repeatCnt == 0 ||                                        // if this is the first time to repeat > OR
                       (*repeatCnt > 5 && currMs >= (*lastRepeatMs + 200)) ||    // if more than the 5th repeat then lock to 200ms interva
                       currMs >= (*lastRepeatMs + 250 + (50 * (5 - *repeatCnt))) // otherwise calculate a proportional interval to check a
                       ))
    {
        if (*repeatCnt < 999)
        {
            *repeatCnt += 1;
        }
        *lastRepeatMs = currMs;
        return true;
    }
    else
        return false;
}


void captureButtonDownStates()
{
  if (btnIsDown(BTN_DIGIT)){btn_Digit_WasDown = true;}
  if (btnIsDown(BTN_CHANGE)){btn_Change_WasDown = true;}
  if (btnIsDown(BTN_OUT_EN)){btn_EnOut_WasDown = true;}
}

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>


//==========================================================================//
//------------------------- DISLPAY SETUP
//==========================================================================//
#define DCONST 100
Adafruit_PCD8544 display = Adafruit_PCD8544(6, 5, 4); //8 pin is NC new folow (no reset needed)

//graphical digit pointer 
int cursorPosition = 1;

//GRAPHICS FOR CURRENT SOURCE
void graphics_print_CurrSource(boolean enable_output, boolean power_enable, float sense, boolean *overLimit, boolean *Z){
    display.clearDisplay();
    //display.setTextColor(WHITE,BLACK);

//==========================================================================//
//------------------------- SET DIGITS
//==========================================================================//

    int digit_1;
    float digit_2;
    int scaled_value = (int)(current_value * 100 + 0.5);

    digit_1 = scaled_value / 100;   
    digit_2 = scaled_value % 100;

    display.setTextSize(3);

    if(digit_1 < 10){
    display.setCursor(7, 0);
    display.print("0");
    display.setCursor(25, 0);
    display.print(digit_1);
    }else{
    display.setCursor(7, 0);
    display.print(digit_1);
    }

    if(digit_2 < 10){
    display.setCursor(48, 0);
    display.print("0");
    display.setCursor(66, 0);
    display.print((int)digit_2);
    display.setCursor(0, 0);
    }else{
    display.setCursor(48, 0);
    display.print((int)digit_2);
    }

//==========================================================================//
//------------------------- OTHER GRAPHICS
//==========================================================================//
    //dot graphic
    display.setTextSize(2); // IMPORTANT HERE
    display.setCursor(38, 8);
    display.print(".");

    //source option graphics
    display.setCursor(60, 32);
    display.print("mA");

    //digit pair select graphics
    if(cursorPosition == 1){
    display.drawLine(24, 23, 39, 23, BLACK);
    }
    else{
    display.drawLine(66, 23, 81, 23 ,BLACK);
    }

    //enable graphics
    display.setTextSize(1); // IMPORTANT HERE
    if(enable_output){
        display.fillCircle( 6, 41, 4, BLACK);
        display.drawCircle( 6, 41, 6, BLACK);
        }else{
        display.drawCircle( 6, 41, 6, BLACK);
        }

//==========================================================================//
//------------------------- DETECTION AND ERROR GRAPHICS
//==========================================================================//
  
    if(*overLimit && enable_output){
        display.setCursor(19, 25);
        display.print("EXTERNAL");
        display.setCursor(23, 33);
        display.print("SOURCE");
    }else if(*Z && enable_output){
        display.setCursor(23, 29);
        display.print("HIGH Z");
    }

    display.setCursor(24, 41);
    if(power_enable){
        display.print(sense);
    }else{
        display.setCursor(18, 41);
        display.print("OFF");
    }
    
    //FINNALE PRINT 
    display.display();
}

void graphics_print_VoltSource(boolean enable_output, boolean power_enable, float sense, boolean *overLimit, boolean *Z){

    display.clearDisplay();
    //display.setTextColor(WHITE,BLACK);

//==========================================================================//
//------------------------- SET DIGITS
//==========================================================================//
    int v_digit_1;
    float v_digit_2;
    int scaled_value;
    float voltage_value_absolute = voltage_value;

    // minus graphic
    if(voltage_value < 0){
        voltage_value_absolute *= -1;
        // minus graphic
        display.drawLine(0, 10, 4, 10, BLACK);
        display.drawLine(0, 11, 4, 11, BLACK);
    }

    scaled_value = (int)(voltage_value_absolute * 100 + 0.5);
    v_digit_1 = scaled_value / 100;   
    v_digit_2 = scaled_value % 100;

    display.setTextSize(3); // IMPORTANT HERE
    
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
    }

//==========================================================================//
//------------------------- OTHER GRAPHICS
//==========================================================================//
    //dot graphic
    display.setTextSize(2); // IMPORTANT HERE
    display.setCursor(38, 8);
    display.print(".");

    //source option graphics
    display.setCursor(72, 32);
    display.print("V");
    
    //digit pair select graphics
    if(cursorPosition == 1){
        display.drawLine(24, 23, 39, 23, BLACK);
    }
    else{
        display.drawLine(66, 23, 81, 23 ,BLACK);
    }
    //enable graphics
    display.setTextSize(1); // IMPORTANT HERE
    if(enable_output){
        display.fillCircle( 6, 41, 4, BLACK);
        display.drawCircle( 6, 41, 6, BLACK);
    }else{
        display.drawCircle( 6, 41, 6, BLACK);
    }

//==========================================================================//
//------------------------- DETECTION AND ERROR GRAPHICS
//==========================================================================//
    
    if(*overLimit && enable_output){
        display.setCursor(19, 25);
        display.print("EXTERNAL");
        display.setCursor(23, 33);
        display.print("SOURCE");
    }else if(*Z && enable_output){
        display.setCursor(23, 29);
        display.print(" LOW Z");
    }

    //measured value
    display.setCursor(24, 41);
    if(power_enable){
        display.print(sense);
    }else{
        display.setCursor(18, 41);
        display.print("OFF");
    }
    //FINNALE PRINT
    display.display();
}
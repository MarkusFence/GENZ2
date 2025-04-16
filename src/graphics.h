#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

//#include "deb.h"

//==========================================================================//
//------------------------- DISLPAY SETUP
//==========================================================================//
#define DCONST 100
Adafruit_PCD8544 display = Adafruit_PCD8544(6, 5, 4); //8 pin is NC new folow (no reset needed)

//graphical digit pointer 
int cursorPosition = 1;

//GRAPHICS FOR CURRENT SOURCE
void graphisc_print_CurrSource(boolean enable_output, float sense, boolean *overLimit, boolean *Z){

    int digit_1;
    float digit_2;

    digit_1 = (int)current_value;
    digit_2 = ((float)current_value - digit_1) * 100; 

    display.clearDisplay();
    //display.setTextColor(WHITE,BLACK);

    display.setTextSize(3);

    if(digit_1 <= 9){
    display.setCursor(7, 0);
    display.print("0");
    display.setCursor(25, 0);
    display.print(digit_1);
    }else{
    display.setCursor(7, 0);
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

    display.setCursor(38, 8);
    display.print(".");
    display.setCursor(60, 32);
    display.print("mA");

    if(cursorPosition == 1){
    display.drawLine(24, 23, 39, 23, BLACK);
    }
    else{
    display.drawLine(66, 23, 81, 23 ,BLACK);
    }

    display.setTextSize(1);

    if(*overLimit && enable_output){
        display.setCursor(19, 25);
        display.print("EXTERNAL");
        display.setCursor(23, 33);
        display.print("SOURCE");
    }else if(*Z && enable_output){
        display.setCursor(23, 29);
        display.print("HIGH Z");
    }

    display.setCursor(30, 41);
    display.print(sense);

    if(enable_output){
    display.fillCircle( 6, 41, 4, BLACK);
    display.drawCircle( 6, 41, 6, BLACK);
    }else{
    display.drawCircle( 6, 41, 6, BLACK);
    }
    //FINNAL DISPLAY COMMAND 
    display.display();
}

void graphics_print_VoltSource(boolean enable_output, float sense, boolean *overLimit, boolean *Z){

    display.clearDisplay();
    //display.setTextColor(WHITE,BLACK);

//==========================================================================//
//------------------------- SET DIGITS
//==========================================================================//
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
    display.setCursor(30, 41);
    if(sense > 13 || sense < -12){
        display.print((int)sense);
    }else{
        display.print(sense);
    }
    //FINNALE PRINT
    display.display();
}
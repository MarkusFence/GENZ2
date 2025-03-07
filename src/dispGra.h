#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

//==========================================================================//
//------------------------- DISLPAY SETUP
//==========================================================================//
#define DCONST 100
Adafruit_PCD8544 display = Adafruit_PCD8544(6, 5, 4);

//graphical digit pointer 
int cursorPosition = 1;

//GRAPHICS
void graphisc_print(boolean enable_output){

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


    display.display();
}
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
void graphisc_print_CurrSource(boolean enable_output, float sense){

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

    // display.setCursor(13, 28);
    // display.print("IMPEDANCE");
    // display.setCursor(30, 39);
    // display.print("HIGH");

    // display.setCursor(21, 28);
    // display.print("BELOW");
    // display.setCursor(21, 39);
    // display.print("LIMIT");

    display.setCursor(30, 39);
    display.print(sense);

    if(enable_output){
    display.fillCircle( 6, 41, 4, BLACK);
    display.drawCircle( 6, 41, 6, BLACK);
    }else{
    display.drawCircle( 6, 41, 6, BLACK);
    }


    display.display();
}

void graphics_print_VoltSource(boolean enable_output, boolean error, float sense){

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

    // display.setCursor(13, 28);
    // display.print("IMPEDANCE");
    // display.setCursor(30, 39);
    // display.print("LOW");
    display.setCursor(30, 39);
    display.print(sense);



    if(enable_output){
    display.fillCircle( 6, 41, 4, BLACK);
    display.drawCircle( 6, 41, 6, BLACK);
    }else{
    display.drawCircle( 6, 41, 6, BLACK);
    }

    // for(int i = 0; i > 10; i++){
    // PRINT(" ");
    // }

    display.display();
}
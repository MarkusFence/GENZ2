#include <Arduino.h>

const char pin_Usense = A7;
const char pin_Isense = A6;

uint32_t start_time_Usense = 50;
uint32_t start_time_Isense = 75;

const uint32_t interval_sense = 1000;

float U_sense;
float I_value;

const float toleration = 00.10;

//-12.18 V >>   0.001 V
//13.52	V >>	4.987 V >> y = 2.36401 + 0.194008 * x

//-12.18 V >>   0 bit &&
//13.52	V >>    1024 bit >> y = 485.304 + 39.8444 * x
//measured 1024 = 4.927 V >> 13.21 V MAX to detect 

void U_convert(float *x){
    float y;
    y = (*x - 485.304) / 39.8444;
    *x = y;
}

void I_convert(float *x){
    float y;
    
    *x = y;
}

boolean time_to_measure (uint32_t *start_time_parameter, float *sense_val, uint32_t current_time, char pin){
    if(current_time - *start_time_parameter >= interval_sense){
        *start_time_parameter = current_time;
        *sense_val = analogRead(pin);

        return true;
    }
 return false;
} 
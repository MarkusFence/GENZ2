#include <Arduino.h>

const char pin_Usense = A7;
const char pin_Isense = A6;

uint32_t start_time_Usense = 50;
uint32_t start_time_Isense = 75;

const uint32_t interval_sense = 1000;

double U_sense;
double I_sense;

const float toleration = 00.10;

struct DetectionFlags{
    boolean over_limit = false;
    boolean high_Z = false;
    boolean low_Z = false; 
}detection;


//-12.18 V  >>   0.001 V || 13.52 V  >>	 4.987 V 
//0.001 = -12.18*a + b  &&  4.987 = 13.52*a + b 
// y = 2.36401 + 0.194008 * x
//measured 1024 = 4.927 V >> 13.21 V - MAX to detect 
//FOR 10-bit (y) >> y = 39.8444 * x + 485.304
void U_convert(double *x){
    double y;
    y = (*x - 490.736) / 40.2903;
    *x = y;
}
//from real measurements
//4.363 = 21.38*a + b  &&  1.9 = 9.23*a + b  
//measured 1024 = 4.927 V >> 24.1622 mA - MAX to detect
//0.0289309 V >> 0 bit || 4.927 V || 1024 bit a = 209.062
//FOR 10-bit (y) >> y = 209.062 * x − 6.04835 
void I_convert(double *x){
    double y;
    y = (*x) / 42.3389;
    *x = y;
}

boolean time_to_measure (uint32_t *start_time_parameter, double *sense_val, uint32_t current_time, char pin){
    if(current_time - *start_time_parameter >= interval_sense){
        *start_time_parameter = current_time;
        *sense_val = analogRead(pin);
        return true;
    }
 return false;
} 

void test_output(DetectionFlags *s){
    if(voltage_value > 0){
        if( ((voltage_value - toleration) <= U_sense) && ((voltage_value + toleration) >= U_sense)){
            s->low_Z = false;
            s->over_limit = false;

            Serial.println("FINE");
        } 
        else if((voltage_value + toleration) >= U_sense && U_sense > 0){
            s->low_Z = true;
            s->over_limit = false;

            Serial.println("LOW Z");
        }else if((voltage_value - toleration) <= U_sense || U_sense < 0){
            s->low_Z = false;
            s->over_limit = true;

            Serial.println("EXTERNAL SOURCE");
        }
    }else{
        if( ((voltage_value - toleration) <= U_sense) && ((voltage_value + toleration) >= U_sense)){
            s->low_Z = false;
            s->over_limit = false;
    
            Serial.println("FINE");

        }else if((voltage_value - toleration) >= U_sense || U_sense > 0){
            s->low_Z = false;
            s->over_limit = true;
            Serial.println("EXTERNAL SOURCE");
            
        }else if((voltage_value + toleration) <= U_sense && U_sense < 0){
            s->low_Z = true;
            s->over_limit = false;
            Serial.println("LOW Z");
        }else{
            // s->low_Z = false;
            // s->over_limit = false;  
        }
    }
}
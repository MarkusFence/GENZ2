#include <Arduino.h>

const char pin_Usense = A6;
const char pin_Isense = A5;

//measurement intervals offset 
uint32_t start_time_Usense = 50;
uint32_t start_time_Isense = 75;
//measurement interval
const uint32_t interval_sense = 1000;

float U_sense;
float I_sense;

const float toleration = 00.10;

struct DetectionFlags{
    boolean over_limit = false;
    boolean high_Z = false;
    boolean low_Z = false; 
}detection;

//for compare and display output values 
void U_convert(float *x){
    float y;
    y = ((*x * 100000) + 342765)/ 4301230;
    *x = y;
}
//for compare and display output values
void I_convert(float *x){
    float y;
    y = (*x * 10000 - 4885070)/403251;
    *x = y;
}
//time interval for measurements
boolean time_to_measure (uint32_t *start_time_parameter, float *sense_val, uint32_t current_time, char pin){
    if(current_time - *start_time_parameter >= interval_sense){
        *start_time_parameter = current_time;
        *sense_val = analogRead(pin);
        return true;
    }
 return false;
} 
//error detection 
void test_output(DetectionFlags *s, float set_value, float sense_value){
    // Serial.println(set_value);
    // Serial.println(sense_value);
    if(set_value > 0){
        if( ((set_value - toleration) <= sense_value) && ((set_value + toleration) >= sense_value)){
            s->low_Z = false;
            s->over_limit = false;
            //Serial.println("FINE");
        } 
        else if((set_value + toleration) >= sense_value && sense_value > 0){
            s->low_Z = true;
            s->over_limit = false;
            //Serial.println("LOW Z");
        }else if((set_value - toleration) <= sense_value || sense_value < 0){
            s->low_Z = false;
            s->over_limit = true;
           //Serial.println("EXTERNAL SOURCE");
        }
    }else{
        if( ((set_value - toleration) <= sense_value) && ((set_value + toleration) >= sense_value)){
            s->low_Z = false;
            s->over_limit = false;
            //Serial.println("FINE");

        }else if((set_value - toleration) >= sense_value || sense_value > 0){
            s->low_Z = false;
            s->over_limit = true;
            //Serial.println("EXTERNAL SOURCE");
            
        }else if((set_value + toleration) <= sense_value && sense_value < 0){
            s->low_Z = true;
            s->over_limit = false;
            //Serial.println("LOW Z");
        }
    }
}


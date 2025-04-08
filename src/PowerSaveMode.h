#include <Arduino.h>

const char display_light = 4;
const char power_out = 7;

unsigned long start_time_power = 0;
unsigned long start_time_light = 0;

const unsigned long interval_power = 30000; //5 min == 3600000 
const unsigned long interval_Light = 10000;

void power(boolean enable){
    enable ? digitalWrite(power_out, HIGH) : digitalWrite(power_out, LOW);
  }

void light(boolean enable){
    enable ? digitalWrite(display_light, HIGH) : digitalWrite(display_light, LOW);
}



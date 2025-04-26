#include <Arduino.h>

boolean darkMode = false;
boolean power_enable = false;

const char display_light = 4;
const char power_out_pin = 7;

//interval off-set
uint32_t start_time_power = 0;
uint32_t start_time_light = 25;

//power saving intervals
const unsigned long interval_power = 7000; //5 min == 3600000 
const unsigned long interval_light = 7000;

//power supply management
void power(boolean enable){
    if(enable){ 
      digitalWrite(power_out_pin, HIGH);
      delay(333);
      Serial.println("P ON");
    }else{ 
      delay(333);
      digitalWrite(power_out_pin, LOW);
      Serial.println("P OFF");
    }
}
//diplay light management
void light(boolean enable){
    enable ? digitalWrite(display_light, HIGH) : digitalWrite(display_light, LOW);
}

//interval calculation
boolean power_save(uint32_t current_time, uint32_t *start_time, const unsigned long interval){
  if(current_time - *start_time > interval){
    *start_time = current_time;
    return true; 
  }
  return false;
}



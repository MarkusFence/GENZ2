#include <Arduino.h>

boolean power_enable = false;

const char display_light = 4;
const char power_out_pin = 7;

uint32_t start_time_power = 0;
uint32_t start_time_light = 25;

const unsigned long interval_power = 7000; //5 min == 3600000 
const unsigned long interval_light = 5000;

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
void light(boolean enable){
    enable ? digitalWrite(display_light, HIGH) : digitalWrite(display_light, LOW);
}

boolean power_save(uint32_t current_time, uint32_t *start_time, const unsigned long interval){
  if(current_time - *start_time > interval){
    *start_time = current_time;
    return true; 
  }
  return false;
}



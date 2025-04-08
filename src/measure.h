#include <Arduino.h>

volatile unsigned int start_time_Vsense = 0;
volatile unsigned int start_time_Isense = 0;

const unsigned int interval_Vsense = 2000;
const unsigned int interval_Isense = 2000;

boolean time_to_measure;

volatile int voltage_sense;
volatile int current_sense;
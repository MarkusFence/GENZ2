#include <Arduino.h>

#define Debbug
#ifdef Debbug
  //while not in use, comment also Seial.being 
  #define PRINT(t) (Serial.println((t)))
  boolean debbug = true;
#else
  boolean debbug = false;
#endif
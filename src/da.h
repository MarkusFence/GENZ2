#include <Arduino.h>
#include <SPI.h>

// 1.st 8-bit addresses word
byte data_r = 0x01;
byte readback_r = 0x02;
byte control_r = 0x55;
byte reset_r = 0x56;

/// nop command ///////////////////////////////////////////////////////
uint8_t nop8 = 0x00;
uint16_t nop16 = 0x0000;

/// Data for reading from registers //////////////////////////////////////////////////////////////////
uint16_t read_status_r = 0x0000;
uint16_t read_data_r = 0x0001;
uint16_t read_control_r = 0x0002;

// data to set OUTPUT
uint16_t cntrltReg_volt = 0x3001;
uint16_t cntrlReg_voltBi = 0x3003;
uint16_t dataReg_volt = 0xFFFF;

uint16_t disableOut = 0x2000;

// data to set output
uint16_t cntrlReg_cur = 0x3006;
uint16_t dataReg_cur = 0xFFFF;

int cursorPosition = 1;

int voltVal1 = 0;
int voltVal2 = 0;

int currVal1 = 0;
int currVal2 = 0;


float voltageValue = 0; // Initial voltage value
float minVoltage = 00.00;
float maxVoltage = 10.00;


int currentValue = 20;
int minCurrent = 0;
int maxCurrent = 20;


int currentValue2 = 99;
int minCurrent2 = 0;
int maxCurrent2 = 99;

void outVolt(uint16_t contReg_V, uint32_t voltageOut)
{
  delay(2);
  digitalWrite(SS, LOW);
  SPI.transfer(control_r);
  SPI.transfer16(contReg_V);
  digitalWrite(SS, HIGH);
  delay(2);
  digitalWrite(SS, LOW);
  SPI.transfer(data_r);
  SPI.transfer16(voltageOut);
  digitalWrite(SS, HIGH);
  delay(2);
  digitalWrite(SS, LOW);
  SPI.transfer(readback_r);
  SPI.transfer16(read_status_r);
  digitalWrite(SS, HIGH);
  delay(2);
  digitalWrite(SS, LOW);
  SPI.transfer(nop8);
  SPI.transfer16(nop16);
  digitalWrite(SS, HIGH);
  delay(2);
}

void outCurr(uint16_t contReg_C, uint32_t currentOut)
{
  digitalWrite(SS, LOW);
  SPI.transfer(control_r);
  SPI.transfer16(contReg_C);
  digitalWrite(SS, HIGH);

  digitalWrite(SS, LOW);
  SPI.transfer(data_r);
  SPI.transfer16(currentOut);
  digitalWrite(SS, HIGH);

  digitalWrite(SS, LOW);
  SPI.transfer(readback_r);
  SPI.transfer16(read_status_r);
  digitalWrite(SS, HIGH);

  digitalWrite(SS, LOW);
  SPI.transfer(nop8);
  SPI.transfer16(nop16);
  digitalWrite(SS, HIGH);
}
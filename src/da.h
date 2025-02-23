#include <Arduino.h>
#include <SPI.h>

// 1.st 8-bit addresses word to choose operation 
const byte operation_write_data = 0x01;
const byte operation_read = 0x02;
const byte operation_set_mode = 0x55;
const byte operation_reset = 0x56; // reset all registers

/// nop command ///
const uint8_t nop_8 = 0x00;
const uint16_t nop_16 = 0x0000;

/// Bit words for reading from registers ///
const uint16_t read_status_register = 0x0000;
const uint16_t read_data_register = 0x0001;
const uint16_t read_control_register = 0x0002;

/// data to set OUTPUT ///
const uint16_t set_uni_voltage_mode = 0x2001;
const uint16_t set_bi_volage_mode = 0x2003;

/// data to set output  ///
// 4-20mA mode 0
// 0-20mA mode 1
// 0-24mA mode 2
const uint16_t set_current_mode[3] = { 0x2005, 0x2006, 0x2007 };

//for testing purposes
uint16_t MAX_current_value = 0xFFFF; //max current value
uint16_t MAX_voltage_value = 0xFFFF; //max voltage value

//value off-set
volatile float voltage_value = 03.39f;
volatile float current_value = 10.00f;

//voltage limits
const int minVoltage = -10;
const int maxVoltage = 10;

//current limits
const int minCurrent = 0;
const int maxCurrent = 24;

//HEX variables for AD-convertor
volatile uint32_t current_value_hex, voltage_value_hex;

void calculate_hex(void){

  //hex value for DA-convertor
  current_value_hex = (current_value * 0xFFFF) / 20;
  //volatile uint32_t voltage_value_hex = (voltage_value * 0xFFFF) / 20 + 0x7FFF; //UNI
  voltage_value_hex = ((voltage_value / 10 + 1) / 2) * 0xFFFF;  //BI

}

void AD_security_feature(void){

}

void signal_output(uint16_t mode_option,volatile uint32_t *signal_value){

  //1. is needed to set control register (operation)
  digitalWrite(SS, LOW);
  SPI.transfer(operation_set_mode);
  SPI.transfer16(mode_option);
  digitalWrite(SS, HIGH);
  delay(1);

  //2. is data what contain expected value
  digitalWrite(SS, LOW);
  SPI.transfer(operation_write_data);
  SPI.transfer16(*signal_value);
  digitalWrite(SS, HIGH);
  delay(1);
}

void read_register(uint16_t *_register){

  //choosing what register will be readed
  digitalWrite(SS, LOW);
  SPI.transfer(operation_read);
  SPI.transfer16(*_register);
  digitalWrite(SS, HIGH);
  delay(1);

  //24 bit blank word (0x000000) to obtain data from DA-convertor
  digitalWrite(SS, LOW);
  SPI.transfer(nop_8);
  SPI.transfer16(nop_16);
  digitalWrite(SS, HIGH);
  delay(1);
}

//BASIC SKELET

// delay(2);
// digitalWrite(SS, LOW);
// SPI.transfer(control_register);
// SPI.transfer16(mode_option);
// digitalWrite(SS, HIGH);
// delay(2);
// digitalWrite(SS, LOW);
// SPI.transfer(data_registe);
// SPI.transfer16(voltageOut);
// digitalWrite(SS, HIGH);
// delay(2);
// digitalWrite(SS, LOW);
// SPI.transfer(reead_back_register);
// SPI.transfer16(read_status_register);
// digitalWrite(SS, HIGH);
// delay(2);
// digitalWrite(SS, LOW);
// SPI.transfer(nop8);
// SPI.transfer16(nop16);
// digitalWrite(SS, HIGH);
// delay(2);
#include <Arduino.h>
#include <SPI.h>


// 1.st 8-bit addresses word to choose operation 
const byte operation_write_data = 0x01;
const byte operation_read = 0x02;
const byte operation_set_mode = 0x55;
const byte operation_reset = 0x56; // reset all registers

/// NOP VALUES ///
const uint8_t nop_8 = 0x00;
const uint16_t nop_16 = 0x0000;

/// Bit words for reading from registers ///
const uint16_t read_status_register = 0x0000;
const uint16_t read_data_register = 0x0001;
const uint16_t read_control_register = 0x0002;

/// VOLTAGES MODES (4 POSSIBLE RANGES) ///
const uint16_t mode_set_uni_voltage = 0x2001; //  0 +10 V
const uint16_t mode_set_bi_voltage = 0x2003;   //-10 +10 V

/// data to set output  ///
// 4-20mA mode 0
// 0-20mA mode 1
// 0-24mA mode 2
const uint16_t mode_set_current[3] = { 0x2005, 0x2006, 0x2007 };

//START VALUES 
volatile float voltage_value = 00.00f;
volatile float current_value = 00.00f;

//voltage limits
const int minVoltage = -10;
const int maxVoltage = 10;
                                  //!!! utility for change min and max values !!!!                  
//current limits
const int minCurrent = 0;
const int maxCurrent = 24;

//HEX variables for AD-convertor
volatile uint32_t current_value_hex, voltage_value_hex;

void calculate_hex(void){

  //hex value for DA-convertor
  current_value_hex = (current_value * 0xFFFF) / 24;                              //  0  24 mA
  //volatile uint32_t voltage_value_hex = (voltage_value * 0xFFFF) / 20 + 0x7FFF; //  0 +10 V
  voltage_value_hex = ((voltage_value / 10 + 1) / 2) * 0xFFFF;                    //-10 +10 V

}

void disable_output(void){

  digitalWrite(SS, LOW);
  SPI.transfer(operation_set_mode);
  SPI.transfer16(0x2000);
  digitalWrite(SS, HIGH);
  delay(1);

  Serial.println("DISABLED!");
}

void signal_output(uint16_t mode_option,volatile uint32_t *signal_value){
  
  calculate_hex(); 

  //for enable outpur via control register
  mode_option = mode_option | 0x1000;
  
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

  //for debbug
  Serial.println(mode_option, HEX);
  Serial.println(*signal_value, HEX);

  Serial.println(current_value);
  Serial.println(voltage_value);

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
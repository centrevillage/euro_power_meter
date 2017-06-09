#include <Wire.h>
#include <SPI.h>
#include <U8g2lib.h>
#include <U8x8lib.h>
#include <Adafruit_INA219.h>
#include <INA226.h>

U8X8_SSD1306_128X32_UNIVISION_HW_I2C u8x8;

Adafruit_INA219 ina219_1((uint8_t)0x40);
Adafruit_INA219 ina219_2((uint8_t)0x41);
INA226 ina226;

void setup() {
  u8x8.begin();
  ina219_1.begin();
  ina219_2.begin();
  ina226.begin((uint8_t)0x44);
  ina226.configure(INA226_AVERAGES_1, INA226_BUS_CONV_TIME_1100US, INA226_SHUNT_CONV_TIME_1100US, INA226_MODE_SHUNT_BUS_CONT);
  ina226.calibrate(0.025, 3.27);
}

void displayVoltage(int row, float voltage, int current_mA) {
  char line[16];
  char voltage_line[8];
  dtostrf(voltage, 5, 2, voltage_line);
  if (voltage_line[0] != '-' ) {
    sprintf(line, "+%sV %4dmA", voltage_line, current_mA);
  } else {
    sprintf(line, "%sV %4dmA", voltage_line, current_mA);
  }
  char line2[16];
  sprintf(line2, "%14s", line);
  u8x8.drawString(0, row, line2);
}

void displayVoltageINA219() {
  float busvoltage = ina219_1.getBusVoltage_V();
  int current_mA = (int)(ina219_1.getCurrent_mA());
  displayVoltage(0, busvoltage, current_mA);

  busvoltage = ina219_2.getBusVoltage_V();
  current_mA = (int)(ina219_2.getCurrent_mA());
  displayVoltage(1, busvoltage, current_mA);
}

void displayVoltageINA226() {
  float busvoltage = -ina226.readBusVoltage();
  int current_mA = (int)(ina226.readShuntCurrent() * 1000);
  displayVoltage(2, busvoltage, current_mA);
}

void loop() {
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  displayVoltageINA219();
  displayVoltageINA226();
  delay(1000);
}

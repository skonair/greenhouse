/* 
  RF24 Pin Mapping:
  GND -> GND on the Arduino
  VCC -> 3.3v on the Arduino
  CE -> PIN 9 on the Arduino
  CSN -> PIN 10 on the Arduino
  SCK -> PIN 13 on the Arduino Uno
  MOSI -> PIN 11 on the Arduino Uno
  MISO -> PIN 12 on the Arduino Uno
  IRQ -> not used

  TMP36 Pin Mapping:
  Left Pin (2.7 - 5.5V) -> 5v on the Arduino
  Middle Pin (Voltage out) -> PIN Analog 0 on the Arduino
  Right Pin (GND) -> GND on the Arduino
*/

#include <SPI.h>
#include <RF24.h>
#include <BTLE.h>

#define TMP36PIN 0                                                           // what digital pin we're connected to

RF24 radio(9, 10); // CE, CSN
BTLE btle(&radio);

void setup() {
  Serial.begin(9600);
  delay(1000);
  Serial.print("BLE and TMP36 Starting... ");
  Serial.println("Send Temperature Data over BTLE");
  btle.begin("Temp01");    // 8 chars max
  Serial.println("Successfully Started");
}

void loop() {                                              
  float temp = readTemperature();   //read temperature data
  if (isnan(temp)) {                                                // Check if any reads failed and exit early (to try again).
    Serial.println(F("Failed to read from TMP36 sensor!"));
    return;
  }
  
  nrf_service_data buf;
  buf.service_uuid = NRF_TEMPERATURE_SERVICE_UUID;
  buf.value = BTLE::to_nRF_Float(temp);

  if (!btle.advertise(0x16, &buf, sizeof(buf))) {
    Serial.println("BTLE advertisement failed..!");
  }
  
  btle.hopChannel(); 
  delay(10);
  Serial.print(".");
}

float readTemperature() {
  //getting the voltage reading from the temperature sensor
  int reading = analogRead(TMP36PIN);  
 
  // converting that reading to voltage, for 3.3v arduino use 3.3
  float voltage = reading * 5.0;
  voltage /= 1024.0; 
 
  // now compute the temperature
  float temperatureC = (voltage - 0.5) * 100 ;  //converting from 10 mv per degree wit 500 mV offset
                                               //to degrees ((voltage - 500mV) times 100)
                                               
  return temperatureC;  
}



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

  Humidity Sensor Pin Mapping:
  GND -> GND on the Arduino
  VCC -> 5v on the Arduino
  D0 -> not connected
  A0 -> PIN Analog 1 on the Arduino
  ...
*/

#include <SPI.h>
#include "RF24.h"
#include "printf.h"

#define TMP36PIN 0
#define HUMIDPIN 1

RF24 radio(9, 10); // CE, CSN

// addresses for the reading and writing pipes
byte addresses[][6] = {"1Node","2Node"};

struct message {
  float t1;
  float h1;
  unsigned long time;
};
typedef struct message Message;

void setup() {
  Serial.begin(9600);
  delay(500);
  Serial.print("RF24 and TMP36 starting... ");

  printf_begin();

  radio.begin();
  radio.setPALevel(RF24_PA_LOW);  // default is RF24_PA_MAX
  radio.openWritingPipe(addresses[0]);
  radio.openReadingPipe(1,addresses[1]);
  radio.setAutoAck(false);

  radio.printDetails();
}

void loop() {
  float temperature = readTemperature();
  float humidity = readHumidity();

  Message msg;
  msg.t1 = temperature;
  msg.h1 = humidity;
  msg.time = micros();

  Serial.print("msg.t1: ");
  Serial.print(msg.t1);
  Serial.print(" msg.h1: ");
  Serial.print(msg.h1);
  Serial.println(" ... now sending");
  
  if (!radio.write( &msg, sizeof(msg) )){
    Serial.println("Sending failed");
  }

  delay(1000);
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

float readHumidity() {
      //getting the voltage reading from the humidity sensor
      int reading = analogRead(HUMIDPIN);

      // get a percentage (dry = 0%, wet = 100%)
      float humidity = (1024 - reading) / 1024.0;

      return humidity;
}

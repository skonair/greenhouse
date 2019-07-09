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

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include "RF24.h"
#include "printf.h"

#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme; // I2C

#define TMP36PIN 0
#define HUMIDPIN 1
#define BRIGHTPIN 2

RF24 radio(9, 10); // CE, CSN

// addresses for the reading and writing pipes
byte addresses[][6] = {"1Node","2Node"};

struct bmedata {
  float temperature;
  float pressure;
  float altitude;
  float humidity;
};
typedef struct bmedata BmeData;

struct message {
  float temperature;
  float humidity;
  float brightness;
  float bme_temperature;
  float bme_pressure;
  float bme_altitude;
  float bme_humidity;
  unsigned long time;
};
typedef struct message Message;

Message msg;

void setup() {
  Serial.begin(9600);
  delay(500);
  Serial.print("RF24 and TMP36 starting... ");

  printf_begin();

    // default settings
    // (you can also pass in a Wire library object like &Wire2)
    unsigned status = bme.begin();  
    if (!status) {
        Serial.println("Could not find a valid BME280 sensor, check wiring, address, sensor ID!");
        Serial.print("SensorID was: 0x"); Serial.println(bme.sensorID(),16);
        Serial.print("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
        Serial.print("   ID of 0x56-0x58 represents a BMP 280,\n");
        Serial.print("        ID of 0x60 represents a BME 280.\n");
        Serial.print("        ID of 0x61 represents a BME 680.\n");
        while (1);
    }

  radio.begin();
  radio.setPALevel(RF24_PA_LOW);  // default is RF24_PA_MAX
  radio.openWritingPipe(addresses[0]);
  radio.openReadingPipe(1,addresses[1]);
  radio.setAutoAck(true);
  radio.printDetails();
  
  radio.startListening();
}

void readSensors(Message *msg) {
  float temperature = readTemperature();
  float humidity = readHumidity();
  float brightness = readBrightness();
  BmeData bmedata = readBmeData();

  msg->temperature = temperature;
  msg->humidity = humidity;
  msg->brightness = brightness;
  msg->bme_temperature = bmedata.temperature;
  msg->bme_pressure = bmedata.pressure;
  msg->bme_altitude = bmedata.altitude;
  msg->bme_humidity = bmedata.humidity;
  msg->time = micros();
}

void printMessage(Message *msg) {
  Serial.print("msg.temperature: ");
  Serial.print(msg->temperature);
  Serial.print(" msg.humidity: ");
  Serial.print(msg->humidity);
  Serial.print(" msg.brightness: ");
  Serial.print(msg->brightness);
  Serial.print(" msg.bme_temperature: ");
  Serial.print(msg->bme_temperature);
  Serial.print(" msg.bme_pressure: ");
  Serial.print(msg->bme_pressure);
  Serial.print(" msg.bme_altitude: ");
  Serial.print(msg->bme_altitude);
  Serial.print(" msg.bme_humidity: ");
  Serial.print(msg->bme_humidity);
  Serial.println(" ... now sending");
}


void loop() {

  int i = 1;
  if (radio.available()) {
    radio.read(i, sizeof(&i));
    Serial.print("read: "); Serial.println(i);

    readSensors(&msg);
    printMessage(&msg);

    radio.stopListening();
    if (!radio.write( &msg, sizeof(msg) )){
      Serial.println("Sending failed");
    }
    radio.startListening();

   
  }

  delay(500);
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

float readBrightness() {
  int reading = analogRead(BRIGHTPIN);
  float brightness = (1024 - reading) / 1024.0;

  return brightness;
}

BmeData readBmeData() {
  BmeData bmedata;
  bmedata.temperature = bme.readTemperature();
  bmedata.pressure = bme.readPressure() / 100.0F;
  bmedata.altitude = bme.readAltitude(SEALEVELPRESSURE_HPA);
  bmedata.humidity = bme.readHumidity();
  return bmedata;
}

void printValues() {
    Serial.print("Temperature = ");
    Serial.print(bme.readTemperature());
    Serial.println(" *C");

    Serial.print("Pressure = ");

    Serial.print(bme.readPressure() / 100.0F);
    Serial.println(" hPa");

    Serial.print("Approx. Altitude = ");
    Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
    Serial.println(" m");

    Serial.print("Humidity = ");
    Serial.print(bme.readHumidity());
    Serial.println(" %");

    Serial.println();
}

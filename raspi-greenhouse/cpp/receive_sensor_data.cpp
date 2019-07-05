#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <stdio.h>
#include <RF24/RF24.h>

using namespace std;

RF24 radio(RPI_V2_GPIO_P1_22, RPI_V2_GPIO_P1_24, BCM2835_SPI_SPEED_8MHZ);

const uint8_t addresses[][6] = {"1Node","2Node"};

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


// Get current date/time, format is YYYY-MM-DD.HH:mm:ss
const std::string currentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    //tstruct = *localtime(&now);
    tstruct = *gmtime(&now);
    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);

    return buf;
}

int main(int argc, char** argv) {
  radio.begin();
  radio.setAutoAck(false);
  radio.setRetries(15,15);
  radio.setPALevel(RF24_PA_HIGH); // default is RF24_PA_MAX

  radio.openWritingPipe(addresses[1]);
  radio.openReadingPipe(1,addresses[0]);

  radio.startListening();

//  radio.printDetails();

  while (1) {
    // if there is data ready
    if (radio.available()) {

      Message msg;
      while(radio.available()){
        radio.read(&msg, sizeof(msg));
      }

      string sdate = currentDateTime();
      printf("%s temperature %.2f humidity %.2f brightness %.2f bmetemperature %.2f bmepressure %.2f bmealtitude %.2f bmehumidity %.2f\n", 
          sdate.c_str(), msg.temperature, msg.humidity, msg.brightness, msg.bme_temperature, msg.bme_pressure, msg.bme_altitude, msg.bme_humidity);

      delay(500);
    }
  }

  return 0;
}

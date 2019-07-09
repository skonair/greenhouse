#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <stdio.h>
#include <RF24/RF24.h>
#include "./includes/httplib.h"


using namespace std;
using namespace httplib;

RF24 radio(RPI_V2_GPIO_P1_22, RPI_V2_GPIO_P1_24, BCM2835_SPI_SPEED_8MHZ);

const uint8_t addresses[][6] = {"1Node","2Node"};
const int i42 = 0x42;

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

void initRadio() {
  radio.begin();
  radio.setAutoAck(true);
  radio.setRetries(15,15);
  radio.setPALevel(RF24_PA_HIGH); // default is RF24_PA_MAX

  radio.openWritingPipe(addresses[1]);
  radio.openReadingPipe(1,addresses[0]);

  radio.startListening();

  radio.printDetails();
}

int readMessage(int retries, int delay, Message *msg) {
  // send a ping request to the arduino
  radio.stopListening();
  radio.write(&i42,sizeof(i42));

  radio.startListening();
  while (retries > 0) {
    retries--;
    // if there is data ready
    if (radio.available()) {
      // Message msg;
      while(radio.available()) {
        radio.read(msg, sizeof(*msg));
      }
     return 0;
    } 
    delay(delay);
  }
  return 1;
}

int readNextLine(char **line) {
    Message msg;
    int result = readMessage(3, 500, &msg);
    if (result == 0) {
      // sprintf(*line, "%s temperature %.2f humidity %.2f brightness %.2f bmetemperature %.2f bmepressure %.2f bmealtitude %.2f bmehumidity %.2f", 

      sprintf(*line, "# TYPE greenhouse_temperature gauge\ngreenhouse_temperature %.10e \n\
# TYPE greenhouse_humidity gauge\ngreenhouse_humidity %.10e \n\
# TYPE greenhouse_brightness gauge\ngreenhouse_brightness %.10e \n\
# TYPE greenhouse_bme_temperature gauge\ngreenhouse_bme_temperature %.10e \n\
# TYPE greenhouse_bme_pressure gauge\ngreenhouse_bme_pressure %.10e \n\
# TYPE greenhouse_bme_altitude gauge\ngreenhouse_bme_altitude %.10e \n\
# TYPE greenhouse_bme_humidity gauge\ngreenhouse_bme_humidity %.10e \n",
          msg.temperature, msg.humidity, msg.brightness, msg.bme_temperature, msg.bme_pressure, msg.bme_altitude, msg.bme_humidity);
    }
    return result;
}

int main(int argc, char** argv) {
  initRadio();

  Server svr;

  svr.Get("/metrics", [](const Request & /* req */, Response &res) {
    char *line = new char[4096];
    int result = readNextLine(&line);
    printf("Result is %d and line is %s\n", result, line);
    if (result == 0) {
      res.set_content(line, "text/plain");
    }
  });

  svr.listen("localhost", 9999);

  return 0;
}

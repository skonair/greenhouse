#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <RF24/RF24.h>

using namespace std;

RF24 radio(RPI_V2_GPIO_P1_22, BCM2835_SPI_CS0);

const uint8_t addresses[][6] = {"1Node","2Node"};

struct message {
  float t1,
  float h1,
  unsigned long time
};
typedef struct message Message;


int main(int argc, char** argv) {
  radio.begin();
  radio.setRetries(15,15);
  radio.printDetails();

  radio.openWritingPipe(addresses[1]);
  radio.openReadingPipe(1,addresses[0]);

  radio.startListening();

  while (1) {
    // if there is data ready
    if (radio.available()) {

      Message msg;
      while(radio.available()){
        radio.read(&msg, sizeof(msg));
      }

      printf("Got payload(t1: %f, h1: %f) %lu...\n", msg.t1, msg.h1, msg.time);

      delay(925);
    }
  }

  return 0;
}
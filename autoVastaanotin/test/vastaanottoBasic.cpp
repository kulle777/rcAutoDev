#include <Arduino.h>
#include "RF24.h"
#include <RF24Network.h>

RF24 radio(7, 8);  // using pin 7 for the CE pin, and pin 8 for the CSN pin

RF24Network network(radio);
const uint16_t this_node = 00;    // Address of our node in Octal format ( 04,031, etc)

struct payloadType {
  uint16_t nopeusPWM = 0;
  uint16_t suunta = 0;
  bool horn = 0;
  bool eteen = 1;
  bool lights = 1;
};

payloadType payload;

void setup() {
  Serial.begin(9600);
  while (!Serial) {};

  if (!radio.begin()) {
    Serial.println(F("radio hardware is not responding!!"));
  }
  network.begin(/*channel*/ 90,/*node address*/ this_node);
}


void loop() {
  network.update();

    while (network.available()) {                     // Is there anything ready for us?
      RF24NetworkHeader header;                       // If so, grab it and print it out
      network.read(header, &payload, sizeof(payloadType));    // Read data to global payload variable

      Serial.print("nopeus PWM: ");
      Serial.print(payload.nopeusPWM);
    }
}
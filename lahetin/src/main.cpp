#include <Arduino.h>
#include "RF24.h"
#include <RF24Network.h>
//NRF24 wifi moduuli + base module (eli regu ja cap)

RF24 radio(7, 8);  // using pin 7 for the CE pin, and pin 8 for the CSN pin

RF24Network network(radio);     // Network uses that radio
const uint16_t this_node = 00;  // Address of our node in Octal format (04, 031, etc)
const uint16_t other_node = 01; // Address of the other node in Octal format


unsigned long timer;                    // start the timer    rollover at 49 days
uint8_t address[6] = {"2Node"};

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
  while (!Serial) {
    // some boards need to wait to ensure access to serial over USB
  }

  if (!radio.begin()) {                                         // initialize the transceiver on the SPI bus
    Serial.println(F("radio hardware is not responding!!"));
    while (1) {}
  }
  radio.setPALevel(RF24_PA_LOW);                                // RF24_PA_MAX is default.
  radio.setPayloadSize(sizeof(payloadType));
  radio.openWritingPipe(address);
  radio.stopListening();
  radio.setDataRate(RF24_250KBPS);
  timer = millis();
}

void loop() {
  //TODO: take inputs and construct a payload for transmission



  //transmit
  bool transmissionSucceeded = radio.write(&payload,sizeof(payloadType));
  if(!transmissionSucceeded){
    Serial.println("could not send payload");
  }else{
    Serial.println("Payload sent!");
  }
}
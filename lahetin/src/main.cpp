#include <Arduino.h>
#include <SPI.h>
#include "RF24.h"
//NRF24 wifi moduuli + base module (eli regu ja cap)

RF24 radio(7, 8);  // using pin 7 for the CE pin, and pin 8 for the CSN pin

uint8_t address[6] = {"1Node"};

struct payloadType {
  bool eteen = 1;
  uint16_t nopeus = 0;
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
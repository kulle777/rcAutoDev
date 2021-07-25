#include <Arduino.h>
#include "RF24.h"
#include <RF24Network.h>

RF24 radio(7, 8);  // using pin 7 for the CE pin, and pin 8 for the CSN pin
RF24Network network(radio);

const uint16_t this_node = 01;
const uint16_t other_node = 00;

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
  while (!Serial) {}    //wait for serial

  if (!radio.begin()) {                                         // initialize the transceiver on the SPI bus
    Serial.println(F("radio hardware is not responding!!"));
  }
  
  network.begin(/*channel*/ 90, /*node address*/ this_node);    // channel is same for both. address is individual
}

void loop() {
    payload.nopeusPWM += 1;
    network.update();
  
    RF24NetworkHeader header(/*to node*/ other_node);     //vastaanottaja kohde valitaan sen addressilla
    bool ok = network.write(header,&payload,sizeof(payloadType));
    if(!ok){
        Serial.println("could not send payload");
    }else{
        Serial.println("Payload sent with nopeusPWM = ");
        Serial.println(payload.nopeusPWM);
    }
}
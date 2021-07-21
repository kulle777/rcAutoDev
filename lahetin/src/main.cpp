#include <Arduino.h>
#include "RF24.h"
#include <RF24Network.h>
//NRF24 wifi moduuli + base module (eli regu ja cap)

RF24 radio(7, 8);  // using pin 7 for the CE pin, and pin 8 for the CSN pin

RF24Network network(radio);
const uint16_t this_node = 01;
const uint16_t other_node = 00;

unsigned long timer;                        // start the timer    rollover at 49 days
const unsigned long timeInterval = 100;    // how often we send message
unsigned long last_sent = 0;

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
  }
  radio.setPALevel(RF24_PA_LOW);                                // RF24_PA_MAX is default.
  radio.setPayloadSize(sizeof(payloadType));
  radio.setDataRate(RF24_250KBPS);
  network.begin(/*channel*/ 90, /*node address*/ this_node);    // channel is same for both, nut address is individual
  timer = millis();
}

void loop() {
  delay(1);
  payload.nopeusPWM += 1;
  network.update();
  //TODO: take inputs and construct a payload for transmission

  //nappuloilla voidaan luoda keskiarvottava signaali, jolloin lähetettävä kontrollisignaali ei menis suoraan nappulan mukaan.
  // tee niin, etta eteen -> plussaa ja taakse -> miinusta = integroi, eikä tee akillisia





  //transmit at regular intervals (timeInterval in milliseconds)
  timer = millis();
  if(timer - last_sent > timeInterval){
    last_sent = timer;

    RF24NetworkHeader header(/*to node*/ other_node);     //vastaanottaja kohde valitaan sen addressilla
    bool ok = network.write(header,&payload,sizeof(payloadType));
    if(!ok){
      Serial.println("could not send payload");
    }else{
      Serial.println("Payload sent!");
    }
  }
}
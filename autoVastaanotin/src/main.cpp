#include <Arduino.h>
#include <SPI.h>
#include <PID_v1.h>
#include "RF24.h"

/*
//    PID saadin demokoodia
//Define Variables we'll be connecting to
double Setpoint, Input, Output;
//Specify the links and initial tuning parameters
double Kp=2, Ki=5, Kd=1;
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);
*/

RF24 radio(7, 8);  // using pin 7 for the CE pin, and pin 8 for the CSN pin

uint8_t address[6] = {"1Node"};   //kayttaa charactereja integereina. Toimii koska kumpikin vie 8 bittia
//  WIFI kayttaa kahta asiaa:
//    address ja pipe. Kun ne on oikein, niin kommunikaatio pelaa

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

  if (!radio.begin()) {
    Serial.println(F("radio hardware is not responding!!"));
    while (1) {}
  }
  radio.setPALevel(RF24_PA_LOW);
  radio.setPayloadSize(sizeof(payloadType));
  radio.openReadingPipe(1,address);
  radio.startListening();
}

void loop() {
  uint8_t pipe;
    if (radio.available(&pipe)) {             // is there a payload? get the pipe number that recieved it
      uint8_t bytes = radio.getPayloadSize();
      radio.read(&payload, bytes);            // fetch payload from FIFO (inside memory of wifi chip)
      Serial.print(F("Received "));
      Serial.print(bytes);                    // print the size of the payload
      Serial.print(F(" bytes on pipe "));
      Serial.print(pipe);                     // print the pipe number
      Serial.print(F(": "));
      Serial.println(payload.eteen);                // print the payload's value
      Serial.println(payload.nopeus);
    }

    //saatu viesti on nyt payload muuttujassa tallessa
    //TODO: kontrolloi moottoria


}
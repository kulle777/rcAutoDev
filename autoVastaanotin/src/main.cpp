/*
#include <Servo.h>

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position

void setup() {
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
}
*/

//testeissä ohjausservo menee enemman oikealle auton takaa katsottuna.
//Servossa on luultavasti offsettia.
//servossa myos reuna-arvot, joita ei voi fyysisesti ylittaa
// suuntakulman arvot 60-120 toimivat ilman etta osuu reunoihin
//servon suuntakulman arvot 0-90 ovat auton takaa katsottuna OIKEALLE -> 90-180 on vasemmalle.

//NAPPI: ei saa olla yhteytta jos haluaa kayttaa ulkoista poweria.
//jos kytketaan 2 johtoa yhteen napilla -> kayttaa kontrollilogiikalta saatavaa poweria (5v)

//wifi toimii perus sketsilla, kun kaytetaan fyysisia yhteyksia taman sivu mukaan:
//https://howtomechatronics.com/tutorials/arduino/arduino-wireless-communication-nrf24l01-tutorial/

#include <Arduino.h>
#include <PID_v1.h>
#include "RF24.h"
#include <RF24Network.h>
#include <Servo.h>
/*
//    PID saadin demokoodia
//Define Variables we'll be connecting to
double Setpoint, Input, Output;
//Specify the links and initial tuning parameters
double Kp=2, Ki=5, Kd=1;
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);
*/

//Servo myServo;
const int currentMeasurePin = A0;

RF24 radio(7, 8);  // using pin 7 for the CE pin, and pin 8 for the CSN pin

RF24Network network(radio);
const uint16_t this_node = 00;    // Address of our node in Octal format ( 04,031, etc)
//  WIFI kayttaa kahta asiaa:
//    address ja pipe. Kun ne on oikein, niin kommunikaatio pelaa

struct payloadType {
  uint16_t nopeusPWM = 0;
  uint16_t suunta = 0;
  bool horn = 0;
  bool eteen = 1;
  bool lights = 1;
};

payloadType payload;

unsigned long lastReceived;
const unsigned long maxTimeBeforeTimeOut = 3000;

unsigned long safetyTime;
/*safetyTime muuttujaa kaytetaan moottorikontrolloinnin if lauseessa.
Jos safety on laitettu hetkeksi paalle, niin moottori ei anna ohjata vaan pysahtyy.
safety laitetaan paalle muuttamalla safetytime tulevaisuuden arvoksi millis + 1000. numero kertoo safetyn maaran.
*/

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    // some boards need to wait to ensure access to serial over USB
  }

  if (!radio.begin()) {
    Serial.println(F("radio hardware is not responding!!"));
  }
  radio.setPALevel(RF24_PA_LOW);
  radio.setDataRate(RF24_250KBPS);
  radio.setPayloadSize(sizeof(payloadType));
  network.begin(/*channel*/ 90,/*node address*/ this_node);
  //myServo.attach(9);
}

// Variable for calculating how long between RX
uint32_t timeBetweenPackets = 0;

void loop() {
  network.update();

    while (network.available()) {                     // Is there anything ready for us?
      RF24NetworkHeader header;                       // If so, grab it and print it out
      lastReceived = millis();

      //TODO: testaa toimiiko jos vaan sanon, etta payloadSize = sizeof(payloadType)
      //uint16_t payloadSize = network.peek(header);    // Use peek() to get the size of the payload
      network.read(header, &payload, sizeof(payloadType));    // Read data to global payload variable

      Serial.print("Received packet, id: ");         // Print info about received data
      Serial.print(header.id);
      Serial.print("(");
      Serial.print(millis() - timeBetweenPackets);
      Serial.println("ms since last)");
      timeBetweenPackets = millis();
      Serial.print("nopeus PWM: ");
      Serial.print(payload.nopeusPWM);
    }

    //saatu viesti on nyt dataBuffer muuttujassa tallessa


  if(!(millis()-lastReceived>maxTimeBeforeTimeOut) && (safetyTime < millis())){    //jos ei ole kulunut yli maksimiaikaa viime lahetyksen vastaanotosta ja safety ei ole paalla
    //TODO: kontrolloi moottoria


    //TODO: analog read -> jos virtaa kulkee liikaa -> nollaa moottoriohjaus 100ms :ksi.
    if(analogRead(currentMeasurePin)>102){          //0v -- 5v => 0 -- 1023. 4,88mV per numero. => arvo 102 = 500mV
      //TODO: nollaa moottoriohjaus
    }
  }else{
    //TODO: kerro moottorille etta pitaa pysahtya. Viesteja ei olla saatu
  }
}
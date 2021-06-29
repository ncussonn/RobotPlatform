#include <SPI.h>
//#include <nRF24L01.h>  
#include "RF24.h"

// INPUTS
float     nJoyX;              // Joystick X input                     (-128..+127)
float     nJoyY;              // Joystick Y input                     (-128..+127)
int       select_state;

// Joystick Definitions:
#define joyX A5
#define joyY A4

// CE and CSN pins
const int CE_pin = 7;
const int CSN_pin = 8;

// Select Button
const int SEL = 5;    // Select Button

RF24 myRadio (CE_pin, CSN_pin);  //(CE_pin, CSN_pin)
byte addresses[][6] = {"0"};

struct Joystick {
  int id=1;
  float nJoyX = 0;  //default X position
  float nJoyY = 0;  //defualt Y position
  int button = 0;   //default button position
  char  text[100] = "Text to be transmitted";
};

typedef struct Joystick datapack;
datapack jdata;

void setup()
{
  Serial.begin(115200);
  pinMode(SEL, INPUT_PULLUP); //set select input pin in pullup mode
  delay(1000);
  
  myRadio.begin();  
  myRadio.setChannel(115); 
  myRadio.setPALevel(RF24_PA_MAX);
  myRadio.setDataRate( RF24_250KBPS ) ; 
  myRadio.openWritingPipe( addresses[0]);
  delay(1000);
}

void loop()
{
  myRadio.write(&jdata, sizeof(jdata)); 

  Serial.print("\nPackage:");
  Serial.print(jdata.id);       //print data pack number (check for dropped packages)
  Serial.print("\n");
  Serial.println(jdata.nJoyX);  //print the X-position
  Serial.println(jdata.nJoyY);  //print the Y-position
  Serial.println(jdata.button); //print the Y-position
  Serial.println(jdata.text);   //print the package text  
  
  jdata.id = jdata.id + 1;      //iterating to next package
  jdata.nJoyX = analogRead(joyX); //Read x position of joystick and assign value to struct
  jdata.nJoyY = analogRead(joyY); //Read y position of joystick and assign value to struct
  jdata.button = digitalRead(SEL); //Read select button position of joystick and assign value to struct
  delay(10);
}

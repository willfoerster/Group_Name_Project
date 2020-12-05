/*
  Serial data transmitter test

  Test the transmission of data from an arduino to the MSP432

  Written by: Ray Richter
*/

// Switch out the commented line below or change the baud rate if it is different from the MSP432
//#define BAUD_RATE 9600
#define BAUD_RATE 115200

// This is the array that the received string is stored
char inBuffer[32];

void setup() {
  // start serial port at BAUD_RATE bps:
  Serial.begin(BAUD_RATE);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  Serial.write("ON");
  delay(1000);
  Serial.write("OFF");
  delay(1000);
}

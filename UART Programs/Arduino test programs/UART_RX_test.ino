/*
  Serial data receiver test

  Test the transmission of data from a raspberry pi to an arduino

  Written by: Ray Richter
*/

// Switch out the commented line below or change the baud rate if it is different from the Raspberry Pi
//#define BAUD_RATE 9600
#define BAUD_RATE 115200
//#define LED_BUILTIN 2

// This is the array that the received string is stored
char inBuffer[32];

void setup() {
  // start serial port at BAUD_RATE bps:
  Serial.begin(BAUD_RATE);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  if(Serial.available() != 0){
    handle_input();
  }
  // Data can be sent to the Raspberry Pi bu using:
  // Serial.write("insert a string here");
}

void handle_input(void){
  // Create a received length counter
  int i = 0;
  while(Serial.available() > 0) {
    // Store incoming byte
    inBuffer[i] = Serial.read();
    // Incriment counter
    i++;
  }
  Serial.write("Echo: ");
  Serial.write(inBuffer);
  Serial.write('\n');
  // If the second char is n or N and the length is 2, turn on the LED
  if((inBuffer[1] == 'n' || inBuffer[1] == 'N')){
    digitalWrite(LED_BUILTIN, HIGH);
  }
  // If the second char is f or F and the length is 3, turn off the LED
  if((inBuffer[1] == 'f' || inBuffer[1] == 'F')){
    digitalWrite(LED_BUILTIN, LOW);
  }
  // String comparison also works, but in the interest of time this is what I put together to test
  // UART on a Raspberry Pi
}

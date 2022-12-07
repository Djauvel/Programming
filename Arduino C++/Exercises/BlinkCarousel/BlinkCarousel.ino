int pinYellow = 13; //Definerer gult lys som pin 13 HIGH
int pinGreen = 30; //Definerer grønt lys som pin 30 LOW
int pinRed = 17; //Definerer rødt lys som pin 17 LOW
int pinIRL = 5; //Definerer IR lys i venstre side som pin 5 LOW
int pinIRR = 19; // Definerer IR lys i venstre side som pin 19 LOW

#include <Wire.h>
#include <Zumo32U4.h>

Zumo32U4Buzzer buzzer; 

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pins as outputs.
  pinMode(pinYellow, OUTPUT); 
  pinMode(pinGreen, OUTPUT);
  pinMode(pinRed, OUTPUT);
  pinMode(pinIRL, OUTPUT);
  pinMode(pinIRR, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(pinYellow, HIGH); //Code chunk lights and turns off yellow light for 200 ms followed by 200ms delay.
  delay(200);
  digitalWrite(pinYellow, LOW);
  delay(200);

  digitalWrite(pinGreen, LOW); //Code chunk lights and turns off green light for same interval
  delay(200);
  digitalWrite(pinGreen, HIGH);
  delay(200);


  digitalWrite(pinRed, LOW); //Code chunk lights and turns off red light for same interval
  delay(200);
  digitalWrite(pinRed, HIGH);
  delay(200);

  
  digitalWrite(pinIRL, HIGH); //Code chunk lights and turns off right IR light for same interval using two pins for control
  digitalWrite(pinIRR, HIGH);
  delay(200);
  digitalWrite(pinIRL, LOW);
  delay(200);

  digitalWrite(pinIRR, LOW); //Code chunk lights and turns off IR light for same interval using two pins for control
  digitalWrite(pinIRL, HIGH);
  delay(200);
  digitalWrite(pinIRL, LOW);
}

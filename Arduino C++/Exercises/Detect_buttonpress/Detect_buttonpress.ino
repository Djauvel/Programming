int buttonA = 14; // pin for button A
int buttonOn= 0;
int Yellow = 13;

void setup() {
  pinMode(buttonA,INPUT);
  Serial.begin(9600);
  pinMode(Yellow, OUTPUT);
}

void loop() {
  buttonOn = digitalRead(buttonA); // button is pressed when is read a 0
  while (buttonOn == 0){
    buttonOn = digitalRead(buttonA);
    digitalWrite(Yellow, HIGH);
  }
  digitalWrite(Yellow, LOW);
}

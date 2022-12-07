char c;
int Yellow = 13;
int i = 0;

void setup() {
Serial.begin(9600);
pinMode(Yellow, OUTPUT);
}

void loop() {
  digitalWrite(Yellow, HIGH); //Turns on idle LED
  
  while(Serial.available() > 0) { //While there is data in the buffer, execute:
    digitalWrite(Yellow, LOW); //Turns off idle LED
    delay(3); // wait for the serial data to fill the buffer
    c = Serial.read(); //read next byte in the buffer.
    Serial.println(c); // echo the received char
    delay(200);
  }
} //While the buffer is empty (idle) then the yellow LED will be on. While there is data in the buffer the LED turns off

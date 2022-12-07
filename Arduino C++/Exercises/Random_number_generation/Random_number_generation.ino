long randNumber;
int pin13 = 13;

void setup() {
  // put your setup code here, to run once:
 randomSeed(analogRead(0));
 pinMode(pin13, OUTPUT);
 Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  randNumber = random(200, 10001);
  Serial.println(randNumber);
  
  digitalWrite(pin13, HIGH);
  delay(randNumber);
  digitalWrite(pin13, LOW);
  delay(2000);
}

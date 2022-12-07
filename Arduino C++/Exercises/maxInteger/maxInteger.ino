int maxIter = 2;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  Serial.println("Max Iter = " + (String)maxIter);
  for(int i ) 0; i<maxIter; i++){
    Serial.println("Im in the iter = " + (String)i);
    delay(1500);
    }
    maxIter = maxiter + 2;
}

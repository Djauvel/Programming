int myGlobalCounter = 0;
void increment(int inc, int &counter){
  counter = counter + inc;
  
  Serial.println(counter);
}

void setup() {
  // put your setup code here, to run once:
 Serial.begin(9600);
}

void loop() {
  for(int i=0; i<5;i++){
    increment(3, myGlobalCounter);
   delay(1000);
  }
  Serial.println("----New Loop----");
  delay(2000);
}

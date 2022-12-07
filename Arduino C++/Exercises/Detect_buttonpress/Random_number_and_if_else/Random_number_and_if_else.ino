long randNumber;
int nIteration = 0;
int overSixty = 0;
int underThirty = 0;
int other = 0;
String fullString;
void setup() {
  // put your setup code here, to run once:
 randomSeed(analogRead(0));
 Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  randNumber = random(0, 101);
  delay(500);
  nIteration++; //nIteration = nIteration + 1 - Counts the number of generated values

  if (randNumber > 60) //if the random number is greater than 60
  {
   overSixty++; // add 1 to overSixty = 0
  }
  else if (randNumber < 30) //if the random number is lower than 30
  {
    underThirty++; //add 1 to underThirty = 0
   }
   
   //else //if none of these statements are true
   //{
   //other++; //add 1 to other = 0
   //}
   
  if((nIteration%6) ==1 ) { // makes the program calculate the "other" variable and print the result line every sixth iteration. So that we save CPU power
    other = nIteration - underThirty - overSixty; //alternative method for calculating other
  Serial.println((String)"Number of values read = " + nIteration + "; " + "Higher than 60 = " + overSixty + "; " + "Lower than 30 = " + underThirty + "; " + "Rest = " + other);  
  }
}

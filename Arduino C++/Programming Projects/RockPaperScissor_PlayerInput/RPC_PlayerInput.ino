long randNumber1;
int userInput;
#include <Wire.h>
#include <Zumo32U4.h>

String CPU1Choice; String playerChoice;
String CPU1 = "CPU 1 Has Chosen: ";
String userChoice = "Player Has Chosen: ";
Zumo32U4ButtonA buttonA;
Zumo32U4ButtonB buttonB;
Zumo32U4ButtonC buttonC;

void setup() {// put your setup code here, to run once:
  randomSeed(20);
  Serial.begin(9600);
}

void loop() {  // put your main code here, to run repeatedly:
  Serial.println("Press A to begin.");
  delay(20);
  buttonA.waitForPress();
  Serial.println("Use onboard buttons to choose:");
  Serial.println("Press A for Rock, Press B for Paper, Press C for Scissors.");
  delay(500);
  
  while (userInput < 1){
  if (buttonA.isPressed()){
    userInput = 1;}
  else if (buttonB.isPressed()){
      userInput = 2;}      
  else if(buttonC.isPressed()){
        userInput = 3;
        }
  }
  delay(200);
        
  randNumber1 = random(1,4);
  switch(randNumber1){
    case 1:
      CPU1Choice = "Rock";
      break;
    case 2:
      CPU1Choice = "Paper";
      break;
    default:
      CPU1Choice = "Scissors";
      break;
    }
    Serial.println(CPU1 + CPU1Choice);

    switch(userInput){
    case 1:
      playerChoice = "Rock";
      break;
    case 2:
      playerChoice = "Paper";
      break;
    default:
      playerChoice = "Scissors";
      break;
    }
   Serial.println(userChoice + playerChoice); 

   if(randNumber1 == userInput){                        // If CPU's choose the same option, Stalemate
    Serial.println("Stalemate!");
    }else if (randNumber1 == 1 && userInput == 3){      // If CPU 1 is Rock, and CPU 2 is scissor, CPU 1 wins (Because of (1))
          Serial.println("CPU wins!");
        }else if (userInput == 1 && randNumber1 == 3){  // If CPU 2 is Rock, and CPU 2 is scissor, CPU 2 wins (Because of (1))
          Serial.println("Player wins!");
            }else if(randNumber1 < userInput){          // Because of the increment of power, 1, 2, 3 = Rock, paper, scissors, then the greater number wins, except when one is rock and one scissors
              Serial.println("Player wins!");
                }else{
                   Serial.println("CPU wins!");
                      } 
 userInput = 0;
}

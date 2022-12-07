long randNumber1;
long randNumber2;
String CPU1Choice; String CPU2Choice;
String CPU1 = "CPU 1 Has Chosen: ";
String CPU2 = "CPU 2 Has Chosen: ";

#include <Wire.h>
#include <Zumo32U4.h>

Zumo32U4ButtonA ButtonA;

void setup() {
  randomSeed(20);
  Serial.begin(9600);
}

void loop() {
  ButtonA.waitForPress();
    delay(500);
  randNumber1 = random(1,4);
  randNumber2 = random(1,4);

  switch(randNumber1){
    case 1:
      CPU1Choice = "Rock";
      break;
    case 2:
      CPU1Choice = "Paper";
      break;
    default:
      CPU1Choice = "Scissors";
    }
    Serial.println(CPU1 + CPU1Choice);

    switch(randNumber2){
      case 1:
        CPU2Choice = "Rock";
        break;
      case 2:
        CPU2Choice = "Paper";
        break;
      default:
        CPU2Choice = "Scissors";
    }
   Serial.println(CPU2 + CPU2Choice); 

   if(randNumber1 == randNumber2){                        // If CPU's choose the same option, Stalemate
    Serial.println("Stalemate!");
    }else if (randNumber1 == 1 && randNumber2 == 3){      // If CPU 1 is Rock, and CPU 2 is scissor, CPU 1 wins (Because of (1))
          Serial.println("CPU 1 wins!");
        }else if (randNumber2 == 1 && randNumber1 == 3){  // If CPU 2 is Rock, and CPU 2 is scissor, CPU 2 wins (Because of (1))
          Serial.println("CPU 2 wins!");
            }else if(randNumber1 < randNumber2){          // Because of the increment of power, 1, 2, 3 = Rock, paper, scissors, then the greater number wins, except when one is rock and one scissors
              Serial.println("CPU 2 wins!");
                }else{
                   Serial.println("CPU 1 wins!");
                      } 
}

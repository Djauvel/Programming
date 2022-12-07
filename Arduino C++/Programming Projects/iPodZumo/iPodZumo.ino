
/*
 Name:		UserInterface.ino
 Created:	11/2/2021 08:13:36 PM
 Author:	Peter Plass Jensen ROBTEK
*/

// This code utilizes the encoders to select a movement type, a speed and a duration for the operation
// When run, the LCD displays MT FWD (Movement type: forward). By scrolling on the right belt in the direction of the front-shovel
// it shows two other options, BK = backward and SPN = spin. Press button A to select. This should make the buzzer sound
// Next displayed value is Speed. Choose between 0-400 for proper functionality.
// Lastly the LCD will display ms (Millisecond) as a time delay. Choose any positive delay value.
// 3000ms after last selection buzz, the operation will run with the chosen settings, and then reset to movement type selection.


#include <Wire.h>
#include <Zumo32U4.h>

Zumo32U4Encoders encoders;
Zumo32U4Motors motors;
Zumo32U4Buzzer buzzer;
Zumo32U4LCD lcd;
Zumo32U4ButtonA buttonA;

bool movementChosen = false;                    //
bool doneChoosing = false;                      //

struct operations                               // create a datatype to store chosen values
{
  char oper;                                    // save a character for movement type
  };

operations operationData = {'x',0,0};           // create the structure with placeholder values

void setup() {
}

void loop() {
  encoders.getCountsAndResetRight();
  while(!doneChoosing){                           // until all options have been chosen; run these three while-loops
    while(!movementChosen){                       // loop this code until the type of movement has been chosen
      int countsR = encoders.getCountsRight();
    
      if(countsR<100){
        lcd.clear();                              // clear previously printed lines
        lcd.print("MEGALOV");                     // print new lines of the current selection
        delay(100);
        if(buttonA.isPressed()){                  // if buttonA is pressed while counts are under 100
            operationData.oper = 'f';             // set operation char in the struct to f for forward
            movementChosen = true;                // break while-loop by updating the condition
            buzzer.play("a");                     // play note
            }
      
      }else if(countsR>100 && countsR < 200){
        lcd.clear();                              // clear previously printed lines
        lcd.print("DOOM");                     // print new lines of the current selection
        delay(100);
          if(buttonA.isPressed()){                // if buttonA is pressed while counts are between 100 and 200:
            operationData.oper = 'b';             // set operation char in the struct to b for backward
            movementChosen = true;                // break while-loop by updating the condition
            buzzer.play("a");                     // play note
            }
      
        }else{
          lcd.clear();                            // clear previously printed lines
          lcd.print("POLISHCOW");                   // print new lines of the current selection
          delay(100);
          if(buttonA.isPressed()){                // if buttonA is pressed while counts are any other option than above;
            operationData.oper = 's';             // set operation char in the struct to s for spin
            movementChosen = true;                // break while-loop by updating the condition
            buzzer.play("a");                     // play note
          }
      }
    }
    
    }
    doneChoosing = true;                              // all options have been chosen; Break while-loop
    delay(3000);                                      // delay after buttonpress, to prevent moving while user is still touching the button
 }
  if(doneChoosing){                                   // if all necessary values have been chosen, run this code;
    
    switch(operationData.oper){
      case 'f':
        buzzer.playFromProgramSpace(Megalov);
        while(buzzer.isPlaying()){ }
        delay(1000);
      break;

      case 'b':
        buzzer.playFromProgramSpace(Doom);
        while(buzzer.isPlaying()){ }
        delay(1000);
      break;

      case 's':
        buzzer.playFromProgramSpace(PolishCow);
        while(buzzer.isPlaying()){ }
        delay(1000);
      break;

      default:
        
        
      }
      movementChosen = false; //
      doneChoosing = false;   //
    }
  
}

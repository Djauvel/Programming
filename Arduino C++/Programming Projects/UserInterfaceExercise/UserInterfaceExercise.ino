
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
bool timeChosen = false;                        // Some boolean values to help construct while loops
bool speedChosen = false;                       //
bool doneChoosing = false;                      //

struct operations                               // create a datatype to store chosen values
{
  char oper;                                    // save a character for movement type
  int speed;                                    // save an integer for speed
  int time;                                     // save an integer for time delay
  };

operations operationData = {'x',0,0};           // create the structure with placeholder values

void setup() {
}
 //Christoffer skal lære at bruge github
void loop() {
  encoders.getCountsAndResetRight();
  while(!doneChoosing){                           // until all options have been chosen; run these three while-loops
    while(!movementChosen){                       // loop this code until the type of movement has been chosen
      int countsR = encoders.getCountsRight();
    
      if(countsR<100){
        lcd.clear();                              // clear previously printed lines
        lcd.print("MT: Fwd");                     // print new lines of the current selection
        delay(100);
        if(buttonA.isPressed()){                  // if buttonA is pressed while counts are under 100
            operationData.oper = 'f';             // set operation char in the struct to f for forward
            movementChosen = true;                // break while-loop by updating the condition
            buzzer.play("a");                     // play note
            }
      
      }else if(countsR>100 && countsR < 200){
        lcd.clear();                              // clear previously printed lines
        lcd.print("MT: Bck");                     // print new lines of the current selection
        delay(100);
          if(buttonA.isPressed()){                // if buttonA is pressed while counts are between 100 and 200:
            operationData.oper = 'b';             // set operation char in the struct to b for backward
            movementChosen = true;                // break while-loop by updating the condition
            buzzer.play("a");                     // play note
            }
      
        }else{
          lcd.clear();                            // clear previously printed lines
          lcd.print("MT: Spn");                   // print new lines of the current selection
          delay(100);
          if(buttonA.isPressed()){                // if buttonA is pressed while counts are any other option than above;
            operationData.oper = 's';             // set operation char in the struct to s for spin
            movementChosen = true;                // break while-loop by updating the condition
            buzzer.play("a");                     // play note
          }
      }
  }
  
  delay(250);
  
  encoders.getCountsAndResetRight();
  while(!speedChosen){                          // loop this code until the speedvalue has been chosen
    int countsR = encoders.getCountsRight();    // retrieve encodercount
      lcd.clear();                              // clear the screen
      lcd.print("Spd:" + (String)countsR);      // print the encoder count
      delay(100);
       if(buttonA.isPressed()){                 // if the button is pressed
        operationData.speed = countsR;          // set the current value of the encoders to the speedvalue in the structure
        speedChosen = true;                     // break the while-loop by updating the condition
        buzzer.play("a");                       // play note
       }
    }
    
  delay(250);
    
  encoders.getCountsAndResetRight();
  while(!timeChosen){                                 // loop this code until the timevalue has been chosen
    int countsR = encoders.getCountsRight();          // retrieve encodercount
      lcd.clear();                                    // clear screen
      lcd.print("ms:" + (String)countsR);             // print encoder count
      delay(100);
      if(buttonA.isPressed()){                        // if buttonA is pressed
          operationData.time = countsR;               // set the current value of the encoders to the speedvalue in the structure
          timeChosen = true;                          // break the while loop by updating the condition
          buzzer.play("a");                           // play note
          lcd.clear();
      }
    }
    doneChoosing = true;                              // all options have been chosen; Break while-loop
    delay(3000);                                      // delay after buttonpress, to prevent moving while user is still touching the button
 }
  if(doneChoosing){                                   // if all necessary values have been chosen, run this code;
    
    switch(operationData.oper){
      case 'f':
        forward(operationData.speed,operationData.time);
      break;

      case 'b':
        backward(operationData.speed,operationData.time);
      break;

      case 's':
        spin(operationData.speed,operationData.time);
      break;

      default:
        motors.setSpeeds(0,0);
        
      }
      movementChosen = false; //
      speedChosen = false;    // Reset all boolean values to reset the script
      timeChosen = false;     //
      doneChoosing = false;   //
    }
  
}

//Functions to perform movement types
void forward(int speed,int time){
  motors.setSpeeds(speed,speed);
  delay(time);
  motors.setSpeeds(0,0);
  }

void backward(int speed,int time){
  motors.setSpeeds(-speed,-speed);
  delay(time);
  motors.setSpeeds(0,0);
  }

void spin(int speed,int time){
  motors.setSpeeds(-speed,speed);
  delay(time);
  motors.setSpeeds(0,0);
  }

#include <Wire.h>
#include <Zumo32U4.h>

long seconds = 143748;

Zumo32U4ButtonA buttonA;

void setup() {
  Serial.begin(9600);
}

void loop(){
  buttonA.waitForPress();                                           //Wait for button A to be pressed to execute code to prevent spam
  delay(250);
  if ((seconds / 86400) != 0){                                      //Seconds is saved as an integer, so either its a whole number which amounts to the total amount of days or zero
    Serial.println((String)(seconds / 86400) + " Day(s)");          //print seconds in days without rest of division
    }

  if ((seconds / 3600)%24 != 0){                                    //If there are any whole hours but less than one day
    Serial.println((String)((seconds/3600) % 24) + " Hour(s)");     //print whole hours
  }
    
  if ((seconds / 60)%60 != 0){                                      //if there are whole minutes, and not enough to make an entire hour
    Serial.println((String)((seconds/60)%60) + " Minute(s)");       //print those extra whole minutes
     }
  
  if ((seconds % 60) != 0){                                         //check for left over seconds, as to only print them when they exist
    Serial.println((String)(seconds%60) + " Second(s)");            //print the rest left over when dividing into whole minutes
    }
}

/*
 * 1 minut = 60 sec
 * 1 time = 3600 sec
 * 24 timer = 86400 sec
 */

char c;                       //defining character variable c
int pinY = 13;
int pinG = 30;
int pinR = 17;

void setup() {                // put your setup code here, to run once:
  Serial.begin(9600);         //Begin serial communication at 9600 bits
  pinMode(pinY, OUTPUT);
  pinMode(pinG, OUTPUT);
  pinMode(pinR, OUTPUT);
}

void loop() {                 // put your main code here, to run repeatedly:    
  if(Serial.available() > 0){
    delay(3);                 //Wait for the serial data to fill the buffer
    c = Serial.read();        //Read next byte in the buffer - Returns next character in the buffer
       digitalWrite(pinY, LOW);
       digitalWrite(pinG, HIGH);
       
      switch (c){
        case 'y':
            Serial.println("Light up yellow");
           digitalWrite(pinY, HIGH); 
           break;
           
        case 'g':
          Serial.println("Light up green");
          delay(200);   //Delay on green LED is needed, because its a recieving LED- The previous serial print is a recieved string, turning on the LED, so a delay is needed before it doesnt interfere
          digitalWrite(pinG, LOW); 
          break;
          
        default:
        break;
        }
     delay(2000);
     c = 0;
   }
}

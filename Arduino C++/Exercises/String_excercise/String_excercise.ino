char doublev = 'W';

String robotName = "Zumo";

int Zumonumber = 10;

float fraction = 0.23;

bool boolval = 1;

String stringa, stringb, stringc, stringd;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); //Initialize serial communication at 9600 bits per second
}

void loop() {
  // put your main code here, to run repeatedly:
  stringa = robotName + (String)doublev; //This is the ZumoW string creation 
  Serial.println(stringa); //printing new string

  stringb = robotName + (String)Zumonumber; //This is the Zumo10 string creation
  Serial.println(stringb); //printing new string

  stringc = robotName + (String)fraction; //This is the Zumo0.23 string creation
  Serial.println(stringc); //printing new string

  stringd = robotName + boolval; //This is the Zumo1 string creation
  Serial.println(stringd); //printing new string

  delay(2000); //delay for stability
}

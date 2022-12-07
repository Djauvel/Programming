int var1 = 10;
int var2 = 2;
int addition, subtraction, multiplication, division;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); // Initializes serial communication at 9600 bits per second
}

void loop() {
  // put your main code here, to run repeatedly:
  addition = var1 + var2; //Defining variables of addition, subtraction, multiplication and division to certain operations.
  subtraction = var1 - var2;
  multiplication = var1 * var2;
  division = var1 / var2;
  
  Serial.println("Hello, i am zumobot. these are my variables"); // Send the string "Hello world" via the serial port
  Serial.println("var1:");
  Serial.println(var1);
  Serial.println("var2:");
  Serial.println(var2);

  Serial.println("addition:"); //prints a string saying "addition" followed by the result of the addition operation
  Serial.println(addition);

  Serial.println("subtraction:");
  Serial.println(subtraction);

  Serial.println("multiplication:");
  Serial.println(multiplication);
  
  Serial.println("division:");
  Serial.println(division);
  
  Serial.println("--------I will print my variables again in 10 seconds--------");
  delay(10000); //wait 10 seconds
}

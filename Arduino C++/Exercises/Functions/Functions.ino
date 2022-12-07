void greeting(){
  Serial.println("Greetings");
  }
void printValue(int value,String measure){
  Serial.print(value );
  Serial.println(measure);
}
void readValues(int value, String measure, int pos){
  for (int i = 0;i<3;i++){
    printValue(value, measure);
  }
 }
int numberArr[]={10,20,30};
String stringArr[]={"mm","cm","m"};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  for(int i = 0; i<3; i++){
    greeting();
  readValues(numberArr[i],stringArr[i],i);
 delay(3000);
  }
}

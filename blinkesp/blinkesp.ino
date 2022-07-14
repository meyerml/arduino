#define ONBOARD_LED  2
#define buttonPin 14
void setup() {
  // put your setup code here, to run once:
  pinMode(ONBOARD_LED,OUTPUT);
  pinMode(buttonPin,INPUT_PULLUP);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(digitalRead(buttonPin)==LOW){
digitalWrite(ONBOARD_LED, HIGH);
delay(1000);
digitalWrite(ONBOARD_LED, LOW);
delay(1000);
}
}

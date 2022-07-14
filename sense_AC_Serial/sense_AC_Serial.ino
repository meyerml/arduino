#define inPin 2
#define outPin 3
int count=0;
int i;
const int arraysize=10;
int reads[arraysize];
int summe=0;
// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(outPin, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(inPin,INPUT);
  Serial.begin(9600);
}

// the loop function runs over and over again forever

void loop() {
  summe=0;
for (i=0;i<arraysize;i++){
  reads[i]=!digitalRead(inPin);
  Serial.println(reads[i]);
  Serial.println();
}
for (i=0;i<arraysize;i++){
  summe=summe+reads[i];
}
Serial.print(":::");
Serial.print(summe);
Serial.print(":::");  
Serial.println();
  if(summe==arraysize){
    count++;
    Serial.println(count);
  digitalWrite(outPin, HIGH);  
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000m0);
  }else{//wenn HIGH. promlem: ist nicht immer high, da abgehacktes signal
     digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    digitalWrite(outPin, LOW);
  }
Serial.println(digitalRead(inPin));
}

#define inPin 2
#define outPin 3
int count=0;
int i;
const int arraysize=10;

int reads[9];
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
Serial.print("test");
for (i=0;i<arraysize;i++){
  Serial.print("tessst");
}

}

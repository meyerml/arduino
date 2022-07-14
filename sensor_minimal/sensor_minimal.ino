//www.elegoo.com
//2018.10.24






int ledPin = 7;  // LED on Pin 13 of Arduino
int pirPin = 6; // Input for HC-S501

int pirValue; // Place to store read PIR Value


void setup()
{
  Serial.begin(9600); // Öffnet die serielle Schnittstelle bei 9600 Bit/s:

  pinMode(ledPin, OUTPUT);
  pinMode(pirPin, INPUT);
  digitalWrite(ledPin, LOW);
  
}

void loop()
{

    //enable motion sensor

      pirValue = digitalRead(pirPin);
      digitalWrite(ledPin, pirValue);
Serial.print(pirValue);

}

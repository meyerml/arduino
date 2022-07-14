/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp-now-two-way-communication-esp32/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/
#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN   22     // Configurable, see typical pin layout above
#define SS_PIN    5    // Configurable, see typical pin layout above

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance

/* Set your new UID here! */
#define NEW_UID {0xDE, 0xAD, 0xBE, 0xEF}

MFRC522::MIFARE_Key key;
byte password1[] = {137,226,18,164};
byte password2[] = {246,145,38,249};
 bool einmal;

#define ONBOARD_LED 2

#include <esp_now.h>
#include <WiFi.h>

#include <Wire.h>





// REPLACE WITH THE MAC Address of your receiver 84:CC:A8:A3:00:9C

uint8_t broadcastAddress[] = {0x84, 0xCC, 0xA8, 0xA3, 0x00, 0x9C};

// hier macadresse für vorhang eifügen
uint8_t broadcastAddressVorhang[] = {0x8C, 0xAA, 0xB5, 0x0D, 0xE8, 0xE5};

// Define variables to store BME280 readings to be sent
bool alarm_aus;
bool stoppstatus=1;


// Define variables to store incoming readings
bool alarm_in;

// Variable to store if sending data was successful
String success;

//Structure example to send data
//Must match the receiver structure
typedef struct struct_message {
    char a[32];
    bool alarm;
    bool stopp;
} struct_message;

// Create a struct_message called BME280Readings to hold sensor readings
struct_message outgoingReadings;

// Create a struct_message to hold incoming sensor readings
struct_message incomingReadings;

struct_message vorhang;

// Callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
  if (status ==0){
    success = "Delivery Success :)";
  }
  else{
    success = "Delivery Fail :(";
  }
}

// Callback when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&incomingReadings, incomingData, sizeof(incomingReadings));
  Serial.print("Bytes received: ");
  Serial.println(len);
  
  alarm_in = incomingReadings.alarm;
  Serial.print("Der Button/Alarm ist ");
  Serial.println(alarm_in);
  einmal = 1;
}
 
void setup() {
    //while (!Serial);     // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
  SPI.begin();         // Init SPI bus
  mfrc522.PCD_Init();  // Init MFRC522 card
  Serial.println(F("Warning: this example overwrites the UID of your UID changeable card, use with care!"));
  
  // Prepare key - all keys are set to FFFFFFFFFFFFh at chip delivery from the factory.
  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }
  pinMode(ONBOARD_LED,OUTPUT);

  // Init Serial Monitor
  Serial.begin(115200);
 Serial.print("Test");
 
 
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;

  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
  // Register peer
  esp_now_peer_info_t peerInfo2;
  memcpy(peerInfo2.peer_addr, broadcastAddressVorhang, 6);
  peerInfo2.channel = 1;  
  peerInfo2.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo2) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
  // Register for a callback function that will be called when data is received
  esp_now_register_recv_cb(OnDataRecv);
}
 
void loop() {
 delay(20);
 //Serial.print("Test");
 //Serial.print(digitalRead(buttonPin));
  // Set values to send

if (alarm_in==1){
  digitalWrite(ONBOARD_LED,HIGH);
vorhang.alarm = 1;
if (einmal ==1){
   // Send message via ESP-NOW
      esp_err_t result = esp_now_send(broadcastAddressVorhang, (uint8_t *) &vorhang, sizeof(vorhang));
   
  if (result == ESP_OK) {
    Serial.println("Sent with success Vorhang");
  }
  else {
    Serial.println("Error sending the data");
  }
  einmal =0;
}    
} else {
  digitalWrite(ONBOARD_LED,LOW);
}
if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }
  //Serial.println("Card found!"); 
  
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  for (byte i = 0; i < mfrc522.uid.size; i++) {
      // Abstand zwischen HEX-Zahlen und führende Null bei Byte < 16
      Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
      Serial.print(mfrc522.uid.uidByte[i], HEX);
      if (mfrc522.uid.uidByte[i]!=password1[i] && mfrc522.uid.uidByte[i]!=password2[i]){
        return;
      }
    } 
    Serial.println();
    
        mfrc522.PICC_HaltA();
     stoppstatus=!stoppstatus;  //stoppstatus 0 beduetet stopp
      outgoingReadings.stopp=!stoppstatus;


  // Send message via ESP-NOW
      esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &outgoingReadings, sizeof(outgoingReadings));
   
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }    
    digitalWrite(ONBOARD_LED,HIGH);
    delay(100);
    digitalWrite(ONBOARD_LED,LOW);
    delay(100);
    digitalWrite(ONBOARD_LED,HIGH);
    delay(100);
    digitalWrite(ONBOARD_LED,LOW);
    delay(100);
    digitalWrite(ONBOARD_LED,HIGH);
    delay(100);
    digitalWrite(ONBOARD_LED,LOW);
    delay(100);
         stoppstatus=!stoppstatus;  //stoppstatus 0 beduetet stopp
      outgoingReadings.stopp=!stoppstatus;


  // Send message via ESP-NOW
       result = esp_now_send(broadcastAddress, (uint8_t *) &outgoingReadings, sizeof(outgoingReadings));
   
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }  
  
}

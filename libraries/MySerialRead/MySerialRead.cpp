//MySerialRead.cpp
//Code fuer Arduino
//Author Retian
//Version 2.1

/*
Einlesen von Zeichenketten zur Umwandlung in Zahlen (Float-, Interger- und Unsigned-Integer-Zahlen) vom Seriellen Monitor.
Die Zeichenketten werden auf Gueltigkeit und die ermittelten Zahlen auf eventuelle Bereichsueberschreitung ueberprueft.

MySerialRead Name;

Beispiel siehe unter:
http://arduino-projekte.webnode.at/projekte/serieller-monitor/

Funktionen siehe unter:
http://arduino-projekte.webnode.at/projekte/serieller-monitor/funktionen/

*/

#include "Arduino.h"
#include "MySerialRead.h"

MySerialRead::MySerialRead()
{
}

//***************************************************************************
//Einlesen einer Float-Zahl

bool MySerialRead::readFloat(float* val, uint8_t mode)
{
  float* _val = val;
  uint8_t _mode = mode;
  
  index = 0;
  inStringOK = false;
  inString[0] = '\0';
  
  if (_mode == 1)
  {
    while (Serial.available()) Serial.read(); //Serial-Buffer leeren
    do {} while (!Serial.available()); //Warte auf Zeichen
  }
     
  while (Serial.available())
  {
    incomingByte = Serial.read();
      
    if (index == 0 && incomingByte == 45)
    {
      inString[0] = 45;
      index++;
    }

    else if ((incomingByte >= 48 && incomingByte < 58) 
              || incomingByte == 46 || incomingByte == 44)
    {
      if (incomingByte == 44) incomingByte = 46;
      inString[index]  = incomingByte;
      index++;
      if (index <= maxIndex - 1) inStringOK = true; //Array-Ueberlaufschutz
      else
      {
         inStringOK = false;
         index--;
       }
    }
    else if (incomingByte == 10 || incomingByte == 13)
    {
      if (index == 0)
      {
        inStringOK = false;
        break;
      }
    }
    else
    {
      index = 0;
      inStringOK = false;
      break;
    }
    delay(1);
  }
  while (Serial.available()) delay(1); //Serial-Buffer leeren
  inString[index] = '\0';
    
  if (inStringOK)
  {
    (*_val) = atof(inString);
    return true;
  }
  return false;
}

//***************************************************************************
//Einlesen eines Bytes

bool MySerialRead::readByte(byte* val, uint8_t mode)
{
  byte* _val = val;
  uint8_t _mode = mode;
  
  index = 0;
  checkVal = 0;
  inStringOK = false;
  inString[0] = '\0';

  if (_mode == 1)
  {
    while (Serial.available()) Serial.read(); //Serial-Buffer leeren
    do {} while (!Serial.available()); //Warte auf Zeichen
  }
  incomingUInt();
  if (inStringOK)
  {
    checkVal = atol(inString);
    if (checkVal >= 0 && checkVal < 256)
    {
      *_val = (byte)checkVal;
      return true;
    }
  }
  return false;
}

//***************************************************************************
//Einlesen einer Integer-Zahl (8 Bit)

bool MySerialRead::readInt8(int8_t* val, uint8_t mode)
{
  int8_t* _val = val;
  uint8_t _mode = mode;

  index = 0;
  checkVal = 0;
  inStringOK = false;
  inString[0] = '\0';

  if (_mode == 1)
  {
    while (Serial.available()) Serial.read(); //Serial-Buffer leeren
    do {} while (!Serial.available()); //Warte auf Zeichen
  }
  incomingInt();
  if (inStringOK)
  {
    checkVal = atol(inString);
    if (checkVal >= -128 && checkVal < 128)
    {
      *_val = (int8_t)checkVal;
      return true;
    }
  }
  return false;
}

//***************************************************************************
//Einlesen einer Integer-Zahl (16 Bit)

bool MySerialRead::readInt16(int* val, uint8_t mode)
{
  int* _val = val;
  uint8_t _mode = mode;

  index = 0;
  checkVal = 0;
  inStringOK = false;
  inString[0] = '\0';

  if (_mode == 1)
  {
    while (Serial.available()) Serial.read(); //Serial-Buffer leeren
    do {} while (!Serial.available()); //Warte auf Zeichen
  }
  incomingInt();
  if (inStringOK)
  {
    checkVal = atol(inString);
    if (checkVal >= -32768 && checkVal < 32768)
    {
      *_val = (int)checkVal;
      return true;
    }
  }
  return false;
}

//***************************************************************************
//Einlesen einer Unsigned-Integer-Zahl (16 Bit)

bool MySerialRead::readUInt16(unsigned int* val, uint8_t mode)
{
  unsigned int* _val = val;
  uint8_t _mode = mode;

  index = 0;
  checkVal = 0;
  inStringOK = false; 
  inString[0] = '\0';
  
  if (_mode == 1)
  {
    while (Serial.available()) Serial.read(); //Serial-Buffer leeren
    do {} while (!Serial.available()); //Warte auf Zeichen
  }
  incomingUInt();
  if (inStringOK)
  {
    checkVal = atol(inString);
    if (checkVal >= 0 && checkVal <= 65535)
    {
      *_val = (unsigned int)checkVal;
      return true;
    }
  }
  return false;
}

//***************************************************************************
//Einlesen einer Long-Zahl (32 Bit)

bool MySerialRead::readInt32(long* val, uint8_t mode)
{
  long* _val = val;
  uint8_t _mode = mode;

  index = 0;
  //checkVal = 0;
  inStringOK = false;
  inString[0] = '\0';

  if (_mode == 1)
  {
    while (Serial.available()) Serial.read(); //Serial-Buffer leeren
    do {} while (!Serial.available()); //Warte auf Zeichen
  }
  incomingInt();
  if (inStringOK)
  {
    checkVal = atol(inString);
    if (checkVal >= -2147483648L && checkVal <= 2147483647L)
    {
      *_val = checkVal;
      return true;
    }
  }
  return false;
}

//***************************************************************************
//Einlesen von Unsigned-Integer-Zeichenkette vom Seriellen Monitor
//(Interne Verwendung)

void MySerialRead::incomingUInt()
{
  while (Serial.available())
  {
    incomingByte = Serial.read();

    if (incomingByte >= 48 && incomingByte < 58) 
    {
      inString[index] = incomingByte;
      index++;
      if (index <= maxIndex - 1) inStringOK = true; //Array-Ueberlaufschutz
      else
      {
        inStringOK = false;
        break;
      }
    }
    else if (incomingByte == 10 || incomingByte == 13)
    {
      if (index == 0)
      {
        inStringOK = false;
        break;
      }
    }
    else
    {
      index = 0;
      inStringOK = false;
      break;
    }
    delay(1);
  }
  delay(1);
  while (Serial.available()) Serial.read(); //Serial-Buffer leeren
  inString[index] = '\0';
}

//***************************************************************************
//Einlesen von Integer-Zeichenkette vom Seriellen Monitor
//(Interne Verwendung)

void MySerialRead::incomingInt()
{
  while (Serial.available())
  {
    incomingByte = Serial.read();
          
    if (index == 0 && incomingByte == 45) 
    {
      inString[0] = 45;
      index++;
    }
    else if (incomingByte >= 48 && incomingByte < 58) 
    {
      inString[index] = incomingByte;
      index++;
      if (index <= maxIndex - 1) inStringOK = true; //Array-Ueberlaufschutz
      else
      {
        inStringOK = false;
        break;
      }
   }
    else if (incomingByte == 10 || incomingByte == 13)
    {
      if (index == 0)
      {
        inStringOK = false;
        break;
      }
    }
    else
    {
      index = 0;
      inStringOK = false;
      break;
    }
    delay(1);
  }
  delay(1);
  while (Serial.available()) Serial.read(); //Serial-Buffer leeren
  inString[index] = '\0';
}
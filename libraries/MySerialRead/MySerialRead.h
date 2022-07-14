//MySerialRead.h

#ifndef MySerialRead_h
#define MySerialRead_h

#include "Arduino.h"

#define SERIAL_WAIT 1
#define SERIAL_CONT 0

#define maxIndex 12 //max. Groesse des "inString"-Arrays

class MySerialRead
{
  public:
    MySerialRead();
    bool readFloat(float*, uint8_t);
    bool readByte(byte*, uint8_t);
    bool readInt8(int8_t*, uint8_t);
    bool readInt16(int*, uint8_t);
    bool readUInt16(unsigned int*, uint8_t);
    bool readInt32(long*, uint8_t);
            
  private:
    void incomingInt(void);
    void incomingUInt(void);
    char incomingByte;
    bool inStringOK;
    byte index;
    char inString[maxIndex];
    long checkVal;
 }; 

#endif

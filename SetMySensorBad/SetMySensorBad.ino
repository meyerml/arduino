//Set_DS3231
//Datum/Uhrzeit einstellen bei der RTC DS3231
//Code fuer Arduino
//Author Retian
//Version 1.2

#include <MyDS3231.h>
#include <MySerialRead.h>

MyDS3231 RTC(0x68);
MySerialRead SRead;

//Prototypen
bool leseWert(byte* , byte, byte);
void pruefeKleiner10(byte);
void zeigeMenue(void);
void setzeDatum(void);
void setzeUhrzeit(void);
void setzeWochetag(void);
void zeigeDatumUhrzeit(void);
void zeigeUhrzeit10s(void);
void zeigeWochentag(void);

byte tag, monat, jahr;
byte stunde, minute, sekunde;
byte wochenTag;
char inByte;

void setup() {
  Serial.begin(115200);
  Serial.println(F("Einstellen und Anzeigen von"));
  Serial.println(F("   Datum und Uhrzeit der   "));
  Serial.println(F("Real Time Clock (RTC) DS3231"));

  zeigeMenue();
}

void loop() {
  while (Serial.available()) Serial.read(); //Buffer leeren
  while (!Serial.available());

  inByte = Serial.read();
  if (inByte == 'M') zeigeMenue();
  if (inByte == 'D') setzeDatum();
  else if (inByte == 'U') setzeUhrzeit();
  else if (inByte == 'W') setzeWochetag();
  else if (inByte == 'd') zeigeDatumUhrzeit();
  else if (inByte == 'u') zeigeUhrzeit10s();
  else if (inByte == 'w') zeigeWochentag();

}

bool leseWert(byte *wert, byte ug, byte og)
{
  if (SRead.readByte(&(*wert), SERIAL_WAIT))
  {
    if (*wert < ug || *wert > og) return false;
    else return true;
  }
  else return false;
}

void pruefeKleiner10(byte wert)
{
  if (wert < 10) Serial.print("0");
}

void zeigeMenue()
{
  Serial.println("\nMenue:");
  Serial.println(F("M ... Zeige Menue"));
  Serial.println(F("D ... Setze Datum"));
  Serial.println(F("U ... Setze Uhrzeit"));
  Serial.println(F("W ... Setze Wochentag"));
  Serial.println(F("d ... Zeige Datum/Uhrzeit"));
  Serial.println(F("u ... Zeige Uhrzeit 10 Sek."));
  Serial.println(F("w ... Zeige Wochentag"));
  Serial.println();
}

void setzeDatum()
{
  Serial.println(F("Eingabe des Datums:"));

  Serial.print(F("Tag  : "));
  while (!leseWert(&tag, 1, 31));
  pruefeKleiner10(tag);
  Serial.println(tag);

  Serial.print(F("Monat: "));
  while (!leseWert(&monat, 1, 12));
  pruefeKleiner10(monat);
  Serial.println(monat);

  Serial.print(F("Jahr : "));
  while (!leseWert(&jahr, 0, 99));
  Serial.print("20");
  pruefeKleiner10(jahr);
  Serial.println(jahr);

  Serial.println(F("Uebernehmen? J/N"));
  while (!Serial.available());

  inByte = Serial.read();
  if (inByte == 'J')
  {
    RTC.setDate(tag, monat, jahr);
    Serial.println(F("Datum uebernommen!"));
    zeigeDatumUhrzeit();
  }
  else Serial.println(F("Abbruch durch Benutzer"));
  Serial.println();

}

void setzeUhrzeit()
{
  Serial.println(F("Eingabe der Uhrzeit:"));

  Serial.print(F("Stunde : "));
  while (!leseWert(&stunde, 0, 23));
  pruefeKleiner10(stunde);
  Serial.println(stunde);

  Serial.print(F("Minute : "));
  while (!leseWert(&minute, 0, 59));
  pruefeKleiner10(minute);
  Serial.println(minute);

  Serial.print(F("Sekunde: "));
  while (!leseWert(&sekunde, 0, 59));
  pruefeKleiner10(sekunde);
  Serial.println(sekunde);

  Serial.println(F("Uebernehmen? J/N"));
  while (!Serial.available());

  inByte = Serial.read();
  if (inByte == 'J')
  {
    RTC.setTime(stunde, minute, sekunde);
    Serial.println(F("Uhrzeit uebernommen!"));
    zeigeDatumUhrzeit();
  }
  else Serial.println(F("Abbruch durch Benutzer"));
  Serial.println();
}

void setzeWochetag()
{
  //byte wochenTag;

  Serial.println(F("Eingabe des Wochentags:"));

  Serial.println(F("1 ... Montag"));
  Serial.println(F("2 ... Dienstag"));
  Serial.println(F("3 ... Mittwoch"));
  Serial.println(F("4 ... Donnerstag"));
  Serial.println(F("5 ... Freitag"));
  Serial.println(F("6 ... Samstag"));
  Serial.println(F("7 ... Sonntag"));

  if (SRead.readByte(&wochenTag, SERIAL_WAIT))
  {
    if (wochenTag > 0 && wochenTag <= 7)
    {
      Serial.print(F("Gewaehlt: "));
      Serial.println(wochenTag);
      Serial.println(F("Uebernehmen? J/N"));
      while (!Serial.available());

      inByte = Serial.read();
      if (inByte == 'J')
      {
        RTC.setDow(wochenTag);
        Serial.println(F("Wochentag uebernommen!"));
        RTC.nowDateTime();
        zeigeWochentag();
      }
      else Serial.println(F("Abbruch durch Benutzer"));
      Serial.println();
    }
    else Serial.println(F("Falsche Eingabe!"));
  }
}

void zeigeDatumUhrzeit()
{
  char timeString[9];
  char dateString[11];

  Serial.println(F("Zeige Datum und Uhrzeit:"));
  RTC.nowDateTime();
  RTC.getDateString(dateString, YEAR_4_DIG);
  RTC.getTimeString(timeString, HH_MM_SS);
  Serial.print(dateString);
  Serial.print("  ");
  Serial.println(timeString);
  Serial.println();
}

void zeigeUhrzeit10s()
{
  char timeString[9];

  Serial.println(F("Zeige Uhrzeit 10 Sekunden:"));
  for (byte i = 0; i < 10; i++)
  {
    RTC.nowDateTime();
    RTC.getTimeString(timeString, HH_MM_SS);
    Serial.println(timeString);
    delay(1000);
  }
  Serial.println();
}

void zeigeWochentag()
{
  char dowString[11];

  Serial.println(F("Zeige Wochentag:"));
  RTC.nowDateTime();
  RTC.getDowString(dowString);
  Serial.println(dowString);
  Serial.println();
}

#include <SPI.h>  //for the SD card module
#include <SD.h>   // for the SD card
#include "RTClib.h"
RTC_DS1307 RTC;

//These values are in the datasheet
#define RT0 100000  // Ω
#define B 3950
//K
//--------------------------------------
#define VCC 5  //Supply voltage
#define R 10000  //R=10KΩ
const int chipSelect = 10;


// Create a file to store the data
File myFile;

//Variables
float RT, VR, ln, t, TX, T0, VRT;
uint16_t count=0;
void setup() {
  RTC.begin();
  //initializing Serial monitor
  Serial.begin(9600);

  if (!RTC.begin()) {
    Serial.println("Couldn't find RTC");

  } else {
    // following line sets the RTC to the date & time this sketch was compiled
     RTC.adjust(DateTime(__DATE__, __TIME__));
  }
  if (!RTC.isrunning()) {
    Serial.println("RTC is NOT running!");
  }

  // setup for the SD card
  Serial.print("Initializing SD card...");

  if (!SD.begin(chipSelect)) {
    Serial.println("initialization failed!");
    delay(1000);
  }
  Serial.println("initialization done.");

  //open file
}

void loop() {
  Writedata();
  
  delay(1000);
}

void Writedata()
{
  myFile = SD.open("NTC1.txt", FILE_WRITE);
  if (myFile) {
    Serial.print("File opened ok  ");
    Serial.println(count);
    count++;
    DateTime now = RTC.now();

    myFile.print(now.day(), DEC);
    myFile.print('/');
    myFile.print(now.month(), DEC);
    myFile.print('/');
    myFile.print(now.year(), DEC);
    myFile.print(' ');
    myFile.print(now.hour(), DEC);
    myFile.print(':');
    myFile.print(now.minute(), DEC);
    myFile.print(':');
    myFile.print(now.second(), DEC);
    myFile.print(",");

    Serial.print(now.day(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.year(), DEC);
    Serial.print(" ");
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);

    T0 = 25 + 273.15;
    float VRT = 0;

    for (int i = 0; i < 20; i++) {
      VRT = VRT + analogRead(A0);  //Acquisition analog value of VRT
    }
    VRT = VRT / 20;
    //Serial.println(VRT);
    VRT = (5.00 / 1023.00) * VRT;  //Conversion to voltage
    VR = VCC - VRT;
    RT = VRT / (VR / R);  //Resistance of RT
    ln = log(RT / RT0);
    TX = (1 / ((ln / B) + (1 / T0)));  //Temperature from thermistor
    TX = TX - 273.15;
    //debugging purposes
    Serial.print("\t");
    Serial.print(TX);
    Serial.println("°C");
    myFile.print(' ');
    myFile.print(TX);
    myFile.println(" ,");
    myFile.close();
  }
  else
  {
    Serial.println("not open");
  }
  
  }
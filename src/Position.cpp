// Require hardware cutoff GPS power for power saving
// Output Data Not test
#include "Position.h"
#include <HardwareSerial.h>
#include <TinyGPS++.h>
extern HardwareSerial Serial2;
TinyGPSPlus gps;

void Position::Initialize()
{
  Serial2.setRxBufferSize(256);
  Serial2.begin(9600,SERIAL_8N1, -1, 4); // RX: 26 TX: 27, 
  pinMode(2,OUTPUT);
  digitalWrite(2,HIGH);
  GPSSwichMode = true;
}

void Position::encode()
{
  if(Serial2.available() > 0){    
    gps.encode(Serial2.read());
  }
}

void Position::Read()
{
  if (GPSSwichMode)
  {
    Satellites = gps.satellites.value();
    if (Satellites ==0){ManualShow = "No Connection";}
    if (gps.location.isValid())
    {
      LatNow = gps.location.lat();
      LonNow = gps.location.lng();
    }else{
      LatNow = -3.0;
      LonNow = -3.0;
    }
    if (gps.time.isValid())
    {
      Time = String(gps.date.day());
    }else{
      Time = "NoTime";
    }
  }else{
    LatNow = -1.0;
    LonNow = -1.0;
    Time = "";
  }
}

void Position::Swich()
{
  if(GPSSwichMode){
    GPSSwichMode = false;
    Satellites = -1;
    ManualShow = "Off";
  }else{
    GPSSwichMode = true;
  }
}


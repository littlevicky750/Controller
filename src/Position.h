#ifndef Position_h
#define Position_h

#include <Arduino.h>

class Position
{
  public:
  void Initialize();
  void Read();
  void encode();
  void Swich();
  bool GPSSwichMode;
  double LatNow = 0;
  double LonNow = 0;
  String Time = "Initial String";
  String ManualShow;
  int Satellites = 0;
};

#endif

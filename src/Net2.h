#ifndef Net2_h
#define Net2_h

#include <Arduino.h>

class Net2
{
  public:
  void Initialize();
  void Send(String Msg);
  void Swich();
  int State = -1; // -1 = LoRa off
  int LED25Val;

  private:
  bool LoRaBegin = true;
  bool LoRaBeginCheck;
};


#endif

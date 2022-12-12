#include "Net2.h"
#include <SPI.h>
#include <LoRa.h>

#define LoRaSPI
SPIClass *LoRaspi = NULL;


void Net2::Initialize()
{
  LoRaspi = new SPIClass(LoRaSPI);
  LoRa.setSPI(*LoRaspi);
  LoRaspi->begin(18,19,23,5);
  LoRa.setPins(5,-1); // ss: 5, no rst pin
  pinMode(19, INPUT_PULLUP);
  pinMode( 5, INPUT_PULLUP);
  pinMode(18, OUTPUT);
  digitalWrite(18,HIGH);
  LoRaBegin = false;
  for(int i = 0; i < 3 ; i++){
    if(LoRa.begin(433E6)){
      LoRaBegin = true;
      Serial.println("LoRa Start");
      break;
    }
    delay(100);
  }
  if (!LoRaBegin) 
  {
    Serial.println("LoRa Failed!");
  }
}

void Net2::Send(String Msg)
{
  if(State == -1){return;}
  digitalWrite(25,HIGH);
  LoRaBeginCheck = LoRaBegin;
  LoRaBegin = false;
  for(int i = 0; i < 3 ; i++){
    if(LoRa.begin(433E6)){
      LoRaBegin = true;
      break;
    }
    Serial.print("LoRa Check faild : ");
    Serial.println(i);
    delay(100);
  }
  if(LoRaBegin != LoRaBeginCheck){
    if(LoRaBegin){
      Serial.println("LoRa Start");
    }else{
      Serial.println("LoRa Failed!");
    }
  }
  if(LoRaBegin){
    LoRa.beginPacket();
    LoRa.print(Msg);
    LoRa.endPacket(true);
  }
  digitalWrite(25,LED25Val);
}

void Net2::Swich(){
  State *= -1;
}

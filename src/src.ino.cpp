# 1 "C:\\Users\\littl\\AppData\\Local\\Temp\\tmppso5ylod"
#include <Arduino.h>
# 1 "C:/Users/littl/HKUST/CHANG Ching Wei - Wonder Construct Team/Arduino codes/MainSensor_v3/src/src.ino"





TaskHandle_t Read;
TaskHandle_t Show;
TaskHandle_t Save;
TaskHandle_t Load;
TaskHandle_t Send;
TaskHandle_t Scan;
TaskHandle_t SEND;
TaskHandle_t Buff;
TaskHandle_t SleepClock;

#include "SerialDebug.h"
SerialDebug D;
extern SerialDebug Debug = D;

bool SendCommand = false;

#include "RealTimeClock.h"
#include "IMU.h"
#include "MsgBuffer.h"
#include "Control.h"

#include "SDCard.h"

#include "Battery.h"
#include "OLED.h"

Control control;
SDCard sdCard;
MsgBuffer msg;
IMU imu;
RealTimeClock Clock;
Battery battery;
OLED oled;

bool led = true;

#include "Net.h"
#include "OnOffControl.h"
#include "Button.h"
void setup();
void ReadDataCallback(void *pvParameters);
void SaveBuffCallback(void *pvParameters);
void ShowDataCallback(void *pvParameters);
void SaveDataCallback(void *pvParameters);
void ScanBLEDCallback(void *pvParameters);
void SendDataCallback(void *pvParameters);
void loop();
#line 46 "C:/Users/littl/HKUST/CHANG Ching Wei - Wonder Construct Team/Arduino codes/MainSensor_v3/src/src.ino"
void setup()
{
  WakeUpDetect();

  imu.Initialize(&Euler_Children[0][0], &NodeNumber[0]);
  battery.SetPin(17);
  oled.Initialize();
  sdCard.Swich();
  oled.bat = &battery;
# 75 "C:/Users/littl/HKUST/CHANG Ching Wei - Wonder Construct Team/Arduino codes/MainSensor_v3/src/src.ino"
  xTaskCreatePinnedToCore(ScanBLEDCallback, "Scan", 20000, NULL, 4, &Scan, 0);
  xTaskCreatePinnedToCore(SaveDataCallback, "Save", 10000, NULL, 1, &Save, 0);
  xTaskCreatePinnedToCore(SendDataCallback, "Send", 8192, NULL, 5, &Send, 0);
  xTaskCreatePinnedToCore(ShowDataCallback, "Show", 4096, NULL, 3, &Show, 1);
  xTaskCreatePinnedToCore(ReadDataCallback, "Read", 4096, NULL, 2, &Read, 1);
  xTaskCreatePinnedToCore(SaveBuffCallback, "Buff", 4096, NULL, 2, &Buff, 1);

  Button_Initialize();

  msg.Clear();
  digitalWrite(40, HIGH);
  digitalWrite(41, HIGH);
  digitalWrite(42, HIGH);
}

void ReadDataCallback(void *pvParameters)
{
  for (;;)
  {

    imu.Read();
    imu.LocalAngleShift();
    vTaskDelay(25);
  }
}

void SaveBuffCallback(void *pvParameters)
{
  vTaskDelay(30 * 1000);
  for (;;)
  {
    msg.Add(Clock.DateTimeStamp() + "," + String((battery.Percent), 2), imu.EulerAvg, imu.EulerLocal, Address, Euler_Children, NodeNumber);
    vTaskDelay(msg.WriteWhen * 1000);
  }
}

void ShowDataCallback(void *pvParameters)
{
  vTaskDelay(3000);
  for (;;)
  {
    Clock.update();
    ButtonPress();
    oled.Display(Mode, DeviceID, *ModeChoose, ButtonState,
                 NodeNumber, DeviceType,
                 control.UserSetting, control.CheckSetting(),
                 msg.Msg, msg.WriteWhen, Clock, Address, imu, sdCard);
    vTaskDelay(100);
  }
}

void SaveDataCallback(void *pvParameters)
{
  vTaskDelay(5000);
  sdCard.SetPin();
  pinMode(4, OUTPUT);
  digitalWrite(4, HIGH);
  pinMode(16, OUTPUT);
  digitalWrite(16, HIGH);
  for (;;)
  {

    if (!sdCard.SwichMode)
    {
      msg.BufferCheck();
      digitalWrite(16, LOW);
      delay(500);
    }
    else
    {
      digitalWrite(4,LOW);
      if(sdCard.CheckCount != -1)
        digitalWrite(16,LOW);
      if (!sdCard.CheckState())
      {
        msg.BufferCheck();
      }
      else
      {
        digitalWrite(16,HIGH);
        if (!sdCard.Initialize)
        {
          sdCard.CreateFile(Clock.DateStamp("", 2));
        }
        if (sdCard.Initialize)
        {
          String MsgTest = msg.Msg;
          msg.Clear();
          sdCard.WriteFile(MsgTest);
        }
        else
        {
          msg.BufferCheck();
        }
      }
    }
    oled.State[3] = sdCard.CheckCount;
    digitalWrite(4, HIGH);
    digitalWrite(16, HIGH);

    vTaskDelay(max({10000, msg.WriteWhen * 1000}));
  }
}

void ScanBLEDCallback(void *pvParameters)
{


  Net_Initialize();
  for (;;)
  {
    vTaskDelay(5000);
    BLE_Scan();
    battery.Update();
  }
}

void SendDataCallback(void *pvParameters)
{
  for (;;)
  {
    if (SendCommand && NodeNumber[0] != 0 && NodeNumber[1] == 0)
    {
      control.Estimate(imu.LocalAngleShift());
      BLE_Send(control.V);
    }
    vTaskDelay(control.dt_ms);
  }
}

void loop() {}
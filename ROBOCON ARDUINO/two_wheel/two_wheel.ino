/*
 Example sketch for the PS3 Bluetooth library - developed by Kristian Lauszus
 For more information visit my blog: http://blog.tkjelectronics.dk/ or
 send me an e-mail:  kristianl@tkjelectronics.com
 */

#include <PS3BT.h>
#include <usbhub.h>

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

USB Usb;
//USBHub Hub1(&Usb); // Some dongles have a hub inside

BTD Btd(&Usb); // You have to create the Bluetooth Dongle instance like so
/* You can create the instance of the class in two ways */
PS3BT PS3(&Btd); // This will just create the instance
//PS3BT PS3(&Btd, 0x00, 0x15, 0x83, 0x3D, 0x0A, 0x57); // This will also store the bluetooth address - this can be obtained from the dongle when running the sketch

//bool printTemperature, printAngle;

//PS3BT PS3(&Btd, 0x00, 0x15, 0x83, 0x3D, 0x0A, 0x57); // This will also store the bluetooth address - this can be obtained from the dongle when running the sketch
int dir[] = {15, 17}; //17,13 are left wheel
int pwm[] = {12, 13}; //15,12 are right wheel
bool printTemperature, printAngle;
int s;
int upwd, dwnwd, rtro, lfro, clk, anticlk;
void setup() {
  Serial.begin(115200);
#if !defined(__MIPSEL__)
  while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#endif
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1); //halt
  }
  Serial.print(F("\r\nPS3 Bluetooth Library Started"));
  for (int i = 0; i < 2; i++)
  {
    pinMode(dir[i], OUTPUT);
    pinMode(pwm[i], OUTPUT);
  }

}
void loop() {
  Usb.Task();

  if (PS3.PS3Connected || PS3.PS3NavigationConnected) {

    upwd = map(PS3.getAnalogHat(LeftHatY), 137, 255, 0, 100);
    dwnwd = map( PS3.getAnalogHat(LeftHatY), 117, 0, 0, 100);
    rtro = map( PS3.getAnalogHat(RightHatX), 137, 255, 0, 100);
    lfro = map( PS3.getAnalogHat(RightHatX), 117, 0, 0, 100);
    anticlk = map(PS3.getAnalogButton(L2), 0, 255, 0, 100);
    clk = map(PS3.getAnalogButton(R2), 0, 255, 0, 100);
    Serial.println(upwd);
    Serial.println(dwnwd);
    Serial.println(rtro);
    Serial.println(lfro);

    if (PS3.getAnalogButton(L2) < 10 || PS3.getAnalogButton(R2) < 10)
    {
      if (PS3.getAnalogHat(LeftHatY) < 137 && PS3.getAnalogHat(LeftHatY) > 117 )
      {
        for (int i = 0; i < 2; i ++)
        {
          analogWrite(pwm[i], 0);
        }

      }
      if (PS3.getAnalogHat(RightHatX) < 137 && PS3.getAnalogHat(RightHatX) > 117)
      {
        for (int i = 0; i < 2; i ++)
        {
          analogWrite(pwm[i], 0);
        }

      }
    }
    if (PS3.getAnalogHat(RightHatX) < 137)
    {
      for (int i = 0; i < 2; i++)
      {
        digitalWrite(dir[i], LOW);
        analogWrite(pwm[i], lfro);
      }

    }
    if (PS3.getAnalogHat(RightHatX) > 137)
    {
      for (int i = 0; i < 2; i++)
      {
        digitalWrite(dir[i], HIGH);
        analogWrite(pwm[i], rtro);
      }

    }
    if (PS3.getAnalogHat(LeftHatY) > 137 )
      //forward
    {
      Serial.println(upwd);
      for (int i = 0; i < 2; i = i++)
      {
        digitalWrite(dir[i], HIGH);
        analogWrite(pwm[i], upwd);
      }
    }
    if (PS3.getAnalogHat(LeftHatY) < 137 )
      //backward
    {
      for (int i = 0; i < 2; i = i++)
      {
        digitalWrite(dir[i], LOW);
        analogWrite(pwm[i], dwnwd);
      }
    }


    if (PS3.getAnalogButton(L2) > 10)
    {
      //anticlockwise

      digitalWrite(dir[0], HIGH); //0-right
      analogWrite(pwm[0], anticlk);//1-left
      digitalWrite(dir[1], LOW);
      analogWrite(pwm[1], anticlk);
    }
    if (PS3.getAnalogButton(R2) > 10)
    {
      //clockwise

      digitalWrite(dir[0], LOW);
      analogWrite(pwm[0], clk);
      digitalWrite(dir[1], HIGH);
      analogWrite(pwm[1], clk );
    }
  }

}

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
int dir[]={1,2,3,4};
int abc[]={6,7,8,9};
bool printTemperature, printAngle;
int s;
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
  for(int i=0;i<4;i++)
  {
    pinMode(dir[i],OUTPUT);
    pinMode(abc[i],INPUT);
  }
  
}
void loop() {
  Usb.Task();

  if (PS3.PS3Connected || PS3.PS3NavigationConnected) {
    if (PS3.getAnalogHat(LeftHatY) >= 117 &&  PS3.getAnalogHat(LeftHatY) <= 137)
    {
    }
      
      else if(PS3.getAnalogHat(LeftHatY)<117 || PS3.getAnalogHat(LeftHatY)>137)
      {
        int c=PS3.getAnalogHat(LeftHatY);
        
         if(PS3.getAnalogHat(LeftHatY)<117)
         {
          s=map(c,0,117,0,100);
          for(int i=0;i<4;i=i+2)
          {
            digitalWrite(dir[i],LOW);
            analogWrite(abc[i],s);
            
          }
          
         }
         if(PS3.getAnalogHat(LeftHatY)>137)
         {
          s=map(c,137,255,0,100);
          for(int i=0;i<4;i+=2)
          {
            digitalWrite(dir[i],HIGH);
            analogWrite(abc[i],s);
            
          }
          
         }
      }
      if (PS3.getAnalogHat(RightHatX) >= 117 &&  PS3.getAnalogHat(RightHatX) <= 137)
      {        
      }
      else if(PS3.getAnalogHat(RightHatY)<117 || PS3.getAnalogHat(RightHatY)>137)
      {
        int c=PS3.getAnalogHat(RightHatY);
         if(PS3.getAnalogHat(RightHatY)<117)
         {
          s=map(c,0,117,0,100);
          for(int i=1;i<4;i+=2)
          {
            digitalWrite(dir[i],LOW);
            analogWrite(abc[i],s);
            
          }
          
         }
         if(PS3.getAnalogHat(RightHatY)>137)
         {
          s=map(c,137,255,0,100);
          for(int i=1;i<4;i+=2)
          {
            digitalWrite(dir[i],HIGH);
            analogWrite(abc[i],s);
            
          }
          
         }
      
      }
    }

    // Analog button values can be read from almost all buttons
    if (PS3.getAnalogButton(L2) || PS3.getAnalogButton(R2)) {
        int c=PS3.getAnalogHat(RightHatY);
        int d=PS3.getAnalogHat(LeftHatX);
        
        if(c>10)
        {
          s=map(c,10,255,0,100);
          for(int i=1;i<4;i++)
          {
            digitalWrite(dir[i],HIGH);
            analogWrite(abc[i],s);
            
          }
        }
        
        if(d>10)
        {
          s=map(d,10,255,0,100);
          for(int i=1;i<4;i++)
          {
            digitalWrite(dir[i],LOW);
            analogWrite(abc[i],s);
            
          }
        }
        
    }

    if (PS3.getButtonClick(PS)) {
      Serial.print(F("\r\nPS"));
      PS3.disconnect();
    }
    else {
      if (PS3.getButtonClick(TRIANGLE)) {
        Serial.print(F("\r\nTriangle"));
        PS3.setRumbleOn(RumbleLow);
      }
      if (PS3.getButtonClick(CIRCLE)) {
        Serial.print(F("\r\nCircle"));
        PS3.setRumbleOn(RumbleHigh);
      }
      if (PS3.getButtonClick(CROSS))
        Serial.print(F("\r\nCross"));
      if (PS3.getButtonClick(SQUARE))
        Serial.print(F("\r\nSquare"));

      if (PS3.getButtonClick(UP)) {
        Serial.print(F("\r\nUp"));
        if (PS3.PS3Connected) {
          PS3.setLedOff();
          PS3.setLedOn(LED4);
        }
      }
      if (PS3.getButtonClick(RIGHT)) {
        Serial.print(F("\r\nRight"));
        if (PS3.PS3Connected) {
          PS3.setLedOff();
          PS3.setLedOn(LED1);
        }
      }
      if (PS3.getButtonClick(DOWN)) {
        Serial.print(F("\r\nDown"));
        if (PS3.PS3Connected) {
          PS3.setLedOff();
          PS3.setLedOn(LED2);
        }
      }
      if (PS3.getButtonClick(LEFT)) {
        Serial.print(F("\r\nLeft"));
        if (PS3.PS3Connected) {
          PS3.setLedOff();
          PS3.setLedOn(LED3);
        }
      }

      if (PS3.getButtonClick(L1))
        Serial.print(F("\r\nL1"));
      if (PS3.getButtonClick(L3))
        Serial.print(F("\r\nL3"));
      if (PS3.getButtonClick(R1))
        Serial.print(F("\r\nR1"));
      if (PS3.getButtonClick(R3))
        Serial.print(F("\r\nR3"));

      if (PS3.getButtonClick(SELECT)) {
        Serial.print(F("\r\nSelect - "));
        PS3.printStatusString();
      }
      if (PS3.getButtonClick(START)) {
        Serial.print(F("\r\nStart"));
        printAngle = !printAngle;
      }
    }
#if 0 // Set this to 1 in order to see the angle of the controller
    if (printAngle) {
      Serial.print(F("\r\nPitch: "));
      Serial.print(PS3.getAngle(Pitch));
      Serial.print(F("\tRoll: "));
      Serial.print(PS3.getAngle(Roll));
    }
#endif
  
#if 0 // Set this to 1 in order to enable support for the Playstation Move controller
  else if (PS3.PS3MoveConnected) {
    if (PS3.getAnalogButton(T)) {
      Serial.print(F("\r\nT: "));
      Serial.print(PS3.getAnalogButton(T));
    }
    if (PS3.getButtonClick(PS)) {
      Serial.print(F("\r\nPS"));
      PS3.disconnect();
    }
    else {
      if (PS3.getButtonClick(SELECT)) {
        Serial.print(F("\r\nSelect"));
        printTemperature = !printTemperature;
      }
      if (PS3.getButtonClick(START)) {
        Serial.print(F("\r\nStart"));
        printAngle = !printAngle;
      }
      if (PS3.getButtonClick(TRIANGLE)) {
        Serial.print(F("\r\nTriangle"));
        PS3.moveSetBulb(Red);
      }
      if (PS3.getButtonClick(CIRCLE)) {
        Serial.print(F("\r\nCircle"));
        PS3.moveSetBulb(Green);
      }
      if (PS3.getButtonClick(SQUARE)) {
        Serial.print(F("\r\nSquare"));
        PS3.moveSetBulb(Blue);
      }
      if (PS3.getButtonClick(CROSS)) {
        Serial.print(F("\r\nCross"));
        PS3.moveSetBulb(Yellow);
      }
      if (PS3.getButtonClick(MOVE)) {
        PS3.moveSetBulb(Off);
        Serial.print(F("\r\nMove"));
        Serial.print(F(" - "));
        PS3.printStatusString();
      }
    }
    if (printAngle) {
      Serial.print(F("\r\nPitch: "));
      Serial.print(PS3.getAngle(Pitch));
      Serial.print(F("\tRoll: "));
      Serial.print(PS3.getAngle(Roll));
    }
    else if (printTemperature) {
      Serial.print(F("\r\nTemperature: "));
      Serial.print(PS3.getTemperature());
    }
  }
#endif
}

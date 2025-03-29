
#include <PS5BT.h>
#include <usbhub.h>
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

USB Usb;
//USBHub Hub1(&Usb); // Some dongles have a hub inside
BTD Btd(&Usb); // You have to create the Bluetooth Dongle instance like so
PS5BT PS5(&Btd, PAIR);

bool printAngle = false, printTouch = false;
uint16_t lastMessageCounter = -1;
uint8_t player_led_mask = 0;
bool microphone_led = false;
uint32_t ps_timer;

int dir[] = { 17, 15};  //17,13 are right wheel
int pwm[] = { 13, 12 };  //15,12 are left wheel
int dwnwd, upwd, rtwd, lftwd;
void setup() {
  Serial.begin(115200);
#if !defined(__MIPSEL__)
  while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#endif
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1);
  }
  Serial.print(F("\r\nPS5 Bluetooth Library Started"));
  
  for (int i = 0; i < 2; i++) {
    pinMode(dir[i], OUTPUT);
    pinMode(pwm[i], OUTPUT);
  }
  
}

void loop() {
  Usb.Task();

    
    upwd = map(PS5.getAnalogHat(LeftHatY), 0, 117, 100, 0);
    dwnwd = map(PS5.getAnalogHat(LeftHatY), 137, 255, 0, 100);
    rtwd = map(PS5.getAnalogHat(RightHatX), 117, 0, 0, 100);
    lftwd = map(PS5.getAnalogHat(RightHatX), 137, 255, 0, 100);

  if (PS5.connected() && lastMessageCounter != PS5.getMessageCounter()) {
    lastMessageCounter = PS5.getMessageCounter();

    if ( (PS5.getAnalogHat(LeftHatY) < 137 && PS5.getAnalogHat(LeftHatY) > 117) &&( PS5.getAnalogHat(RightHatX) < 137 && PS5.getAnalogHat(RightHatX) > 117 ))
    {
      for (int i = 0; i < 2; i++) 
        {
            analogWrite(pwm[i], 0);
        }
    }
    if(PS5.getAnalogHat(LeftHatY)>137)
    {
     //front 
     if(PS5.getAnalogHat(RightHatX)>137)
     {
      //right
     }
     else
     {
      
     }
    }
    if(PS5.getAnalogHat(LeftHatY)<117)
    {
     //back
     
    }
    

    
    if (PS5.getButtonPress(PS)) {
      if (millis() - ps_timer > 1000)
        PS5.disconnect();
    } else
      ps_timer = millis();

    if (PS5.getButtonClick(PS))
      Serial.print(F("\r\nPS"));
      
  }
}

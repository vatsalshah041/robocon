/*
 Example sketch for the PS5 Bluetooth library - developed by Kristian Sloth Lauszus
 For more information visit the Github repository: github.com/felis/USB_Host_Shield_2.0 or
 send me an e-mail: lauszus@gmail.com
 */
int prev = 0; int a=0;
  int dir1[] = {15, 17};
int d[] = {1,0};
int pwm1[] = {5, 4};
#include <PS5BT.h>
#include <usbhub.h>

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

USB Usb;
//USBHub Hub1(&Usb); // Some dongles have a hub inside
BTD Btd(&Usb); // You have to create the Bluetooth Dongle instance like so

/* You can create the instance of the PS5BT class in two ways */
// This will start an inquiry and then pair with the PS5 controller - you only have to do this once
// You will need to hold down the PS and Share button at the same time, the PS5 controller will then start to blink rapidly indicating that it is in pairing mode
//PS5BT PS5(&Btd, PAIR);

// After that you can simply create the instance like so and then press the PS button on the device
PS5BT PS5(&Btd);

bool printAngle = false, printTouch = false;
uint16_t lastMessageCounter = -1;
uint8_t player_led_mask = 0;
bool microphone_led = false;
uint32_t ps_timer;

void setup() {
  Serial.begin(115200);
#if !defined(__MIPSEL__)
  while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#endif
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1); // Halt
  }
  Serial.print(F("\r\nPS5 Bluetooth Library Started"));
  digitalWrite(dir1[0], d[0]);
    digitalWrite(dir1[1], d[1]);
}

void loop() {
  
  Usb.Task();

  if (PS5.connected() && lastMessageCounter != PS5.getMessageCounter()) {
    lastMessageCounter = PS5.getMessageCounter();

   
    if (PS5.getButtonPress(PS)) {
      if (millis() - ps_timer > 1000)
        PS5.disconnect();
    } else
      ps_timer = millis();

    if (PS5.getButtonClick(PS))
      Serial.print(F("\r\nPS"));

      
    if (PS5.getButtonClick(CROSS)) {
      for (int i = prev; i >= 0; i -= 5)
      {
        analogWrite(pwm1[0], i);
        analogWrite(pwm1[1], i);
        delay(15);
        a=i;
      }
      prev=a;
    }
    
    if (PS5.getButtonClick(TRIANGLE)) {
      
      for(int i=prev;i<=255;i+=5)
      {
         analogWrite(pwm1[0], i);
        analogWrite(pwm1[1], i);
        delay(15);
        a=i;
      }
      prev=a;
    }
    
    

   if (PS5.getButtonClick(UP)) {
      if(prev<=230){
      for(int i=prev;i<=prev+25;i+=5)
      {
         analogWrite(pwm1[0], i);
        analogWrite(pwm1[1], i);
        delay(15);
        a=i;
      }
      prev=a;
      }
        
    }
   if (PS5.getButtonClick(DOWN)) {
      if(prev>=25){
      for(int i=prev;i>=prev-25;i-=5)
      {
         analogWrite(pwm1[0], i);
        analogWrite(pwm1[1], i);
        delay(15);
        a=i;
      }
      prev=a;
      }
    } 
    
      }
    }
  

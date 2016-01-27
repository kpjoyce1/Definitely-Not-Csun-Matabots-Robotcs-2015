//
// begin license header
//
// This file is part of Pixy CMUcam5 or "Pixy" for short
//
// All Pixy source code is provided under the terms of the
// GNU General Public License v2 (http://www.gnu.org/licenses/gpl-2.0.html).
// Those wishing to use Pixy source code, software and/or
// technologies under different licensing terms should contact us at
// cmucam@cs.cmu.edu. Such licensing terms are available for
// all portions of the Pixy codebase presented here.
//
// end license header
//
// This sketch is a good place to start if you're just getting started with 
// Pixy and Arduino.  This program simply prints the detected object blocks 
// (including color codes) through the serial console.  It uses the Arduino's 
// ICSP port.  For more information go here:
//
// http://cmucam.org/projects/cmucam5/wiki/Hooking_up_Pixy_to_a_Microcontroller_(like_an_Arduino)
//
// It prints the detected blocks once per second because printing all of the 
// blocks for all 50 frames per second would overwhelm the Arduino's serial port.
//
   
#include <SPI.h>  
#include <Pixy.h>

// This is the main Pixy object 
Pixy pixy;

void setup()
{
  Serial.begin(115200);
  pixy.init();
}

void loop()
{ 
  static int i = 0;
  int j;
  uint16_t blocks;
  char buf[32]; 
  
  // grab blocks!
  blocks = pixy.getBlocks();
  
  // If there are detect blocks, print them!
  if (blocks)
  {
    i++;
    
    // do this (print) every 50 frames because printing every
    // frame would bog down the Arduino
    if (i%2==0)
    {

      bool ballExists = false;
      for (j=0; j<blocks; j++)
      {
        if(pixy.blocks[j].width > 15 && pixy.blocks[j].height > 15)
        {
          unsigned char sig = pixy.blocks[j].signature == 1 ? 0x47 : 0x4F;
          String x = String(pixy.blocks[j].x, DEC);
          String y = String(pixy.blocks[j].y, DEC);

          if(x.toInt() < 255 && y.toInt() < 255)
          {
            Serial.write(0x5B);
            Serial.write(sig);
            Serial.write(0x5F);
            for(int i = 0; i < x.length(); i++)
            {
              Serial.write(x[i]);
            }
            Serial.write(0x5F);
            for(int i = 0; i < y.length(); i++)
            {
              Serial.write(y[i]);
            }
            Serial.write(0x5D);
          }
          ballExists = true;
        }     
        
      }
      
      if(!ballExists)
      {         
          Serial.write(0x5B);
          Serial.write('N');
          Serial.write(0x5F);
          Serial.write(-1);
          Serial.write(0x5F);
          Serial.write(-1);
          Serial.write(0x5F); 
          Serial.write(0x5D);         
      }
    }
   
  }  
}


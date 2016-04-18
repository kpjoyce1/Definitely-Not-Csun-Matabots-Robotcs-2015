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
uint16_t prevBlocks;
int state = 0;
// 0 - search for ball
// 1 - search for goal
void setup()
{
  Serial.begin(19200);
  pixy.init();
}




void loop()
{
  static int i = 0;
  uint16_t blocks;
  bool  ballExists = false;

  // grab blocks!
  blocks = pixy.getBlocks();
  // If there are detect blocks, print them!
  i++;

  if (i % 2 == 0)
  {
    if (blocks)
    {

      for (int j = 0; j < blocks; j++)
      {
        if (state == 0)
        {
          if (pixy.blocks[j].width > 15 && pixy.blocks[j].height > 15)
          {
            unsigned char sig = pixy.blocks[j].signature == 1 ? 'G' : 'O';
            if (sig == 'O' || sig == 'G')
            {
              float tempx = (pixy.blocks[j].x + pixy.blocks[j].width / 2);
              float tempy = (pixy.blocks[j].y + pixy.blocks[j].height / 2);

              tempy = (0.0022581)*tempy*tempy + (-0.96265)*tempy + 121.98;
              float a,b;
              if(tempy > 53)
              {
                a = 0.18346;
                b = -32.483;
              }
              else if(tempy > 49)
              {
                a = 0.16966;
                b = -30.391;  
              }
              else if(tempy > 45)
              {
                a = 0.15061;
                b = -27.241;  
              }
              else if(tempy > 41)
              {
                a = 0.14038;
                b = -25.162;  
              }
              else if(tempy > 37)
              {
                a = 0.12643;
                b = -22.836;  
              }
              else if(tempy > 33)
              {
                a = 0.1074;
                b = -19.439;  
              }
              else if(tempy > 29)
              {
                a = 0.090698;
                b = -16.874;  
              }
              else if(tempy > 25)
              {
                a = 0.077751;
                b = -14.729;  
              }
              else if(tempy > 21)
              {
                a = 0.058115;
                b = -11.484;  
              }
              else //if(tempy > 17)
              {
                a = 0.043843;
                b = -8.4397;  
              }
              tempx = tempx * a + b;
              String x = String(int(round(tempx)));
              String y = String(int(round(tempy)));

              Serial.write(0x5B);
              delayMicroseconds(10);
              Serial.write(sig);
              delayMicroseconds(10);
              Serial.write(0x5F);
              delayMicroseconds(10);
              for (int i = 0; i < x.length(); i++)
              {
                Serial.write(x[i]);
                delayMicroseconds(10);
              }
              Serial.write(0x5F);
              for (int i = 0; i < y.length(); i++)
              {
                Serial.write(y[i]);
                delayMicroseconds(10);
              }
              Serial.write(0x5D);
              Serial.println();
            }
          }
        }
      }

    }
    else
    {
      delay(20);
      if (!pixy.getBlocks())
      {
        Serial.write(0x5B);
        Serial.write('N');
        Serial.write(0x5F);
        Serial.write(' - ');
        Serial.write('-');
        Serial.write('1');
        Serial.write(0x5F);
        Serial.write(' - ');
        Serial.write('-');
        Serial.write('1');
        Serial.write(0x5D);
      }
    }
  }

  delay(20);
}


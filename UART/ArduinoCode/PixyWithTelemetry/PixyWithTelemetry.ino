#include <SPI.h>  
#include <Pixy.h>
#include <Wire.h>

//Initialize Sensors
Pixy pixy;
int blockCount;



void PixyDetection();



void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

}

void loop() {

  
  PixyDetection();
  
  delay(50);
  

}

void PixyDetection()
{
  static int i = 0;
  int j = 0;
  uint16_t blocks;
  char buf[32]; 
  
  // grab blocks!
  blocks = pixy.getBlocks();
  blockCount = blocks;
  // If there are detect blocks, print them!
  if (blocks)
  {
    
    i++;
    
    // do this (print) every 50 frames because printing every
    // frame would bog down the Arduino
    if (i%50==0)
    {
      sprintf(buf, "Detected %d:\n", blocks);
      for (j=0; j<blocks; j++)
      {
        //sprintf(buf, "  block %d: ", j);
        Serial.print(buf); 
        //pixy.blocks[j].print();
        //Serial.println(pixy.blocks[j].signature);
      }
    }
  }   
}




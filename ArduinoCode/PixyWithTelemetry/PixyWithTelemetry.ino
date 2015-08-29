#include <SPI.h>  
#include <Pixy.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_L3GD20_U.h>
#include <Adafruit_LSM303_U.h>
#include <Adafruit_BMP085_U.h>
#include <Adafruit_10DOF.h>

float seaLevelPressure = SENSORS_PRESSURE_SEALEVELHPA;

//Initialize Sensors
Pixy pixy;
int blockCount;


Adafruit_10DOF                dof   = Adafruit_10DOF();
Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(30301);
Adafruit_LSM303_Mag_Unified   mag   = Adafruit_LSM303_Mag_Unified(30302);
Adafruit_BMP085_Unified       bmp   = Adafruit_BMP085_Unified(18001);

void initSensors();
void PixyDetection();

float currentHeading;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  initSensors();
}

void loop() {
  // put your main code here, to run repeatedly:
  sensors_event_t accel_event;
  sensors_event_t mag_event;
  sensors_event_t bmp_event;
  sensors_vec_t   orientation;
  
  mag.getEvent(&mag_event);
  if (dof.magGetOrientation(SENSOR_AXIS_Z, &mag_event, &orientation))
  {
    /* 'orientation' should have valid .heading data now */
   currentHeading = orientation.heading;
   //Serial.print(F("Heading: "));
   //Serial.print(orientation.heading);
   // Serial.println(F("; "));
  }
  
  PixyDetection();

  for(int i = 0; i < blockCount; i++)
  {
    //pixy.blocks[i].print(); 
    
  }
  
  Serial.print("^x\\y$");
  Serial.println();
  
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
        //Serial.print(buf); 
        //pixy.blocks[j].print();
        //Serial.println(pixy.blocks[j].signature);
      }
    }
  }   
}


void initSensors()
{
  pixy.init();
  
  if(!accel.begin())
  {
    /* There was a problem detecting the LSM303 ... check your connections */
    Serial.println(F("Ooops, no LSM303 detected ... Check your wiring!"));
    while(1);
  }
  if(!mag.begin())
  {
    /* There was a problem detecting the LSM303 ... check your connections */
    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    while(1);
  }
  if(!bmp.begin())
  {
    /* There was a problem detecting the BMP180 ... check your connections */
    Serial.println("Ooops, no BMP180 detected ... Check your wiring!");
    while(1);
  }
}

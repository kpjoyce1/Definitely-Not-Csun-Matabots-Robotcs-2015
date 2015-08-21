#include "I2Cdev.h"
#include "MPU9255.h"

// Arduino Wire library is required if I2Cdev I2CDEV_ARDUINO_WIRE implementation
// is used in I2Cdev.h
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif

// class default I2C address is 0x68
// specific I2C addresses may be passed as a parameter here
// AD0 low = 0x68 (default for InvenSense evaluation board)
// AD0 high = 0x69
MPU9255 accelgyromag(0x68, 0x0C);
//MPU6050 accelgyro(0x69); // <-- use for AD0 high

int16_t ax, ay, az;
int16_t gx, gy, gz;
int16_t mx, my, mz;



float scaleA(int16_t accel);
float getDirection(int16_t magx, int16_t magy);

#define OUTPUT_READABLE_ACCELGYRO

// uncomment "OUTPUT_BINARY_ACCELGYRO" to send all 6 axes of data as 16-bit
// binary, one right after the other. This is very fast (as fast as possible
// without compression or data loss), and easy to parse, but impossible to read
// for a human.
//#define OUTPUT_BINARY_ACCELGYRO


#define LED_PIN 13
bool blinkState = false;
float totalHeading = 0;
float iterations = 0;


void setup() {
    // join I2C bus (I2Cdev library doesn't do this automatically)
    #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
        Wire.begin();
    #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
        Fastwire::setup(400, true);
    #endif

  

    // initialize serial communication
    // (38400 chosen because it works as well at 8MHz as it does at 16MHz, but
    // it's really up to you depending on your project)
    Serial.begin(38400);

    // initialize device
    Serial.println("Initializing I2C devices...");
    accelgyromag.initialize();

    
    // verify connection
    Serial.println("Testing device connections...");
    Serial.println(accelgyromag.testConnection() ? "MPU9255 connection successful" : "MPU9255 connection failed");
    Serial.println(accelgyromag.testMagnometerConnection() ? "Magnometer connection successful" : "Magnometer connection failed");

    // use the code below to change accel/gyro offset values
    
    Serial.println("Updating internal sensor offsets...");
    // -76  -2359 1688  0 0 0
    Serial.print(accelgyromag.getXAccelOffset()); Serial.print("\t"); // -76
    Serial.print(accelgyromag.getYAccelOffset()); Serial.print("\t"); // -2359
    Serial.print(accelgyromag.getZAccelOffset()); Serial.print("\t"); // 1688
    Serial.print(accelgyromag.getXGyroOffset()); Serial.print("\t"); // 0
    Serial.print(accelgyromag.getYGyroOffset()); Serial.print("\t"); // 0
    Serial.print(accelgyromag.getZGyroOffset()); Serial.print("\t"); // 0
    Serial.print("\n");
    accelgyromag.setXGyroOffset(220);
    accelgyromag.setYGyroOffset(76);
    accelgyromag.setZGyroOffset(-85);
    Serial.print(accelgyromag.getXAccelOffset()); Serial.print("\t"); // -76
    Serial.print(accelgyromag.getYAccelOffset()); Serial.print("\t"); // -2359
    Serial.print(accelgyromag.getZAccelOffset()); Serial.print("\t"); // 1688
    Serial.print(accelgyromag.getXGyroOffset()); Serial.print("\t"); // 0
    Serial.print(accelgyromag.getYGyroOffset()); Serial.print("\t"); // 0
    Serial.print(accelgyromag.getZGyroOffset()); Serial.print("\t"); // 0
    Serial.print("\n");
    

    // configure Arduino LED for
    pinMode(LED_PIN, OUTPUT);
}

void loop() {
    // read raw accel/gyro measurements from device
    accelgyromag.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    accelgyromag.getHeading(&mx, &my, &mz);
    // these methods (and a few others) are also available
    //accelgyro.getAcceleration(&ax, &ay, &az);
    //accelgyro.getRotation(&gx, &gy, &gz);

   // #ifdef OUTPUT_READABLE_ACCELGYRO
        // display tab-separated accel/gyro x/y/z values
        Serial.print("a/g/m:\t");
        Serial.print(scaleA(ax)); Serial.print("\t");
        Serial.print(scaleA(ay)); Serial.print("\t");
        Serial.print(scaleA(az)); Serial.print("\t");
        Serial.print(gx); Serial.print("\t");
        Serial.print(gy); Serial.print("\t");
        Serial.print(gz); Serial.print("\t");
        Serial.print(mx); Serial.print("\t");
        Serial.print(my); Serial.print("\t");
      
        float heading = getDirection(mx,my);
        Serial.print(mz); Serial.print("\tHeading: ");
        Serial.print(heading);
        totalHeading += heading;
        iterations++;
        Serial.print(mz); Serial.print("\tAverage Heading: ");
        Serial.println(totalHeading / iterations);

        delay(500);
   //#endif 
/*
    //#ifdef OUTPUT_BINARY_ACCELGYRO
        Serial.write((uint8_t)(ax >> 8)); Serial.write((uint8_t)(ax & 0xFF));
        Serial.write((uint8_t)(ay >> 8)); Serial.write((uint8_t)(ay & 0xFF));
        Serial.write((uint8_t)(az >> 8)); Serial.write((uint8_t)(az & 0xFF));
        Serial.write((uint8_t)(gx >> 8)); Serial.write((uint8_t)(gx & 0xFF));
        Serial.write((uint8_t)(gy >> 8)); Serial.write((uint8_t)(gy & 0xFF));
        Serial.write((uint8_t)(gz >> 8)); Serial.write((uint8_t)(gz & 0xFF));
    //#endif
*/
    // blink LED to indicate activity
    //blinkState = !blinkState;
    //digitalWrite(LED_PIN, blinkState);
    
}

float scaleA(int16_t accel){

   return (float)accel / 16000.; 
}

float getDirection(int16_t magx, int16_t magy){
    float heading = atan2((double)magy, (double)magx) * 180.0/3.14159265 + 12.33;
    while (heading < 0) heading += 360;
    while (heading > 360) heading -= 360;

    return heading;
}




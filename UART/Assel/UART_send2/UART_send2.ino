#include <SoftwareSerial.h>
SoftwareSerial mySerial(2, 3);

byte toSend[6];       
byte toSend2[6]; 
int i = 0;
int j = 0; 

void setup()
{
  Serial.begin(9600);
  mySerial.begin(115200);
}

void loop()
{
  while (mySerial.available())
  {
    char inChar = (char)mySerial.read();
    //int flagI = 0;
    //Serial.println(inChar);
    if (inChar == '&')
    {
      toSend[0] = 'H';
      toSend[1] = 'E'; 
      toSend[2] = 'L'; 
      toSend[3] = 'L'; 
      toSend[4] = 'O'; 
      toSend[5] = '$'; //$ end of word
      //Serial.print(inChar);
      mySerial.write(toSend, 6);
      Serial.println(inChar);
    }
    else if (inChar == '%')
    {
      toSend2[0] = 'W'; 
      toSend2[1] = 'O';   
      toSend2[2] = 'R'; 
      toSend2[3] = 'L'; 
      toSend2[4] = 'D'; 
      toSend2[5] = '$';
      //Serial.print(inChar);
      mySerial.write(toSend2, 6);
      Serial.println(inChar);
    }
    delay(100);
  }
  delay(50);
} 

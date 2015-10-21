 void setup()
{
  Serial.begin(115200);
}

void loop()
{
    byte toSend[6];         //Header info
    byte toSend2[6];
//    toSend[0] = 0xAA;
//    toSend[1] = 0x55;
//    toSend[2] = 0x1e;
//    toSend[3] = 0x12;

    //Update data
    //toSend[0] = 0x02; //Light on, top row
    toSend[0] = 'H';//0x48; //H
    toSend[1] = 0x45; //E
    toSend[2] = 0x4C; //L
    toSend[3] = 0x4C; //L
    toSend[4] = 0x4F; //O
    toSend[5] = 0x24; //$ end of word

    toSend2[0] = 0x57; //W
    toSend2[1] = 0x4F; //O   
    toSend2[2] = 0x52; //R
    toSend2[3] = 0x4C; //L
    toSend2[4] = 0x44; //D
    toSend2[5] = 0x24;


//    //Calculate Checksum
//    byte cs = 0;
//    for(int i = 4; i < 7; i++) {
//      cs += toSend[i]; //Sum all the update vals 
//    }
//    cs = 0x100 - cs; //Update to sum to 0.
//
//    toSend[7] = cs;

    //Send to LCD
    Serial.write(toSend, 6);
    delay(2000);
    Serial.write(toSend2, 6);
    delay(2000);
} 

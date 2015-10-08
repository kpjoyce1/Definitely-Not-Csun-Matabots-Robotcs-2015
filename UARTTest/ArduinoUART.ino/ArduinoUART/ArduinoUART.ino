void setup() {
  // put your setup code here, to run once:
  Serial.begin(38400);
}


void loop() {
  // put your main code here, to run repeatedly:
  
  for(byte i = 0; i < 256; i++)
  {
   Serial.write(i); 
  }

}

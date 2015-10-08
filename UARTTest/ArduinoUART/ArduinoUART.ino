void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
}


void loop() {
  // put your main code here, to run repeatedly:
  
  Serial.write('S');
  Serial.write('W');
  Serial.write('A');
  Serial.write('G');

  delay(10);
}

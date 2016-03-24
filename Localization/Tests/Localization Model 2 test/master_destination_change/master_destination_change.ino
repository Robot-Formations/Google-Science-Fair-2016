char* SerialAddressHigh[]= {"13A200","13A200","13A200","13A200"};
char* SerialAddressLow[]= {"4090F21D","40E5BD4A","40B41AF9","40D93FCE"};

void serialFlush(){
  while(Serial1.available() > 0) {
    char t = Serial1.read();
  }
}   

void setup() {
  Serial1.begin(9600);
  Serial.begin(9600);
}
void loop() {
  delay(500);
  Serial1.print('b');
  for(int i = 0; i<4; i++){
    delay(1500);
    Serial1.print("+++");
    delay(3010);
    Serial1.print("ATDH ");
    Serial1.println(SerialAddressHigh[i]);
    //delay(500);
    Serial1.print("ATDL ");
    Serial1.println(SerialAddressLow[i]);
    //delay(500);
    Serial1.println("ATDL");
    //delay(500);
    Serial1.println("ATCN");
    delay(500);
    Serial1.flush();
    serialFlush();
    Serial1.print('s');    
    //delay(500);
    while(!Serial1.available());
    while(Serial1.available())
      Serial.print((char)Serial1.read());
    //delay(500);
    //Serial.print("robot:");
    //Serial.println(i);
 }
    delay(1500);
    Serial1.print("+++");
    delay(3010);
    Serial1.print("ATDH ");
    Serial1.println(0);
    //delay(500);
    Serial1.print("ATDL ");
    Serial1.println(0);
    //delay(500);
    Serial1.println("ATCN");
    //delay(500);

}

#include <fix_fft.h>
#include <Time.h>
#include <SoftwareSerial.h>

char im[128], data[128], lastpass[64];
char x=32, ylim=90;
int i=0,val;
SoftwareSerial GPS(2,3);

void setup(){
  Serial.begin(9600);
  GPS.begin(9600);
  analogReference(DEFAULT);                          //  Use default (5v) aref voltage. 
  for (int z=0; z<64; z++) {lastpass[z]=80;};       //  fill the lastpass[] array with dummy data
}

void loop(){
  if(digitalRead(4)==HIGH){
    while(GPS.read()!='\r'){
      Serial.write(GPS.read());
    }
    Serial.print(",");
    for (i=0; i < 128; i++){
      val = analogRead(0);
      data[i] = val/4 -128;
      im[i] = 0;
    }
    fix_fft(data,im,7,0);
    Serial.print(hour());
    Serial.print(minute());
    Serial.print(second());
    Serial.print(",");
    Serial.print(millis());
    Serial.print(",");
    for (i=0; i< 64;i++){
      data[i] = sqrt(data[i] * data[i] + im[i] * im[i]);
      Serial.print(data[i]);
      if(i<63){
        Serial.print(",");
      }
      else{
        Serial.println();
      }
      lastpass[i]=ylim-data[i];
    }
  }
}

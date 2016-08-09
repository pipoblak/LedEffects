#include <Adafruit_NeoPixel.h>
#include <Thread.h>
#include <ThreadController.h>
#define PIN 0
#define NUM_LEDS 8

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);
boolean ida=true;
int i=1;
int ID=0,DELAY=0;
ThreadController controll = ThreadController();
Thread threadRead = Thread();
Thread threadLight = Thread();
ThreadController groupOfThreads = ThreadController();
uint16_t contI = 0, contJ = 0;
String Red,Green,Blue,Speed="10";
int contTempo=0;

void readCallback(){

if (Serial.available() > 0) {
  unsigned long currentMillis = millis();
  String recivedDataStr="";
  char recivedChar;
  while(Serial.available() > 0){
    recivedChar = Serial.read();
     if (recivedChar != '\n'){
      // Concatena valores
      recivedDataStr.concat(recivedChar);
      delay(10);
    }
  }

  //É UM DISPOSITIVO COLOREYEZE?
    if(recivedDataStr.indexOf("$")>=0){
      Serial.println("Coloreyeze Device PC");
    }
    
  //LEITURA RGB
    if(recivedDataStr.indexOf("#")>=0){
     int indexR = recivedDataStr.indexOf("R");
     int indexG = recivedDataStr.indexOf("G");
     int indexB = recivedDataStr.indexOf("B");
     int indexSize = recivedDataStr.length();
     int str_len = recivedDataStr.length() + 1; 
     char char_array[str_len];
     recivedDataStr.toCharArray(char_array, str_len);
     Red="";
     Green="";
     Blue="";
     for(int cont=indexR+1;cont<indexG;cont++){
       Red.concat(char_array[cont]);
       }
     for(int cont=indexG+1;cont<indexB;cont++){
       Green.concat(char_array[cont]);
       }
     for(int cont=indexB+1;cont<indexSize+1;cont++){
       Blue.concat(char_array[cont]);
       }
    }

    //LEITURA DE EVENTO
    if(recivedDataStr.indexOf("@")>=0){
     int indexS = recivedDataStr.indexOf("S");
     int indexId = recivedDataStr.indexOf("@");
     int indexSize = recivedDataStr.length();
     int str_len = recivedDataStr.length() + 1; 
     char char_array[str_len];
     recivedDataStr.toCharArray(char_array, str_len);
     Speed="0";
     String idEvent="";
     for(int cont=indexId+1;cont<indexS+1;cont++){
       idEvent.concat(char_array[cont]);
       }
     for(int cont=indexS+1;cont<indexSize+1;cont++){
       Speed.concat(char_array[cont]);
       }
     ID=idEvent.toInt();
     DELAY=Speed.toInt();
     Serial.println(DELAY);
     }


  //ESPAÇO PARA UM OUTRO EVENTO
    
}
}

void lightCallback(){
    
  if(ID==0){
    rainbowCycleS(DELAY);
  }
  else if(ID==1){
    
  }
  

}


void setup() {
  Serial.begin(115200);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  
  threadRead.onRun(readCallback);
  threadRead.setInterval(1);

  threadLight.onRun(lightCallback);
  threadLight.setInterval(1);
  
  controll.add(&threadRead);
  
  groupOfThreads.add(&threadLight);

  controll.add(&groupOfThreads);
  
  
}
  
// *** REPLACE FROM HERE ***
void loop() { 
controll.run();
 //rainbowCycle(20);

        
 // ---> here we call the effect function <---
//doARun(20);
//NewKITT(0xff, 0, 0, 2, 10, 50);
//Fire(55,120,15);

}

// ---> here we define the effect function <---
// *** REPLACE TO HERE ***

void showStrip() {
 #ifdef ADAFRUIT_NEOPIXEL_H 
   // NeoPixel
   strip.show();
 #endif
 #ifndef ADAFRUIT_NEOPIXEL_H
   // FastLED
   FastLED.show();
 #endif
}

void setPixel(int Pixel, byte red, byte green, byte blue) {
 #ifdef ADAFRUIT_NEOPIXEL_H 
   // NeoPixel
   strip.setPixelColor(Pixel, strip.Color(red, green, blue));
 #endif
 #ifndef ADAFRUIT_NEOPIXEL_H 
   // FastLED
   leds[Pixel].r = red;
   leds[Pixel].g = green;
   leds[Pixel].b = blue;
 #endif
}

void setAll(byte red, byte green, byte blue) {
  for(int i = 0; i < NUM_LEDS; i++ ) {
    setPixel(i, red, green, blue); 
  }
  showStrip();
}

void doARun(int d){

    delay(d);
  if(ida==true){
        setPixel(i,255,255,255);
        setPixel(i-1,0,0,0);
        showStrip();
    if(i==NUM_LEDS){
      ida=false;
      i=i-1;}
    else{ 
            i=i+1;
    }
        
  }
   else{
        setPixel(i,255,255,255);
        setPixel(i+1,0,0,0);
        showStrip();
        i=i-1;
        if (i==0){
            ida=true;
            setAll(0,0,0);
            i=i+1;
        }
    
    
    }    
       
  
  }

void rainbowCycleS(int SpeedDelay) {
  byte *c;
    contTempo++;
   if (contTempo >= SpeedDelay){
    
    for(i=0; i< NUM_LEDS; i++) {
      c=Wheel(((i * 256 / NUM_LEDS) + contJ) & 255);
      setPixel(i, *c, *(c+1), *(c+2));
  
    }
   
     contTempo=0;
      contJ++;
      showStrip();
  }

} 
 


byte * Wheel(byte WheelPos) {
  static byte c[3];
  
  if(WheelPos < 85) {
   c[0]=WheelPos * 3;
   c[1]=255 - WheelPos * 3;
   c[2]=0;
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   c[0]=255 - WheelPos * 3;
   c[1]=0;
   c[2]=WheelPos * 3;
  } else {
   WheelPos -= 170;
   c[0]=0;
   c[1]=WheelPos * 3;
   c[2]=255 - WheelPos * 3;
  }

  return c;
}

#include <Adafruit_NeoPixel.h>
#include <Thread.h>
#include <ThreadController.h>
#define PIN 0
#define NUM_LEDS 8

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);
boolean ida=true;
int i=1;
int ID=0,DELAY=10;
ThreadController controll = ThreadController();
Thread threadRead = Thread();
Thread threadLight = Thread();
ThreadController groupOfThreads = ThreadController();
uint16_t contI = 0, contJ = 0;
String Red,Green,Blue;

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
  threadLight.setInterval(10);
  
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
void Fire(int Cooling, int Sparking, int SpeedDelay) {
  static byte heat[NUM_LEDS];
  int cooldown;
  
  // Step 1.  Cool down every cell a little
  for( int i = 0; i < NUM_LEDS; i++) {
    cooldown = random(0, ((Cooling * 10) / NUM_LEDS) + 2);
    
    if(cooldown>heat[i]) {
      heat[i]=0;
    } else {
      heat[i]=heat[i]-cooldown;
    }
  }
  
  // Step 2.  Heat from each cell drifts 'up' and diffuses a little
  for( int k= NUM_LEDS - 1; k >= 2; k--) {
    heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2]) / 3;
  }
    
  // Step 3.  Randomly ignite new 'sparks' near the bottom
  if( random(255) < Sparking ) {
    int y = random(7);
    heat[y] = heat[y] + random(160,255);
    //heat[y] = random(160,255);
  }

  // Step 4.  Convert heat to LED colors
  for( int j = 0; j < NUM_LEDS; j++) {
    setPixelHeatColor(j, heat[j] );
  }

  showStrip();
  delay(SpeedDelay);
}

void setPixelHeatColor (int Pixel, byte temperature) {
  // Scale 'heat' down from 0-255 to 0-191
  byte t192 = round((temperature/255.0)*191);
 
  // calculate ramp up from
  byte heatramp = t192 & 0x3F; // 0..63
  heatramp <<= 2; // scale up to 0..252
 
  // figure out which third of the spectrum we're in:
  if( t192 > 0x80) {                     // hottest
    setPixel(Pixel, 255, 255, heatramp);
  } else if( t192 > 0x40 ) {             // middle
    setPixel(Pixel, 255, heatramp, 0);
  } else {                               // coolest
    setPixel(Pixel, heatramp, 0, 0);
  }
}
/*
void rainbowCycle(int SpeedDelay) {
  byte *c;
  uint16_t i, j;
unsigned long currentMillis = millis();

contI++;


  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
   
    
      
    for(i=0; i< NUM_LEDS; i++) {

      c=Wheel(((i * 256 / NUM_LEDS) + j) & 255);
      setPixel(i, *c, *(c+1), *(c+2));
  
    }
  
     showStrip();    
    
    }
    

 
       
       
} */

void rainbowCycleS(int SpeedDelay) {
  byte *c;
  
unsigned long currentMillis = millis();
   
   for(i=0; i< NUM_LEDS; i++) {
      c=Wheel(((i * 256 / NUM_LEDS) + contJ) & 255);
      setPixel(i, *c, *(c+1), *(c+2));
  
    }
    if (currentMillis <= (currentMillis + SpeedDelay)){
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



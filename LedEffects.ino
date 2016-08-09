#include <Adafruit_NeoPixel.h>
#include <Thread.h>
#include <ThreadController.h>
#define PIN 0
#define NUM_LEDS 8

#define PIN2 2
#define NUM_LEDS2 10

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);

Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(NUM_LEDS2, PIN2, NEO_GRB + NEO_KHZ800);

boolean ida=true;
int i=1;
int ID=0,DELAY=10;
int EventID1=0,EventID2=0;
ThreadController controll = ThreadController();
Thread threadRead = Thread();
Thread threadLight = Thread();
ThreadController groupOfThreads = ThreadController();
uint16_t contJ1 = 0, contJ2 = 0;
String Red,Green,Blue,Speed="10";
int contTempo1=0,contTempo2=0;

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

    // @(ID_EVENTO)D(DELAY)S(STRIP)
    if(recivedDataStr.indexOf("@")>=0){
    //  Serial.print("\nSpeed ");     Serial.print(DELAY);       Serial.print(" EventID1 ");     Serial.print(EventID1);     Serial.print(" EventID2 ");     Serial.print(EventID2);

     int indexId = recivedDataStr.indexOf("@");
     int indexD = recivedDataStr.indexOf("D");
     int indexS = recivedDataStr.indexOf("S");
     int indexSize = recivedDataStr.length();
     int str_len = recivedDataStr.length() + 1; 
     char char_array[str_len];
     recivedDataStr.toCharArray(char_array, str_len);
     Speed="";
     String idEvent="",idStrip="";
     
     for(int cont=indexId+1;cont<indexD;cont++){
       idEvent.concat(char_array[cont]);
       }
     
     for(int cont=indexD+1;cont<indexS;cont++){
       Speed.concat(char_array[cont]);
       }
       
     for(int cont=indexS+1;cont<indexSize+1;cont++){
       idStrip.concat(char_array[cont]);
       }
     ID=idEvent.toInt();
     DELAY=Speed.toInt();
     int idStri = idStrip.toInt();
     
     if (idStri == 1){
      EventID1=ID;
     }
     else if (idStri == 2){
      EventID2=ID;
     }
     else if (idStri == 0){
      EventID1=ID;
      EventID2=ID;
      contTempo1=0;
      contTempo2=0;
      }

   // Serial.print("\nEvento "); Serial.print(ID);     Serial.print(" Speed ");     Serial.print(DELAY);       Serial.print(" Strip ");  Serial.print(idStri);     Serial.print(" EventID1 ");     Serial.print(EventID1);     Serial.print(" EventID2 ");     Serial.print(EventID2);
     }


  //ESPAÇO PARA UM OUTRO EVENTO
    
}
}

void lightCallback(){

  if(EventID1==0){
    rainbowCycleS(DELAY,1);
  }
  else if(EventID1==1){
    
  }

  if(EventID2==0){
    rainbowCycleS(DELAY,2);
  }
  else if(EventID1==1){
    
  }


}


void setup() {
  Serial.begin(115200);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  strip2.begin();
  strip2.show(); // Initialize all pixels to 'off'
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

}


void showStrip(int stripID) {
 #ifdef ADAFRUIT_NEOPIXEL_H 
   // NeoPixel
  if(stripID==1){
    strip.show(); 
    }
  else if (stripID==2){
    strip2.show();
    }
   
 #endif
 #ifndef ADAFRUIT_NEOPIXEL_H
   // FastLED
   FastLED.show();
 #endif
}

void setPixel(int Pixel, byte red, byte green, byte blue,int stripID) {
 #ifdef ADAFRUIT_NEOPIXEL_H 
   // NeoPixel
    if(stripID==1){
     strip.setPixelColor(Pixel, strip.Color(red, green, blue));
    }
  else if (stripID==2){
     strip2.setPixelColor(Pixel, strip2.Color(red, green, blue));
    }
   
  
 #endif
 #ifndef ADAFRUIT_NEOPIXEL_H 
   // FastLED
   leds[Pixel].r = red;
   leds[Pixel].g = green;
   leds[Pixel].b = blue;
 #endif
}

void setAll(byte red, byte green, byte blue,int stripID) {
  
  for(int i = 0; i < NUM_LEDS; i++ ) {
    setPixel(i, red, green, blue,stripID); 
  }
  showStrip(stripID);
}

void doARun(int d,int stripID){

  if(ida==true){
        setPixel(i,255,255,255,stripID);
        setPixel(i-1,0,0,0,stripID);
        showStrip(stripID);
    if(i==NUM_LEDS){
      ida=false;
      i=i-1;}
    else{ 
            i=i+1;
    }
        
  }
   else{
        setPixel(i,255,255,255,stripID);
        setPixel(i+1,0,0,0,stripID);
        showStrip(stripID);
        i=i-1;
        if (i==0){
            ida=true;
            setAll(0,0,0,stripID);
            i=i+1;
        }
    
    
    }    
       
  
  }

void rainbowCycleS(int SpeedDelay,int stripID) {
  byte *c;
  int contTempo;
  int numberLeds;
  int cont;
  
  if(stripID==1){
    contTempo=contTempo1;
    contTempo1++;
    numberLeds=NUM_LEDS;
    cont=contJ1;
    }
  if(stripID==2){
    contTempo=contTempo2;
    contTempo2++;
    numberLeds=NUM_LEDS2;
    cont=contJ2;
    }
    
   if (contTempo >= SpeedDelay){

    for(i=0; i< numberLeds; i++) {
      c=Wheel(((i * 256 / numberLeds) + cont) & 255);
      setPixel(i, *c, *(c+1), *(c+2),stripID);
  
    }
   
    contTempo=0;
    if(stripID==1){
      contTempo1=contTempo;
      contJ1++;
    }
    if(stripID==2){
      contTempo2=contTempo;
      contJ2++;
    }
    
    showStrip(stripID);
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

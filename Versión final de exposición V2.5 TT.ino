 /*TINY TELEMETRY V 1.8 - ONLY USING BMP 280, NANO, & OLED for simple black box recording.
 [EN] To be able to store telemetry data in a black box style system (to be retrieved after flight).
 [ES] Para poder guardar datos telemétricos en un sistema estilo caja negra (a recuperarse después del vuelo).*/

#include <Adafruit_GFX.h>      //Libraries for the OLED and BMP280
#include <Adafruit_SSD1306.h> //Library for OLED adafruit.
#include <Adafruit_BMP280.h> //Library for BMP280 baro (see note above).
#include <Adafruit_Sensor.h> //Common IC2 and adafruit sensor libraries.

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET    4 // Reset pin # (or -1 if sharing Arduino reset pin)

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET); //Declaring the display name (display)
Adafruit_BMP280 bmp;

//Set variables to 0 for once. (AltMax is used later and any other initial value tha 0 will break down the code.)
  float StartTime = 0;
  float EndTime   = 0;
  float A         = 0;
  float altmax    = 0;
  float AltInic   = 0;

  bool ReadAlt    = 1;
  bool ReadTime   = 1;

  float PInic = 0;
  float PRun = 0;

void setup() {  

  bmp.begin();                                //Start the bmp                  
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); //Start the OLED display
  display.clearDisplay();
  display.display();
  display.setTextColor(WHITE); //Splash screen of TINY TELEMETRY in two lines.
  display.setTextSize(1); 
  display.setCursor(0,12);
  display.setTextSize(2);          
  display.println("TINY"); 
  display.setCursor(6,28);
  display.setTextSize(2);          
  display.println("TELEMETRY"); 
  display.display();
  delay(1000);
  display.setTextSize(1);          
  display.println("By Fgutep _"); 
  display.display();
  delay(1500);
  
  AltInic = bmp.readAltitude(1019.66); 
  PInic = bmp.readPressure()/100;
  
  display.clearDisplay();
  display.setCursor(0,12);
  display.setTextSize(2); 
  display.println("COUNTDOWN"); 
  display.setCursor(6,28);
  display.setTextSize(2);          
  display.println("20 SEC"); 
  display.display();
  delay(20000); 
}

void loop() {
    display.clearDisplay();
    
    A = bmp.readAltitude(1019.66); 
    PRun = bmp.readPressure()/100;
    
    if (A > altmax){
      altmax = A;
    }
  
    if ( (PRun >= PInic + 1 || PRun <= PInic - 1) &&  ReadTime) { 
        StartTime = millis();
        ReadTime = 0;
    }
   
    if ( not(ReadTime) and ReadAlt && (millis() >= StartTime + 2500)){ 
      if ((PRun >= PInic - 0.5 || PRun <= PInic + 0.5)) {
          EndTime = millis();
          ReadAlt = 0;
      }
    }
    /* THIS IS HOW STUFF IS DISPLAYED ON THE OLED, CHANGE (X,Y) IF YOUR OLED IS DIFFERENT OR WANT DIFFERENT TEXT*/
    
    // This first block of code inserts the first line ">/TinyTelemetry" if you need this extra line comment or change it to whatever variable/text suits your case. We like the look of it tho ;).
    /*display.setTextSize(1); //Text size of 1, it changes in INTEGER intervals.
    display.setCursor(0,0); // Pixel coordinates (x,y) where the text is written out.
    display.print(">/TinyTelemetry"); // Text to be written out. */
    
    //This block prints the difference in betweeen the end time stored and the start time stored variables.
    display.setTextSize(1); //128 x 64

// Primera línea    

    display.setCursor(0,0); 
    display.print("AltInic =");
    display.setCursor(60,0);
    display.print(AltInic);

// Segunda línea    
    
    display.setCursor(0,11);
    display.print("AltMax  =");
    display.setCursor(60,11);
    display.print(altmax);

// Tercera línea    

    display.setCursor(0,22); 
    display.print("StrTime =");
    display.setCursor(60,22);
    display.print(StartTime);

// Cuarta línea    

    display.setCursor(0,33);
    display.print("EndTime =");
    display.setCursor(60,33);
    display.print(EndTime);

// Quinta línea    

    display.setCursor(0,44);
    display.print("TimeTot =");
    display.setCursor(60,44);
    display.print(abs(EndTime - StartTime)/1000);   

// Sexta línea    

//    display.setCursor(0,55);
//    display.print("PRun = ");
    display.setCursor(0,55);
    display.print(PRun);   
    display.setCursor(60,55);
    display.print(PInic);   

    display.display();
    delay(50);
}

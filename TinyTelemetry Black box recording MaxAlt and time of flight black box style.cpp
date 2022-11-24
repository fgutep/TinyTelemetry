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
  display.setCursor(0,22);
  display.setTextSize(2);          
  display.println("TELEMETRY"); 
  display.display();
  delay(2000);
}

//Set variables to 0 for once. (AltMax is used later and any other initial value tha 0 will break down the code.)
  float altmax = 0;
  float AltInic = 0;
  float StartTime = 0;
  float EndTime = 0;

void loop() {

    display.clearDisplay();
    float P = bmp.readPressure()/100; //Read Pressure in Pa and conversion to hPa
    float A = bmp.readAltitude(1019.66); //Calculating the Altitude, the "1019.66" is the pressure in (hPa) at sea level at day in your region //If you don't know it just modify it until you get the altitude of your place.
    
    // If sequence for saving the max historical altitude, it basically keeps comparing a saved value "altmax" (initially = 0) to the just previously one, if greater, it stores it as "altmax".
    if (A > altmax){
      altmax = A;
      }
      
    // If the millis() (i.e.) the the time the arduino starts counting as soon as it starts executing code IS LESS than 5000 ms (educated assumption) then record the current altitude in "Altinic".
    if (millis() < 5000) {
      AltInic = A;
    }
   // If the current altitude (plus an error margin for barometric presure fluctuations) of "1" meter is detected, then record the current millis() value in "StartTime". 
    if (A+1 > AltInic){ // Note: Adjust freely the error margin for barometric fluctuations according to the precission of your own sensor. More precise sensor = A lower margin error = more precise time recording.
      StartTime = millis();
    }
    // If (when) the Telemetry falls back to it's inicial level (PLUS MINUS one meter (ERROR MARGIN)) then record millis() in Variable "EndTime".
    if (AltInic = (A-1 <= A <= A+1)){ //Note: Yet again, adjust this plusminus range according to your sensor's precision. More precise sensor = A lower margin error = More precise time recording.
      EndTime = millis(); 
    }
    
    /* THIS IS HOW STUFF IS DISPLAYED ON THE OLED, CHANGE (X,Y) IF YOUR OLED IS DIFFERENT OR WANT DIFFERENT TEXT*/
    
    // This first block of code inserts the first line ">/TinyTelemetry" if you need this extra line comment or change it to whatever variable/text suits your case. We like the look of it tho ;).
    display.setTextSize(1); //Text size of 1, it changes in INTEGER intervals.
    display.setCursor(0,0); // Pixel coordinates (x,y) where the text is written out.
    display.print(">/TinyTelemetry"); // Text to be written out.
    
    //This block prints the difference in betweeen the end time stored and the start time stored variables.
    display.setTextSize(1);  
    display.setCursor(20,20); 
    display.print("Tiempo de vuelo:");
    display.setCursor(20,30);
    display.print((StartTime - EndTime)/1000);
    display.setCursor(65,30);
    display.print("s");

    //This block prints the max historical altitude, updated every 50 ms, compared trough the IF inequality of variable "altmax".
    display.setCursor(20,45);
    display.print("Historico ALT-MAX:");
    display.setCursor(20,55);
    display.print(altmax); // Print variable "alt max" which is the stored maximum value for A. Note, if you dont want decimals displayed change to: "display.print(altmax,0);"
    display.setCursor(65,55);  
    display.print("m");
    
    display.display(); // Take a wild guess
    delay(50); //How often the whole loop runs, which also is, the refresh rate (ms). (i.e) 1000 m/s = 1 FPS (For dummies like me IK that isn't how this works).
}
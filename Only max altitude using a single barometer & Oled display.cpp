//WORKING CODE!!!!!!!!!!!!!!

/* Este codigo usa una pantalla OLED de 0.96", de 64 x 128 px y una sensor barometrico BMP280
  este codigo es funcional si su unico proposito deseado es tener la altura maxima despues de un vuelo
  *IMPORTANTE: Para que mi sensor BMP280 funcionara fue necesario cambiar 
  la direccion IC2 de el sensor barometrico que utilice en -la libreria- BMP280.h, puede descargar
  la liberia YA MODIFICADA en: www.github.com/fgutep/TinyTelemetry/tree/main/Modified%20libraries/Adafruit_BMP280_Library
  Este codigo se construyo parcialmente en base de un esquema existente. Creditos: www.surtrtech.com
  */

#include <Adafruit_GFX.h>      //Libraries for the OLED and BMP280
#include <Adafruit_SSD1306.h> //Library for OLED adafruit.
#include <Adafruit_BMP280.h> //Library for BMP280 baro (see note above).
#include <Adafruit_MPU6050.h> //Libraries for MPU6050.
#include <Adafruit_Sensor.h> //Common IC2 and adafruit sensor libraries.
#include <Wire.h>

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
  display.setTextColor(WHITE);
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
//Variables ifs (Used for saving maximum altitude achieved and time (when applicable)):
   float altmax = 0;

void loop() {

    display.clearDisplay();
    float P = bmp.readPressure()/100;         //Read Pressure in Pa and conversion to hPa
    float A = bmp.readAltitude(1019.66);      //Calculating the Altitude, the "1019.66" is the pressure in (hPa) at sea level at day in your region //If you don't know it just modify it until you get the altitude of your place
    
    // If sequence for saving the max historical altitude, it basically keeps comparing a saved value "altmax" (initially = 0) to the just previously one, if grater, it stores it as "altmax"/
    if (A > altmax){
      altmax = A;
      }

    
    // This first block of code inserts the first line ">/TinyTelemetry" if you need this extra line comment or change it to whatever variable/text suits your case. We like the look of it tho ;).
    display.setTextSize(1); //Text size of 1, it changes in INTEGER intervals.
    display.setCursor(0,0); // Pixel coordinates (x,y) where the text is written out.
    display.print(">/TinyTelemetry"); // Text to be written out.
    
    //This block prints the current altitude, read every 50 ms (delay at the end of this loop).
    display.setTextSize(1);  //Oled display, just playing with text size and cursor to get the display you want
    display.setCursor(20,20); 
    display.print("Altura actual:");
    display.setCursor(20,30);
    display.print(A); // Print variable "A" which is  = bmp.readAltitude(1019.66). Declared above. Note, if you dont want decimals displayed change to: "display.print(A,0);"
    display.setCursor(65,55);  
    display.setCursor(65,30);
    display.print("m");

    //This block prints the max historical altitude, updated every 50 ms, compared trough the IF inequality of variable "altmax".
    display.setCursor(20,45);
    display.print("Historico ALT-MAX:");
    display.setCursor(20,55);
    display.print(altmax); // Print variable "alt max" which is the stored maximum value for A. Note, if you dont want decimals displayed change to: "display.print(altmax,0);"
    display.setCursor(65,55);  
    display.print("m");
    
    display.display(); // Take a wild guess
    delay(50);
}
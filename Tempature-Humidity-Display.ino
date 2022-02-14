#include "DHT.h"
#define DHTPIN 4     // what digital pin the DHT22 is conected to
#define DHTTYPE DHT22   // there are multiple kinds of DHT sensors
DHT dht(DHTPIN, DHTTYPE);

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

//Declare LCD object for SPI
//Adafruit_PCD8544(CLK,DIN,D/C,CE,RST);
Adafruit_PCD8544 display = Adafruit_PCD8544(14, 13, 5, 15, 0); /*D5, D7, D1, D8, D3 */
int contrastValue = 40; /* Default Contrast Value */

void setup()
{
  Serial.begin(9600);
  Serial.setTimeout(2000);

  // Wait for serial to initialize.
  while(!Serial) { }
  
  dht.begin();

  
  // Initialize the Display
  display.begin();

  // Change the contrast using the following API
  display.setContrast(contrastValue);

  // Clear the buffer
  display.clearDisplay();
  display.display();
  delay(100);
  
  // Now let us display some text
  display.setTextColor(WHITE, BLACK);
  display.setTextSize(1);
  display.setCursor(15,1);
  display.println("|ESP8266|");
  display.setCursor(15,13);
  display.println("|NodeMCU|");
  //display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(22,25);
  display.println("|Nokia|");
  display.setCursor(25,37);
  display.println("|5110|");
  display.display();
  //delay(2000);
  
}

int timeSinceLastRead = 0;
void loop() {

  // Report every 2 seconds.
  if(timeSinceLastRead > 2000) {
    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    float h = dht.readHumidity();
    // Read temperature as Celsius (the default)
    float t = dht.readTemperature();
    // Read temperature as Fahrenheit (isFahrenheit = true)
    float f = dht.readTemperature(true);

    // Check if any reads failed and exit early (to try again).
    if (isnan(h) || isnan(t) || isnan(f)) {
      Serial.println("Failed to read from DHT sensor!");
      timeSinceLastRead = 0;
      return;
    }

    // Compute heat index in Fahrenheit (the default)
    float hif = dht.computeHeatIndex(f, h);
    // Compute heat index in Celsius (isFahreheit = false)
    float hic = dht.computeHeatIndex(t, h, false);

    Serial.print("Humidity: ");
    Serial.print(h);
    Serial.print(" %\t");
    Serial.print("Temperature: ");
    Serial.print(t);
    Serial.print(" *C ");
    Serial.print(f);
    Serial.print(" *F\t");
    Serial.print("Heat index: ");
    Serial.print(hic);
    Serial.print(" *C ");
    Serial.print(hif);
    Serial.println(" *F");

    display.clearDisplay();
    display.setTextColor(WHITE, BLACK);
    display.setTextSize(1);
    display.setCursor(5,1);
    display.println("|Temperature|");
    display.setCursor(10,25);
    display.println("|Humidity|");
    //display.setTextSize(1);
    display.setTextColor(BLACK);
    display.setCursor(25,13);
    display.println(t);
    display.setCursor(25,37);
    display.println(h);
    display.display();

    timeSinceLastRead = 0;
  }
  delay(100);
  timeSinceLastRead += 100;
}

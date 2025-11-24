/*******************************************************
   SMART POLLEN & AIR QUALITY MONITORING SYSTEM
   ESP32 + BME280 + MQ135 + DHT11 + LCD + BLYNK IoT
   ~ A Project Of Mrityun Jay 
********************************************************/

#define BLYNK_TEMPLATE_ID "TMPLwToQUqRw"
#define BLYNK_TEMPLATE_NAME "Air Quality Monitoring"
#define BLYNK_AUTH_TOKEN "C8Y7T0Fr54QF8pdfQ5dZsdfhhSdiQBFLj8mYe"

#define BLYNK_PRINT Serial

// -------------------- LIBRARIES -----------------------
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

// -------------------- LCD SETTINGS --------------------
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Custom degree symbol for LCD
byte degree_symbol[8] =
{
  0b00111,
  0b00101,
  0b00111,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000
};

// -------------------- BLYNK AUTH ---------------------
char auth[] = BLYNK_AUTH_TOKEN;

char ssid[] = "WiFi Username";    // change this
char pass[] = "WiFi Password";    // change this

BlynkTimer timer;

// -------------------- SENSOR PINS --------------------
int gasPin = 32;     // MQ135 analog pin
#define DHTPIN 2     // DHT11 data pin
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

// -------------------- VARIABLES -----------------------
int GAS_THRESHOLD = 400;  // realistic MQ135 threshold

/*******************************************************
   FUNCTION: Send Data to Blynk
********************************************************/
void sendToBlynk()
{
  float humidity = dht.readHumidity();
  float temp = dht.readTemperature();

  if (isnan(humidity) || isnan(temp)) {
    Serial.println("DHT sensor read failed");
    return;
  }

  int gasValue = analogRead(gasPin);

  // Push to Blynk
  Blynk.virtualWrite(V0, temp);
  Blynk.virtualWrite(V1, humidity);
  Blynk.virtualWrite(V2, gasValue);

  // Debug output
  Serial.println("----------- BLYNK DATA -----------");
  Serial.print("Temp: "); Serial.print(temp); Serial.println(" °C");
  Serial.print("Humidity: "); Serial.print(humidity); Serial.println(" %");
  Serial.print("Gas Value: "); Serial.println(gasValue);

  // Trigger pollution alert
  if (gasValue > GAS_THRESHOLD) {
    Blynk.logEvent("pollution_alert", "Air Quality Dropped!");
  }
}

/*******************************************************
   FUNCTION: Update LCD Display
********************************************************/
void updateLCD()
{
  float humidity = dht.readHumidity();
  float temp = dht.readTemperature();
  int gasValue = analogRead(gasPin);

  lcd.clear();

  // Line 1 - Temperature
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(temp);
  lcd.write(1);    // degree symbol
  lcd.print("C");

  // Line 2 - Humidity & Gas
  lcd.setCursor(0, 1);
  lcd.print("Hum: ");
  lcd.print(humidity);
  lcd.print("%");

  delay(2000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Gas: ");
  lcd.print(gasValue);

  lcd.setCursor(0, 1);
  if (gasValue > GAS_THRESHOLD) {
    lcd.print("Bad Air");
  } else {
    lcd.print("Fresh Air");
  }

  delay(2000);
}

/*******************************************************
                      SETUP
********************************************************/
void setup()
{
  Serial.begin(115200);

  // LCD initialization
  lcd.begin();
  lcd.backlight();
  lcd.createChar(1, degree_symbol);

  lcd.setCursor(1, 0);
  lcd.print("Air Quality");
  lcd.setCursor(1, 1);
  lcd.print("Monitoring...");
  delay(2000);
  lcd.clear();

  // Start WiFi + Blynk
  Blynk.begin(auth, ssid, pass);

  // Start DHT sensor
  dht.begin();

  // Timers (non-blocking!)
  timer.setInterval(30000L, sendToBlynk);  // send data every 30s
  timer.setInterval(5000L, updateLCD);     // update LCD every 5s
}

/*******************************************************
                      LOOP
********************************************************/
void loop()
{
  Blynk.run();
  timer.run();
}


// This is the end of the code 

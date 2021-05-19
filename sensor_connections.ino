#include <ESP8266WiFi.h>                                                    // Importing ESP8266 Library
#include <FirebaseArduino.h>                                                // Importing Firebase library
#include <DHT.h>                                                            // Importing DHT11 sensor library

#define FIREBASE_HOST "waterquality-67c37-default-rtdb.firebaseio.com"      // Firebase Database ID
#define FIREBASE_AUTH  "V4sTHsTufTnSEC7KJHGlLgMy2aWTQmL58PyhQ0Dj"           // Firebase Database credentials key

#define WIFI_SSID "Diya"                                                    //Wi-Fi SSID
#define WIFI_PASSWORD "Diya#1234"                                           //Wi-Fi password
 
#define DHTPIN D4                                                           //Digital pin of ESP connection is done to
#define DHTTYPE DHT11                                                       //Selecting DHT type as DHT11
DHT dht(DHTPIN, DHTTYPE);                                                     


void setup() 
{
  Serial.begin(115200);
  delay(1000);                
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);                                     //Establishing connectionw ith Wi-FI
  Serial.print("Connecting to ");                                           //Printing message if connection successful
  Serial.print(WIFI_SSID);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("Connected to ");
  Serial.println(WIFI_SSID);
  Serial.print("IP Address is : ");
  Serial.println(WiFi.localIP());                                            //Displaying IP address 
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);                              //beginning Firebase connection
  dht.begin();                                                               //Reading data from DHT11 sesnor
}

void loop() { 
  float h = dht.readHumidity();                                              // Reading temperature and humidity data
  float t = dht.readTemperature();                                           // Read temperature in Celsius
    
  if (isnan(h) || isnan(t)) {                                                //Display error messages if reading not possible.
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  
  Serial.print("Humidity: ");  Serial.print(h);
  String fireHumid = String(h) + String("%");                                //convert integer humidity to float humidity 
  Serial.print("%  Temperature: ");  Serial.print(t);  Serial.println("°C ");
  String fireTemp = String(t) + String("°C");                                //convert integer temperature to float temperature
  delay(4000);
  
  Firebase.pushFloat("/DHT11/Humidity", h);                                  //Push humidity data to firebase database
  //Serial.println("Sent to database");
  Firebase.pushFloat("/DHT11/Temperature", t);                               //Push temperature data to firebase database
   
}

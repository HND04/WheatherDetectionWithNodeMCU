#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DHT.h>


const char* ssid = "humans";
const char* password = "abcd1234";

ESP8266WebServer server(80);

#define DHTPIN D4 
#define DHTTYPE DHT11 

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  delay(10);
  

  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  
  dht.begin();


  server.on("/", handleRoot);
  
  
  server.begin();
  Serial.println("HTTP server started");
}

void handleRoot() {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  if (isnan(humidity) || isnan(temperature)) {
    server.send(500, "text/plain", "Failure");
    return;
  }

  String fe = "<!DOCTYPE html><html><head><title>Weather Station</title></head><body>";
  fe += "<h1>Weather Station</h1>";
  fe += "<p>Temperature: " + String(temperature) + " &#8451;</p>";
  fe += "<p>Humidity: " + String(humidity) + " %</p>";
  fe += "</body></html>";

  server.send(200, "text/html", fe);
}

void loop() {
  server.handleClient();
}

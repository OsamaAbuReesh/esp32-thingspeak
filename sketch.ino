#include <DHT.h>
#include <WiFi.h>
#include <HTTPClient.h>

#define DHTPIN 15
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "Wokwi-GUEST";
const char* password = "";
String apiKey = "YOUR_WRITE_API_KEY";
String studentId = "12115003";

void setup() {
  Serial.begin(115200);
  dht.begin();

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("Connected");
}

void loop() {
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();

  if (isnan(temp) || isnan(hum)) {
    Serial.println("Sensor error");
    delay(2000);
    return;
  }

  String url = "http://api.thingspeak.com/update?api_key=" + apiKey;
  url = url + "&field1=" + String(temp);
  url = url + "&field2=" + String(hum);
  url = url + "&status=" + studentId;

  Serial.println(url);

  Serial.println("ID: " + studentId);
  Serial.println("Temp: " + String(temp));
  Serial.println("Hum: " + String(hum));

  HTTPClient http;
  http.begin(url);
  int code = http.GET();
  http.end();

  Serial.println("HTTP code: " + String(code));

  delay(20000);
}

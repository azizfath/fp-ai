#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

char ssid[] = "Kost Santri 3";
char pass[] = "satusampailima";

String api = "https://fp-ai.tukutahu.tk/insertData";
HTTPClient http;
WiFiClientSecure client;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  WiFi.begin(ssid,pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  client.setInsecure();
  Serial.println("Connected to the WiFi network");
  Serial.println(WiFi.localIP());
}

void loop() {
  if(WiFi.status() == WL_CONNECTED){

        
        http.begin(client,api.c_str());
        http.addHeader("Content-Type", "application/json");
//        String temperature = String(analogRead(temperaturePin));
//        String humidity = String(analogRead(humidityPin));
        String reqBody = "{\"deviceId\":\"esp8266\",\"tempereture\":69,\"humidity\":69}";
        Serial.println("-----------------------------");
        Serial.println(reqBody);
        int httpResponseCode = http.POST(reqBody);

        if (httpResponseCode == 200) {
            Serial.print("HTTP response code: ");
            Serial.println(httpResponseCode);
        }
        else {
            Serial.print("Error post data code: ");
            Serial.println(httpResponseCode);
            Serial.println(http.getString());
        }
        http.end();
        delay(15000);

    }

}

//#include <SPI.h>
//#include <Wire.h>
//#include <Adafruit_GFX.h>
//#include <Adafruit_SSD1306.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <HTTPClient.h>
            //W, H, MOSI, CLK, DC, RS, CS  for OLED SPI connection
//Adafruit_SSD1306 display(128, 64, 23, 18, 16, 17, 5);
char ssid[] = "redmi3pro";
char password[] = "sijiwolu";
unsigned long timerDelay = 1000, LastTime = 0;
unsigned long currentMillis = 0, previousMillis = 0, interval = 250;
String datasetAPI ="https://fp-ai.tukutahu.tk/insertData";


HTTPClient http;
float temps()                     //temperature sensor calculation
{
  int Vo = analogRead(33);
  float R1 = 8000, logR2, R2, T, Tc, Tf, c1 = 1.5858696695070901e-03, c2 = 1.3000527215930986e-4, c3 = 7.305028121166598e-7;
  R2 = R1 * (4095.0 / (float)Vo - 1.0);  
  logR2 = log(R2);
  T = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2));
  Tc = T - 273.15;
  return Tc;
}

//void oledScreen()                   //oled function
//{
//  display.clearDisplay();
//  display.setTextSize(3);
//  display.setTextColor(SSD1306_WHITE);
//  display.setCursor(0,0);
//  display.print(temps());
//  display.print(char(247));
//  display.println("C");
//  display.display();
//}

void setup() {
  Serial.begin(921600);
//  if(!display.begin(SSD1306_SWITCHCAPVCC)) {
//    Serial.println(F("SSD1306 allocation failed"));
//    for(;;);
//  }
//  display.setRotation(2);
//  display.clearDisplay();
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
 
  Serial.println("Connected to the WiFi network");
  Serial.println(WiFi.localIP());
}

void loop() {
  
  currentMillis = millis();
//  if (currentMillis - previousMillis >= interval){
//    //Serial.println(temps());
//    oledScreen();                   //calling oled function
//    previousMillis = currentMillis;
//  }
  if ((millis() - LastTime > timerDelay)) {
    if(WiFi.status() == WL_CONNECTED){
      http.begin(datasetAPI);
      http.addHeader("Content-Type", "application/json");
      String temperature = String(temps());
      String reqBody = "{\"deviceId\":\"esp32-faez\",\"temperature\":"+ temperature + "}";
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
      LastTime = millis();
    }else {
      WiFi.reconnect();
    }
  }
}

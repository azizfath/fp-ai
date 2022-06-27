#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

#define BLYNK_AUTH_TOKEN "uw4iQYRC6ZROnqbkCKt4HqGYmwnMp37F"
#define BLYNK_PRINT Serial
// W, H, MOSI, CLK, DC, RS, CS  for OLED SPI connection
Adafruit_SSD1306 display(128, 64, 23, 18, 16, 17, 5);
char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Kost Santri 3";
char pass[] = "satusampailima";
unsigned long timerDelay = 1000, LastTime = 0;
unsigned long currentMillis = 0, previousMillis = 0, interval = 250;
BlynkTimer timer;

// BAHAYA BAWAH
double probBB, meanBB = 10.87, stdevBB = 3.149439842;

// PERINGATAN BAWAH
double probPB, meanPB = 18.97, stdevPB = 1.428389229;

// NORMAL
double probNO, meanNO = 26.85, stdevNO = 3.223024688;

// PERINGATAN ATAS
double probPA, meanPA = 33.69, stdevPA = 0.96095498;

// BAHAYA ATAS
double probBA, meanBA = 38.09, stdevBA = 2.501687588;

float temp, pi = 3.14;
String kondisi;

float temps() // temperature sensor calculation
{
    int Vo = analogRead(33);
    float R1 = 8000, logR2, R2, T, Tc, Tf, c1 = 1.5858696695070901e-03, c2 = 1.3000527215930986e-4, c3 = 7.305028121166598e-7;
    R2 = R1 * (4095.0 / (float)Vo - 1.0);
    logR2 = log(R2);
    T = (1.0 / (c1 + c2 * logR2 + c3 * logR2 * logR2 * logR2));
    Tc = T - 273.15;
    return Tc;
}

void program()
{
    temp = temps();

    probBB = (1 / (sqrt(2 * pi * stdevBB))) * exp(-(pow(temp - meanBB, 2) / (pow(2 * stdevBB, 2))));
    probPB = (1 / (sqrt(2 * pi * stdevPB))) * exp(-(pow(temp - meanPB, 2) / (pow(2 * stdevPB, 2))));
    probNO = (1 / (sqrt(2 * pi * stdevNO))) * exp(-(pow(temp - meanNO, 2) / (pow(2 * stdevNO, 2))));
    probPA = (1 / (sqrt(2 * pi * stdevPA))) * exp(-(pow(temp - meanPA, 2) / (pow(2 * stdevPA, 2))));
    probBA = (1 / (sqrt(2 * pi * stdevBA))) * exp(-(pow(temp - meanBA, 2) / (pow(2 * stdevBA, 2))));

    Blynk.virtualWrite(V0, temp);
    Blynk.virtualWrite(V6, probBB);
    Blynk.virtualWrite(V7, probPB);
    Blynk.virtualWrite(V8, probNO);
    Blynk.virtualWrite(V9, probPA);
    Blynk.virtualWrite(V10, probBA);

    if (probBB > probPB && probBB > probNO && probBB > probPA && probBB > probBA)
    {
        kondisi = "Bahaya Bawah";
    }
    if (probPB > probBB && probPB > probNO && probPB > probPA && probPB > probBA)
    {
        kondisi = "Peringatan Bawah";
    }
    if (probNO > probBB && probNO > probPB && probNO > probPA && probNO > probBA)
    {
        kondisi = "Normal";
    }
    if (probPA > probBB && probPA > probPB && probPA > probNO && probPA > probBA)
    {
        kondisi = "Peringatan Atas";
    }
    if (probBA > probBB && probBA > probPB && probBA > probNO && probBA > probPA)
    {
        kondisi = "Bahaya Atas";
    }

    Blynk.virtualWrite(V1, kondisi);
}

void oledScreen() // oled function
{
    display.clearDisplay();
    display.setTextSize(3);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.print(temps());
    display.print(char(247));
    display.println("C");
    display.setTextSize(2);

    int index = kondisi.indexOf(' ');
    if (index == -1)
    {
        display.print(kondisi);
    }
    else
    {
        display.println(kondisi.substring(0, index));
        display.println(kondisi.substring(index + 1));
    }

    // display.print(kondisi);
    display.display();
}

void setup()
{
    Serial.begin(921600);
    if (!display.begin(SSD1306_SWITCHCAPVCC))
    {
        Serial.println(F("SSD1306 allocation failed"));
        for (;;)
            ;
    }
    display.setRotation(2);
    display.clearDisplay();
    WiFi.begin(ssid, pass);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.println("Connecting to WiFi..");
    }

    Serial.println("Connected to the WiFi network");
    Serial.println(WiFi.localIP());
    Blynk.begin(auth, ssid, pass, "sg1.azizfath.my.id", 8080);
    timer.setInterval(1000L, program);
}

void loop()
{

    currentMillis = millis();
    if (currentMillis - previousMillis >= interval)
    {
        oledScreen(); // calling oled function
        previousMillis = currentMillis;
    }
    Blynk.run();
    timer.run();
}

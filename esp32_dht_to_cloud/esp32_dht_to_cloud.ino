#include <Adafruit_Sensor.h>
#include <DHT.h>

#define DHTPIN  4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  delay(1000);
  dht.begin();
  Serial.println("DHT22 test start...");
}

void loop() {
  float t = dht.readTemperature();   // °C
  float h = dht.readHumidity();      // %RH

  // DHT22 บางทีอ่านพลาด คืนค่า NaN -> เช็คก่อนพิมพ์
 if (isnan(t) || isnan(h)) {
    Serial.println("อ่านค่าไม่ได้! เช็คสาย DATA / ไฟ 3.3V / GND");
    delay(2000);
    return;
  }

  Serial.print("Temp: ");
  Serial.print(t);
  Serial.print(" C   Humidity: ");
  Serial.print(h);
  Serial.println(" %");

  delay(2000);     // DHT22 อ่านได้เร็วสุด ~ทุก 2 วินาที
}
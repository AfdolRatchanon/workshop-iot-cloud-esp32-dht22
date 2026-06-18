#include <WiFi.h>
#include <FirebaseESP32.h> // กลับมาใช้ตัวเดิมในเครื่องคุณ
#include <DHT.h>

// ปรับโครงสร้างข้อมูลเชื่อมต่อให้ถูกต้อง
#define FIREBASE_HOST "iot-to-firebase-a1c09-default-rtdb.firebaseio.com/" 
#define FIREBASE_AUTH "AIzaSyD_enLslvmWUS6o5XnTVWMODsOVr1Ezrn0"

DHT dht(4, DHT11);

// ประกาศตัวแปรโครงสร้างคอนฟิก
FirebaseData fbdo;
FirebaseConfig config;
FirebaseAuth auth;

void setup() {
  Serial.begin(115200);
  dht.begin();

  // ใส่ชื่อและรหัสผ่าน Wi-Fi บ้านของคุณ
  WiFi.begin("IoTCloud_2.4Ghz", "iotcloud");
  while (WiFi.status() != WL_CONNECTED) { delay(500); }

  // กำหนดค่าคอนฟิกให้กับไลบรารีที่คุณมีอยู่
  config.host = FIREBASE_HOST;
  config.signer.tokens.legacy_token = FIREBASE_AUTH;

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void loop() {
  float t = dht.readTemperature();
  float h = dht.readHumidity();

  if (isnan(t) || isnan(h)) {
    Serial.println("อ่านค่าไม่ได้!");
    delay(2000);
    return;
  }

  Serial.print("Temp: "); Serial.print(t); Serial.print(" C   Humidity: "); Serial.print(h); Serial.println(" %");

  // ส่งข้อมูลขึ้น Realtime Database
  Firebase.setFloat(fbdo, "/temperature", t);
  Firebase.setFloat(fbdo, "/humidity", h);

  delay(2000); 
}
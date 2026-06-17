#include <WiFi.h>
#include <FirebaseESP32.h>
#include <DHT.h>

// ปรับลิงก์ฐานข้อมูลให้ถูกต้องเรียบร้อยแล้วตามข้อมูลโปรเจกต์ของคุณ
#define FIREBASE_HOST "iot-to-firebase-a1c09-default-rtdb.firebaseio.com" 
#define FIREBASE_AUTH "AIzaSyD_enLslvmWUS6o5XnTVWMODsOVr1Ezrn0"

DHT dht(4, DHT22);

FirebaseData fbdo;
FirebaseConfig config;
FirebaseAuth auth;

void setup() {
  Serial.begin(115200);
  delay(1000);
  dht.begin();
  Serial.println("\n--- เริ่มต้นระบบทดสอบ ESP32 + Firebase ---");

  // 1. ตรวจเช็คสถานะการเชื่อมต่อ Wi-Fi
  Serial.print("กำลังเชื่อมต่อ Wi-Fi");
  WiFi.begin("Teacher2312", "12345678"); // <-- อย่าลืมแก้ชื่อ/รหัส Wi-Fi ตรงนี้
  
  while (WiFi.status() != WL_CONNECTED) { 
    delay(500); 
    Serial.print("."); // พิมพ์จุดไข่ปลาระหว่างรอต่อเน็ต
  }
  
  Serial.println("\n[สำเร็จ] Wi-Fi เชื่อมต่อเรียบร้อยแล้ว!");
  Serial.print("IP Address ของบอร์ด: ");
  Serial.println(WiFi.localIP());

  // 2. กำหนดค่าคอนฟิกให้กับ Firebase
  config.host = FIREBASE_HOST;
  config.signer.tokens.legacy_token = FIREBASE_AUTH;

  Serial.println("กำลังเชื่อมต่อ Firebase...");
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
  Serial.println("[สำเร็จ] ตั้งค่า Firebase พร้อมส่งข้อมูล");
}

void loop() {
  float t = dht.readTemperature();
  float h = dht.readHumidity();

  if (isnan(t) || isnan(h)) {
    Serial.println("[แจ้งเตือน] อ่านค่าจากเซนเซอร์ DHT22 ไม่ได้! เช็คสายสัญญาณ");
    delay(2000);
    return;
  }

  Serial.print("ข้อมูลปัจจุบัน -> Temp: "); Serial.print(t); 
  Serial.print(" C | Humidity: "); Serial.print(h); Serial.println(" %");

  // 3. ตรวจเช็คสถานะการส่งข้อมูลเข้า Realtime Database ทันทีที่ยิงคำสั่ง
  if (Firebase.setFloat(fbdo, "/temperature", t)) {
    Serial.println(" -> [Firebase] ส่งค่า Temperature สำเร็จ ✅");
  } else {
    Serial.print(" -> [Firebase] ส่งค่า Temperature ล้มเหลว ❌ สาเหตุ: ");
    Serial.println(fbdo.errorReason());
  }

  if (Firebase.setFloat(fbdo, "/humidity", h)) {
    Serial.println(" -> [Firebase] ส่งค่า Humidity สำเร็จ ✅");
  } else {
    Serial.print(" -> [Firebase] ส่งค่า Humidity ล้มเหลว ❌ สาเหตุ: ");
    Serial.println(fbdo.errorReason());
  }

  Serial.println("----------------------------------------");
  delay(5000); // ขยับเวลาเป็น 5 วินาทีเพื่อให้ Serial Monitor อ่านง่ายขึ้น ไม่เด้งไวเกินไป
}
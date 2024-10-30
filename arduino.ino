#include "WiFiS3.h"            // 用於 ESP32-S3 的 Wi-Fi 庫
#include "WiFiSSLClient.h"      // 用於 HTTPS 連線
#include <ArduinoJson.h>        // 用於 JSON 解析

// Wi-Fi 連線資訊
const char* ssid = "CS_Class";  
const char* password = "26430686";  

// 伺服器資訊
const char* server = "warningsign.pp.ua";
const int httpsPort = 443;  // HTTPS 埠

// HTTPS 客戶端
WiFiSSLClient client;

// 計時器變數
unsigned long previousMillis = 0;  
const long interval = 10000;  // 每 10 秒爬取一次資料

void setup() {
  Serial.begin(115200);  // 初始化序列埠
  delay(10);

  // 連接 Wi-Fi
  connectToWiFi();
}

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    if (client.connected()) {
      getDataFromServer();  // 每 10 秒執行一次爬取任務
      sendPostRequest();    // 可選：發送 POST 請求
    } else {
      Serial.println("客戶端未連接，嘗試重新連接伺服器...");
      reconnectToServer();
    }
  }
}

// 函數：連接到 Wi-Fi
void connectToWiFi() {
  Serial.println("正在連接 Wi-Fi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi 已連接");
}

// 函數：重新連接到伺服器
void reconnectToServer() {
  client.stop();  // 停止當前連接
  if (client.connect(server, httpsPort)) {
    Serial.println("成功重新連接到伺服器");
  } else {
    Serial.println("伺服器連接失敗，請檢查網路狀態");
    Serial.print("Wi-Fi 狀態: ");
    Serial.println(WiFi.status()); // 顯示 Wi-Fi 狀態
  }
}

// 函數：爬取伺服器上的 JSON 資料
void getDataFromServer() {
  if (client.connect(server, httpsPort)) {
    client.println("GET /output/ HTTP/1.1");
    client.println("Host: warningsign.pp.ua");
    client.println("Connection: close");
    client.println();

    // 等待並讀取伺服器回應
    String response;
    while (client.connected() || client.available()) {
      response = client.readStringUntil('\n');
      if (response == "\r") {
        break;  // 略過空白行
      }
    }

    // 讀取狀態行
    String statusLine = response;
    Serial.print("GET HTTP 狀態: ");
    Serial.println(statusLine);

    // 讀取 JSON 回應內容
    String payload = client.readString();
    Serial.println("爬取的 JSON 資料:");
    Serial.println(payload);

    // 增大 JSON 緩衝區大小為 256
    StaticJsonDocument<256> doc;

    // 解析 JSON 資料
    DeserializationError error = deserializeJson(doc, payload);
    if (error) {
      Serial.print("解析 JSON 失敗: ");
      Serial.println(error.c_str());
      return;
    }

    // 成功解析，打印狀態欄位
    const char* status = doc["status"];
    Serial.print("狀態: ");
    Serial.println(status);
  } else {
    Serial.println("GET 連線失敗");
  }
}

// 函數：發送 POST 請求
void sendPostRequest() {
  if (client.connect(server, httpsPort)) {
    String jsonData = "{\"battery\":0,\"battery_tem\":25,\"waringsign_status\":2,\"UltraSound\":1}";

    client.println("POST /arduino_info/ HTTP/1.1");
    client.println("Host: warningsign.pp.ua");
    client.println("Content-Type: application/json");
    client.print("Content-Length: ");
    client.println(jsonData.length());
    client.println();
    client.println(jsonData);

    // 等待並讀取伺服器回應
    String response;
    while (client.connected() || client.available()) {
      response = client.readStringUntil('\n');
      if (response == "\r") {
        break;  // 略過空白行
      }
    }

    // 讀取狀態行
    String statusLine = response;
    Serial.print("POST HTTP 狀態: ");
    Serial.println(statusLine);

    // 讀取回應內容
    String postResponse = client.readString();
    Serial.println("POST 回應內容:");
    Serial.println(postResponse);
  } else {
    Serial.println("POST 連線失敗");
  }
}

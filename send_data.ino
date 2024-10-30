#include "WiFiS3.h"
#include "WiFiSSLClient.h"

const char* ssid = "CS_Class";
const char* password = "26430686";

const char* server = "warningsign.pp.ua";
const int httpsPort = 443;

WiFiSSLClient client;

void setup() {
  Serial.begin(115200);
  delay(10);

  Serial.println("正在連接 Wi-Fi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi 已連接");

  if (!client.connect(server, httpsPort)) {
    Serial.println("連線失敗");
    return;
  }
  Serial.println("已連線到伺服器");

  String jsonData = "{\"battery\":0,\"battery_tem\":0,\"waringsign_status\":2,\"UltraSound\":1}";

  client.println("POST /arduino_info/ HTTP/1.1");
  client.println("Host: warningsign.pp.ua");
  client.println("Content-Type: application/json");
  client.print("Content-Length: ");
  client.println(jsonData.length());
  client.println();
  client.println(jsonData);

  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      break;
    }
  }

  String response = client.readString();
  Serial.println("伺服器回應內容:");
  Serial.println(response);
}

void loop() {
  
}

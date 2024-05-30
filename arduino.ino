#include "WiFiS3.h"
#include "WiFiSSLClient.h"

#define SECRET_SSID "CS_Class"
#define SECRET_PASS "26430686" 

char ssid[] = "CS_Class";
char pass[] = "26430686";
char server[] = "warningsign.pp.ua";
int keyIndex = 0;
int status = WL_IDLE_STATUS;

char opendoor = false;

WiFiSSLClient client;

void setup() {
  Serial.begin(9600);
  while (!Serial) {
  }

  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    while (true);
  }
  
  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  }

  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
     
    // wait 10 seconds for connection:
    delay(10000);
  }
  
  printWifiStatus();
 
  Serial.println("\nStarting connection to server...");

  if (client.connect(server, 443)) {
    Serial.println("connected to server");
    client.println("GET /output/ HTTP/1.1");
    client.println("Host: warningsign.pp.ua");
    client.println("Connection: close");
    client.println();
    }
  }

void loop() {
  if (!client.connected()) {
    // 如果未连接，则重新连接
    Serial.println("Attempting to reconnect to server...");
    if (client.connect(server, 443)) {
      Serial.println("Reconnected to server");
      client.println("GET /output/ HTTP/1.1");
      client.println("Host: warningsign.pp.ua");
      client.println("Connection: close");
      client.println();
    } else {
      Serial.println("Connection failed. Retrying in 5 seconds...");
      delay(5000);
      return;
    }
  }

  // 读取服务器响应并打印内容
  read_response();

  // 延迟5秒钟
  delay(5000);
}

void read_response() {
  bool jsonStarted = false;
  bool errorFound = false;

  while (client.available()) {
    char c = client.read();
    
    if (c == '{') {
      jsonStarted = true;
    }

    if (jsonStarted) {
      Serial.print(c);

      // 检查是否包含错误信息
      if (c == ':' && errorFound) {
        // 找到了 "status": "error"，将 opendoor 设为 false
        opendoor = false;
        errorFound = false;
      } else if (c == '"') {
        errorFound = !errorFound;
      }
    }

    if (c == '}') {
      jsonStarted = false;
    }
  }  
}


void printWifiStatus() {
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

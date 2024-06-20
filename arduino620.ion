#include "WiFiS3.h"
#include "WiFiSSLClient.h"

#include <LiquidCrystal_PCF8574.h>
LiquidCrystal_PCF8574 lcd(0x27);

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
  lcd.begin(16, 2);
  lcd.setBacklight(255);
  lcd.clear();
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
  char c;
  char ss[400];
  while (client.available()) {
    //char c = client.read();
    c = client.read();
    if (c == '{') {
      jsonStarted = true;
    }

    if (jsonStarted) {
      Serial.print(c);
      strncat(ss, &c, 1);
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
    //找到這個才繼續往下找
    char* d = strstr(ss, "status");
    Serial.println("==============================================");
    Serial.println(d);
    Serial.println("==============================================");

    //if d=error;
    //  opendoor=false
    //else
    //  opendoor=true  
    //
    lcd.setBacklight(255);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(d);
    //if(d=)
    //如果找到不動作
    //char* d = strstr(dd, "error");
    //如果找到就動作
    //char* d = strstr(dd, "success");
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

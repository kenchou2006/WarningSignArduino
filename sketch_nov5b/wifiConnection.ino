void wifiConnection() {
  int w=1,f=1;

  ssid[0] = "CS_Class";
  password[0] = "26430686";
  ssid[1] = "JICTS-FGAP";
  password[1] = "26430686";
  ssid[2] = "密碼是π";
  password[2] = "3.14159265359";
  ssid[3] = "CS_DIGITAL";
  password[3] = "26430686";
  ssid[4] = "OPPO Reno8 5G";
  password[4] = "31415926535";


  do{
  // 連接 Wi-Fi
  Serial.print("正在連接  ");
  Serial.print(ssid[f]);
  Serial.print("  Wi-Fi...");
  WiFi.begin(ssid[f], password[f]);
  while (WiFi.status() != WL_CONNECTED && w<=5) {
    Serial.print(".");
    digitalWrite(WWW, HIGH); 
    delay(250);
    digitalWrite(WWW, LOW);
    delay(250);
    w++;
  }
  Serial.println();
  if (WiFi.status() != WL_CONNECTED){
    w=1;
    f++;
    if (ssid[f] == "" || f >=3){
      f=0;
    }
    }
  }while(WiFi.status() != WL_CONNECTED);
  Serial.println("\nWi-Fi 已連接");
}
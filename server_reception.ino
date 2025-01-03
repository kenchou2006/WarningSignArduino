int performRequest(int mod) {
  // 連接伺服器
  if (!client.connect(server, httpsPort)) {
    Serial.println("連線到伺服器失敗");
    return 2;
  }
  Serial.println("已連線到伺服器");

  // 發送 GET 請求
  client.println(mod == 0 ? "GET /output/ HTTP/1.1" : "GET /output/off/ HTTP/1.1");
  client.println("Host: warningsign.pp.ua");
  client.println("Connection: close");
  client.println();

  // 讀取並處理回應
  char responseBuffer[1000];
  int index = 0;
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") break;
  }
  while (client.available() && index < sizeof(responseBuffer) - 1) {
    responseBuffer[index++] = client.read();
  }
  responseBuffer[index] = '\0';

  // 調試：打印伺服器回應
  //Serial.println("伺服器回應內容:");
  //Serial.println(responseBuffer);

  // 找到 status 的值
  char* statusPtr = strstr(responseBuffer, "\"status\"");
  if (statusPtr) {
    Serial.println("找到 status 字段");
    statusPtr = strchr(statusPtr, ':') + 1;
    Serial.print("status: ");
    Serial.println(statusPtr);

    if (strstr(statusPtr, "error")) {
      Serial.println("0");
      return 0;
    } else if (strstr(statusPtr, "success")) {
      Serial.println("1");
      return 1;
    } else {
      Serial.println("未知的狀態值");
      return 2;
    }
  } else {
    Serial.println("無法找到字段");
    return 2;
  }
}

void server_transfer(int battery,int waringsign_status,int charing,int eco) {
  int q=1;
  //Arduino電量,立牌狀態(1開2關),充電狀態,省電模式(1省電2正常)
  String response;
  do{
  while (!client.connect(server, httpsPort)) {
    Serial.println("連線失敗");
    Serial.println("重新連接中...");
    delay(500);
    q++;
    if(q >= 5){
      wifiConnection();
      q=1;
    }
  }
  Serial.println("已連線到伺服器");

  String jsonData = "{\"battery\":" + String(battery) + 
                    ",\"waringsign_status\":" + String(waringsign_status) + 
                    ",\"charing\":" + String(charing) + 
                    ",\"eco\":" + String(eco) + "}";
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
  Serial.println("傳送資料:");
  Serial.println("battery:" + String(battery));
  Serial.println("waringsign_status:" + String(waringsign_status));
  Serial.println("charing:" + String(charing));
  Serial.println("eco:" + String(eco));
  response = client.readString();
  Serial.println("伺服器回應內容:");
  Serial.println(response);
  }while (response != "{\"status\": \"success\"}");
  digitalWrite(WWW, HIGH); 
  delay(500);
  digitalWrite(WWW, LOW);
  delay(500);
  digitalWrite(WWW, HIGH); 
  delay(500);
  digitalWrite(WWW, LOW);
}
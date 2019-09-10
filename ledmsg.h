const int ledPin =  2;// the number of the LED pin

// Variables will change:
          // ledState used to set the LED

// Generally, you should use "unsigned long" for variables that hold time
// The value will quickly become too large for an int to store
unsigned long previousMillis = 0;        // will store last time LED was updated

// constants won't change:

void ledmsgvoid(){
  unsigned long currentMillis = millis();


  if (str(ssid) == str(ssidAttack)){
    // Serial.println("BOBOB");
    // Serial.print("currentMillis ");
    // Serial.println(currentMillis);
    // Serial.print("previousMillis ");
    // Serial.println(previousMillis);
    // Serial.print("currentMillis - previousMillis ");
    // Serial.println(currentMillis - previousMillis);
    if (currentMillis - previousMillis >= 400) {

      // save the last time you blinked the LED
      previousMillis = currentMillis;
      // if the LED is off turn it on and vice-versa:
      if (ledState == 1) {
        digitalWrite(ledPin, 0);
        ledState = 0;
      } else {
        digitalWrite(ledPin, 1);
        ledState = 1;
      }

      // set the LED with the ledState of the variable:
    }
  }else{
    if (stationConnected){
      digitalWrite(ledPin, 0);
    }else{
      digitalWrite(ledPin, 1);
    }
  }

}

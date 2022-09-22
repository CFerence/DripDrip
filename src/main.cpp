//u8x8 Reference: https://github-wiki-see.page/m/olikraus/u8g2/wiki/u8x8reference
//HTTPS Reference: https://medium.com/@sanghviyash6/migrating-any-http-request-to-https-on-esp32-5545a6de7845
//JSON Reference: https://arduinojson.org/v6/how-to/use-arduinojson-with-httpclient/
//Get Reference: https://randomnerdtutorials.com/esp8266-nodemcu-http-get-post-arduino/#http-get-2
//Relays: https://randomnerdtutorials.com/esp8266-relay-module-ac-web-server/


#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <Arduino_JSON.h>


const char* ssid = "Mars";
const char* password = "7MinutesofTerror";

const char* ca_cert= \
"-----BEGIN CERTIFICATE-----\n" \
"MIIGizCCBXOgAwIBAgIQDP6b0z1Ifj0sgSV7iUrogDANBgkqhkiG9w0BAQsFADBZ\n" \
"MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMTMwMQYDVQQDEypS\n" \
"YXBpZFNTTCBUTFMgRFYgUlNBIE1peGVkIFNIQTI1NiAyMDIwIENBLTEwHhcNMjEx\n" \
"MTAzMDAwMDAwWhcNMjIxMTA5MjM1OTU5WjAYMRYwFAYDVQQDDA0qLmVjb3dpdHQu\n" \
"bmV0MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAzNNAatxPaWyy39nQ\n" \
"05BnsHGniqCqaJGAyejipZqtnhb1WslmWNP0Kf+ygaioPDSXmwH2bxC7rMkLHAk4\n" \
"/hGj8pBIELGLvPuKCaTkVacRdGS9t7HpbcHxCu7s82TxJmcer/QHFSe5VC4RGT5i\n" \
"wBY89663UasSssDUm90E0RYs7zIRbP/guYPiWx6q76xmkUmcOmecjpxe5CW66wk0\n" \
"ExUn1KxJJTV6WmUC3O+KRbaL3pClaq3aWy6RN1Dn/+6vQ5AA8NZpGNsjhNfwdYI+\n" \
"7dHqMWzNmaG7sv608FMXO/0TUbXsbY/L0JjYGsUawzAb6vaiV4I0E8CT4UX0CMp6\n" \
"NW4BAwIDAQABo4IDjjCCA4owHwYDVR0jBBgwFoAUpI3lvnx55HAjbS4pNK0jWNz1\n" \
"MX8wHQYDVR0OBBYEFBR3UQUEwSWaH+O1x+qGN8xg7jRZMCUGA1UdEQQeMByCDSou\n" \
"ZWNvd2l0dC5uZXSCC2Vjb3dpdHQubmV0MA4GA1UdDwEB/wQEAwIFoDAdBgNVHSUE\n" \
"FjAUBggrBgEFBQcDAQYIKwYBBQUHAwIwgZsGA1UdHwSBkzCBkDBGoESgQoZAaHR0\n" \
"cDovL2NybDMuZGlnaWNlcnQuY29tL1JhcGlkU1NMVExTRFZSU0FNaXhlZFNIQTI1\n" \
"NjIwMjBDQS0xLmNybDBGoESgQoZAaHR0cDovL2NybDQuZGlnaWNlcnQuY29tL1Jh\n" \
"cGlkU1NMVExTRFZSU0FNaXhlZFNIQTI1NjIwMjBDQS0xLmNybDA+BgNVHSAENzA1\n" \
"MDMGBmeBDAECATApMCcGCCsGAQUFBwIBFhtodHRwOi8vd3d3LmRpZ2ljZXJ0LmNv\n" \
"bS9DUFMwgYUGCCsGAQUFBwEBBHkwdzAkBggrBgEFBQcwAYYYaHR0cDovL29jc3Au\n" \
"ZGlnaWNlcnQuY29tME8GCCsGAQUFBzAChkNodHRwOi8vY2FjZXJ0cy5kaWdpY2Vy\n" \
"dC5jb20vUmFwaWRTU0xUTFNEVlJTQU1peGVkU0hBMjU2MjAyMENBLTEuY3J0MAkG\n" \
"A1UdEwQCMAAwggF/BgorBgEEAdZ5AgQCBIIBbwSCAWsBaQB3ACl5vvCeOTkh8FZz\n" \
"n2Old+W+V32cYAr4+U1dJlwlXceEAAABfOOCruIAAAQDAEgwRgIhAKtR8l8x3L/d\n" \
"ilJS25fFjCPXA/aIcVBFPBqV/3VzFOq0AiEA1TZzQtGPUEk1VTQpK7cdZda/6jVq\n" \
"kiWu9XGDV6njlZoAdQBRo7D1/QF5nFZtuDd4jwykeswbJ8v3nohCmg3+1IsF5QAA\n" \
"AXzjgq7fAAAEAwBGMEQCIAJnE0F5PnerEJOAdT3NiBxmECfoy0j8alhxFES49swL\n" \
"AiAAz/nPqlX4TBXiZAUiZGm6gWS77Wjpi/eHeaCYYBLdBAB3AEHIyrHfIkZKEMah\n" \
"OglCh15OMYsbA+vrS8do8JBilgb2AAABfOOCrscAAAQDAEgwRgIhAICVOhx1ssfM\n" \
"by5Ka2eCzvB7z6A0EFF6QwC3Olpp068iAiEAoEuMu8wA0g1uGwSO5QBvUC3qunst\n" \
"MhatCO98or3oZpQwDQYJKoZIhvcNAQELBQADggEBAIECd64shTlVN1z2MtZfKmzR\n" \
"j8OhzWBJL9OU9ifzbmNTNw8E1dTPutH0rTrNlaGUqOtc0ENkTtgOA2VcKCxL5Dvs\n" \
"m0Fhr8ykU3Gr3e82+kPD+e7lgwVtjBgN1Q4vfHbaDUSJcHAU0JPFNOoohLEIxVOo\n" \
"M/afZNJT/Qy57QFYaHUISDCTq9mWWQb9aI98qGTIGglvKKKF1sWeK9xkacD1liq6\n" \
"eHiZZVV3JIrfY1TO3G/pOLi8B75+ql+JjIUqkTSpwjJC8PRQPxruU+HTnQ1Ojj7I\n" \
"BG9MlTblYGUQVStInDuT4cBuBhboValYLkKpMc960cCoaX/IcfnqIcYI7RPGCGA=\n" \
"-----END CERTIFICATE-----\n";

//EcoWitt Domain Name
//String ApiKey = "bf0ad9cb-fa4d-4a2e-a937-21328ed3a52b";
//String AppKey = "9D7E4AC76F8A60BAD363A90C089E17A4";
//String Mac1 = "40:F5:20:3B:0A:11";

//const char* serverName = "api4.ecowitt.net/api/v3/device/real_time?application_key=" + AppKey + "&api_key=" + ApiKey + "&mac=" + Mac1 +"&call_back=all";
const char* serverName = "http://api4.ecowitt.net/api/v3/device/real_time?application_key=9D7E4AC76F8A60BAD363A90C089E17A4&api_key=bf0ad9cb-fa4d-4a2e-a937-21328ed3a52b&mac=40:F5:20:3B:0A:11&call_back=all";

// THE DEFAULT TIMER IS SET TO 10 SECONDS FOR TESTING PURPOSES
// For a final application, check the API call limits per hour/minute to avoid getting blocked/banned
unsigned long lastTime = 0;
// Set timer to 30 seconds (30000)
unsigned long timerDelay = 10000;

String sensorReadings;
float sensorReadingsArr[4];
float dataReadingsArr[10];

//Relay Parameters
#define RELAY_NO true
#define NUM_RELAYS 2

//Assign each GPIO to a relay
//Needs to match NUM_RELAYS above
int relayGPIOs[NUM_RELAYS] = {5, 4}; //D1/5, D2/4, D6/12, D7/13, D5/14

//Pump = 5
//Relay 1, Plant 1 = 4

//Actual Soil Moistures
int SM1_Act = 30; //% Lemon
int SM2_Act = 30; //% Bush
int SM3_Act = 30; //% Trellis
int SM4_Act = 30; //% Butterfly

//Soil Moisture Thresholds
int SM1_Min = 27; //%
int SM2_Min = 27; //%
int SM3_Min = 27; //%
int SM4_Min = 27; //%


void setup() {
    Serial.begin(115200);

      // Set all relays to off when the program starts - if set to Normally Open (NO), the relay is off when you set the relay to HIGH
      for(int i=1; i<=NUM_RELAYS; i++){
        pinMode(relayGPIOs[i-1], OUTPUT);
        if(RELAY_NO){
          digitalWrite(relayGPIOs[i-1], HIGH);
        }
      else{
          digitalWrite(relayGPIOs[i-1], LOW);
      }
    }

    //Connect to WiFi network
    WiFi.begin(ssid, password);
    delay(1000);
    Serial.println("Connecting to Wifi ...");
    
    while(WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("");
    Serial.print("Connected to WiFi with IP Address: ");
    Serial.println(WiFi.localIP());
}

void loop() {
 // Send an HTTP GET request depending on timerDelay
  if ((millis() - lastTime) > timerDelay) {
    //Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){

        WiFiClient client;
        HTTPClient http;
    
        // Your IP address with path or Domain name with URL path 
        http.begin(client, serverName);
        delay(1000);
        // Send HTTP POST request
        int httpResponseCode = http.GET();
  
        String payload = "{}";
  
        if (httpResponseCode>0) {
          Serial.print("HTTP Response code: ");
          Serial.println(httpResponseCode);
          payload = http.getString();
         }
        else {
          Serial.print("Error code: ");
          Serial.println(httpResponseCode);
        }
        // Free resources
      http.end();

      sensorReadings = payload;

      JSONVar myObject = JSON.parse(sensorReadings);
  
      // JSON.typeof(jsonVar) can be used to get the type of the var
      if (JSON.typeof(myObject) == "undefined") {
        Serial.println("Parsing input failed!");
        return;
      }
    
      Serial.print("JSON object = ");
      Serial.println(myObject);
    
      /*
      // myObject.keys() can be used to get an array of all the keys in the object
      JSONVar keys = myObject.keys();
    
      for (int i = 0; i < keys.length(); i++) {
        JSONVar value = myObject[keys[i]];
        Serial.print(keys[i]);
        Serial.print(" = ");
        Serial.println(value);
        sensorReadingsArr[i] = double(value);
      }
      */

      JSONVar data = myObject["data"];
      JSONVar data_soil_ch1_soilmoisture = data["soil_ch1"]["soilmoisture"];
      //const char* data_soil_ch1_soilmoisture_time = data_soil_ch1_soilmoisture["time"]; // "1647839913"
      const char* data_soil_ch1_soilmoisture_value = data_soil_ch1_soilmoisture["value"]; // "14"
      SM1_Act = atoi(data_soil_ch1_soilmoisture_value);
      Serial.print("Lemon: ");
      Serial.println(data_soil_ch1_soilmoisture_value);


      JSONVar data_soil_ch2_soilmoisture = data["soil_ch2"]["soilmoisture"];
      //const char* data_soil_ch2_soilmoisture_time = data_soil_ch2_soilmoisture["time"]; // "1647839913"
      const char* data_soil_ch2_soilmoisture_value = data_soil_ch2_soilmoisture["value"]; // "30"
      SM2_Act = atoi(data_soil_ch2_soilmoisture_value);
      Serial.print("Bush: ");
      Serial.println(data_soil_ch2_soilmoisture_value);  
      

      JSONVar data_soil_ch3_soilmoisture = data["soil_ch3"]["soilmoisture"];
      //const char* data_soil_ch3_soilmoisture_time = data_soil_ch3_soilmoisture["time"]; // "1647839913"
      const char* data_soil_ch3_soilmoisture_value = data_soil_ch3_soilmoisture["value"]; // "10"
      SM3_Act = atoi(data_soil_ch3_soilmoisture_value);
      Serial.print("Trellis: ");
      Serial.println(data_soil_ch3_soilmoisture_value);
      
      JSONVar data_soil_ch4_soilmoisture = data["soil_ch4"]["soilmoisture"];
      //const char* data_soil_ch4_soilmoisture_time = data_soil_ch4_soilmoisture["time"]; // "1647839913"
      const char* data_soil_ch4_soilmoisture_value = data_soil_ch4_soilmoisture["value"]; // "24"
      SM4_Act = atoi(data_soil_ch4_soilmoisture_value);
      Serial.print("Butter: ");
      Serial.println(data_soil_ch4_soilmoisture_value);

    //Watering statement for plant #1
    if (SM1_Act < SM1_Min) {
      Serial.println("Lemon Thirsty");
      pinMode(relayGPIOs[1], OUTPUT);
      digitalWrite(relayGPIOs[1], LOW);
      delay(2000);
      //Open plant 1 valve
      Serial.println("Valve 1 Open");
      //Water for 5 seconds
      Serial.println("pumping....");
      pinMode(relayGPIOs[0], OUTPUT);
      digitalWrite(relayGPIOs[0], LOW);
      delay(10000);
      digitalWrite(relayGPIOs[0], HIGH);
      //Close the valve
      delay(2000);
      Serial.println("Valve 1 Closed");
      Serial.println("Plant 1 Watered");
      pinMode(relayGPIOs[1], OUTPUT);
      digitalWrite(relayGPIOs[1], HIGH);
      delay(1000);
    }
/*
    //Watering statement for plant #2
    if (SM2_Act < SM2_Min) {
      Serial.println("Bush Thirsty");
      digitalWrite(V2, HIGH);
      delay(2000);
      //Open plant 1 valve
      Serial.print("Valve 2 Open");
      //Water for 5 seconds
      Serial.print("pumping....");
      pinmode(relayGPIOs[0], OUTPUT)
      digitalWrite(relayGPIOs[0], HIGH);
      delay(10000);
      digitalWrite(relayGPIOs[0], LOW);
      delay(2000);
      //Close the valve
      Serial.println("Valve 2 Closed");
      Serial.println("Plant 2 Watered");
      digitalWrite(V2, LOW);
      delay(1000);
    }

    //Watering statement for plant #3
    if (SM3_Act < SM3_Min) {
      Serial.println("Trellis Thirsty");
      digitalWrite(V3, HIGH);
      delay(2000);
      //Open plant 1 valve
      Serial.print("Valve 3 Open");
      //Water for 5 seconds
      Serial.print("5 second H2O");
      digitalWrite(WP1, HIGH);
      delay(5000);
      digitalWrite(WP1, LOW);
      //Close the valve
      Serial.print("Valve 3 Closed");
      Serial.print("Plant 3 Watered");
      digitalWrite(V3, LOW);
      delay(2000);
    }

    //Watering statement for plant #4
    if (SM4_Act < SM4_Min) {
      Serial.print("Bush Thirsty");
      digitalWrite(V4, HIGH);
      delay(2000);
      //Open plant 1 valve
      Serial.print("Valve 4 Open");
      //Water for 5 seconds
      Serial.print("5 Second H2O");
      digitalWrite(WP1, HIGH);
      delay(5000);
      digitalWrite(WP1, LOW);
      //Close the valve
      Serial.print("Valve 4 Closed");
      Serial.print("Plant 4 Watered");
      digitalWrite(V4, LOW);
      delay(2000);
    }
    */
    
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
}

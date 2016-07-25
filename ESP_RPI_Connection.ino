//Trial code for connecting to Raspberry Pi from the ESP8266 via WiFi
//This article is gold -> https://www.openhomeautomation.net/connect-esp8266-raspberry-pi/
//Arduino IDE must has ESP8266 module and aREST api installed for this to work.

#include <ESP8266WiFi.h>
#include <aREST.h>
const char* ssid = "Our_WIFI";
const char* password = "Our_WIFI_Password";
int test = 1;
aREST rest = aREST();
WiFiServer server(8888); //Start server on port 8888

void setup() {
  WiFi.begin(ssid, password); //Begin wifi
  while (WiFi.status() != WL_CONNECTED) { 
    delay(500);
    Serial.print(".");
  }
    Serial.println("");
    Serial.println("WiFi connected");
    rest.variable("test", &test);
    rest.set_id("1");
    rest.set_name("test_module")
    server.begin(); //Start Server
    Serial.println("Server started"); 
    Serial.println(WiFi.localIP()); //Print IP
  }
 

void loop() {
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  while(!client.available()){
    delay(1);
  }
  rest.handle(client);
  test++; 
}

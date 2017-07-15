#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include "FS.h"

const char* ssid = "";
const char* password = "";

WiFiUDP Udp;
unsigned int localUdpPort = 8888;  // local port to listen on
char receivedPacket[255];  //buffer for incoming packets
char wrongAddressPacket[] = "Error, wrong device address";  // an error reply string to send back
char donePacket[] = "Finished request.";
char unrecognizedCommandPacket[] = "Error. Unrecognized Command."; 
char deviceID = '1';
char deviceName[] = "Front Door";

void setup(){
  Serial.begin(115200);
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" connected");
  Udp.begin(localUdpPort);
  Serial.printf("Now listening at IP %s, UDP port %d\n", WiFi.localIP().toString().c_str(), localUdpPort);
  SPIFFS.begin();
  //SPIFFS.format();
}

void writePacket(char s[]){
  Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
  Udp.write(s);
  Udp.endPacket();
}

void loop(){
  int packetSize = Udp.parsePacket();
  if (packetSize){
    // receive incoming UDP packets
    Serial.printf("Received %d bytes from %s, port %d\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort());
    int len = Udp.read(receivedPacket, 255);
    if (len > 0)
    {
      receivedPacket[len] = 0;
    }
    Serial.printf("UDP packet contents: %s\n", receivedPacket);

    //Check if message received was intended for this device, else return error
    if(receivedPacket[1] == deviceID){

      //Check commands.
      if(receivedPacket[2] == 'D'){
        //Write string to database
        
        File f = SPIFFS.open("/database.txt", "w");
        for(int i = 3; i < len; i++){
          f.write(receivedPacket[i]);
        }
        f.close();
        writePacket(donePacket);
        Serial.println("Log successfully edited.");
        
      }else if(receivedPacket[2] == 'R'){
        //Read database, return query to PI
        File f = SPIFFS.open("/database.txt", "r");
        String s=f.readStringUntil('\n');
        char d[255];
        strcpy(d,s.c_str());
        writePacket(d);
        f.close();
        
      }else if(receivedPacket[2] == 'Q'){
        //Read log, return query to PI
        File f = SPIFFS.open("/log.txt", "r");
        String s = f.readStringUntil('\n');
        char l[255];
        strcpy(l,s.c_str());
        writePacket(l);
        f.close();
        
      }else if(receivedPacket[2] == 'C'){
        //Clear log.
        File f = SPIFFS.open("/log.txt", "w+");
        f.write(' ');
        writePacket(donePacket);
        f.close();
      }else{
        //Command not on list. Send error.
        writePacket(unrecognizedCommandPacket);
      }
      
    }else{
      writePacket(wrongAddressPacket);
    }
  }
}


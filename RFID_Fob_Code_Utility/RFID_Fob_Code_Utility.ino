#include <avr/pgmspace.h>
#include "fobs.h"


int num_chars = 0;
char ch;
String last_str;
String new_str;
int fob_num;
int start = 0;

void setup() {
   Serial.begin(9600);
   Serial1.begin(9600);
}

void loop() {

/*  This code was used to create the fob.h file...
  Serial.println("");
  Serial.println("const char* const fob_codes[] PROGMEM = {fob_0,");  
  for (int i=1; i<256; i++)
  {
    Serial.print("                                         fob_");
    Serial.print(i);
    Serial.println(",");
  }
  while(1);
  */
  
   // read from port 1, send to port 0:
   if (Serial1.available() > 0) {
     
     ch = (char)Serial1.read();
     if (ch > 41 && ch < 71)
     {
       new_str += String(ch);
       num_chars++;
     }
     
     if (ch == 13 ) 
     {
       if (num_chars == 21)
       {
         if (new_str != last_str){
           if (new_str.charAt(1) == 'F')
           {
             Serial.print("const char fob_");            
             Serial.print(fob_num);
             Serial.print("[] PROGMEM = ");
             Serial.print("\"");
             Serial.print(new_str.substring(2,17));  
             Serial.println("\";");
             fob_num++;
           }
           else if (new_str.charAt(1) == '0')
           {
             //Serial.print("Key pressed: ");
             //Serial.println(new_str.substring(17,18));               
           }
           last_str = new_str; 

         }
       }  
       num_chars = 0;  
       new_str = "";
     }

   }
  
}

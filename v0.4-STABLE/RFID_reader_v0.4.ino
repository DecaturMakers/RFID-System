/*
    Decatur Makers Arduino-based RFID Access Controller
    Written by: Bill Sandidge
    
    v0.1  02/21/2016
    v0.2  02/24/2016    
    v0.3  02/26/2016
    v0.4  02/29/2016  added in fob codes, all fobs activated
    
*/

#include <avr/pgmspace.h>
#include "fobs.h"
#include <EEPROM.h>
#include <Wire.h>
#include <RTClib.h>
#include <Time.h>

RTC_DS1307 rtc;

char fob_code[16];
int num_chars = 0;
char ch;
String last_str;
String new_str;
int fob_num;
boolean door_open;
int sysmode;

long door_tmr;    // door open pulse timer
const long DOOR_DLY = 10000;  // time to hold output on
const int DOOR_PIN = 4;  // using 4 so L1 led can be easily lit on shield

const int NUM_FOBS = 102;

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  pinMode(DOOR_PIN, OUTPUT);  // make the DOOR_PIN an OUTPUT
  init_rtc();    // initialize the Real Time Clock, get the current time
  
/* // Initialize the EEPROM, used to init EEPROM in new board
   for (int addr=0; addr<4096; addr++)
   {
     EEPROM.write(addr, 0); 
   }
   //EEPROM.write(3,1);    // deactivate fob 3 for test purposes
*/

}  // end of setup()

void loop() {

  switch (sysmode)
  {
    case 0:
      if (get_rfid() == 0) sysmode = 5;
      break;
    case 5:
      open_door(true);
      sysmode = 99;
      break;
    // insert other actions in here...
    case 99:
      if (millis() > door_tmr )
      {
        open_door(false);
        sysmode = 0;
      }
      break;
    default:
    
      break;  
  } 
} // end of loop()

/*

*/
int get_rfid(void){
  int rfid_result = 255;

  if (Serial1.available() > 0)  // if there are char's in serial buffer
  {  
    ch = (char)Serial1.read();    // read one char from buffer
    if (ch > 41 && ch < 71)      // if it is a valid character (0-9, A or B)
    {
      new_str += String(ch);    // add to string
      num_chars++;              // increment character counter
    }
    
    if (ch == 13 )   // end of string from reader?
    {
      if (num_chars == 21)    // correct number of characters?
      {
        if (new_str != last_str)    // new string means a fob scanned or key pressed
        {
          if (new_str.charAt(1) == 'F')    // is it a fob scanned?
          {
            for (int i=0; i<=NUM_FOBS; i++)    // check fob code
            {
              // get fob code from database in flash (program) memory
              strcpy_P(fob_code, (char*)pgm_read_word(&(fob_table[i])));  
              if (String(fob_code) == new_str.substring(2,17))    // if new fob code is in database
              {
                if (EEPROM.read(i) > 0)    // check to see if fob is active
                {
                    Serial.print("Fob ");
                    Serial.print(i);
                    Serial.println(" INACTIVE!");
                    door_open = false;
                    rfid_result = 1;
                    i = NUM_FOBS;                   
                }
                else
                {
                    Serial.print("Fob ");
                    Serial.print(i);
                    Serial.println(" accepted, opening door...");
                    door_open = true;
                    rfid_result = 0;
                    if (i == 0) Serial.println("Hello Lew... come on in!");
                    i = NUM_FOBS;
                }
              }
            }
            if (door_open == false) Serial.println("Bad fob! Access denied!  Go away!");
          }
          else if (new_str.charAt(1) == '0')    // new key press
          {
            //Serial.print("Key pressed: ");
            //Serial.println(new_str.substring(17,18));               
          }
          last_str = new_str; 
        }
      }  
      // reset for next string...
      num_chars = 0;  
      new_str = "";
      door_open = false;
    }
  } 
  return rfid_result; 
}

/*
 *  
*/
void open_door(boolean door_state)
{
  if (door_state == true)
  {
    digitalWrite(DOOR_PIN, HIGH);
    door_tmr = millis() + DOOR_DLY;    
  }
  else
  {
    digitalWrite(DOOR_PIN, LOW);    
  }
}

/*
 * 
*/
void init_rtc(void)
{
  if (! rtc.begin()) 
  {
    Serial.println("Couldn't find RTC");
  }
  else
  {
    if (! rtc.isrunning()) 
    {
      Serial.println("RTC is NOT running!");
      // following line sets the RTC to the date & time this sketch was compiled
      // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
      // This line sets the RTC with an explicit date & time, for example to set
      // January 21, 2014 at 3am you would call:  rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
      rtc.adjust(DateTime(2016, 2, 24, 12, 0, 0));
    }
    else
    {
      Serial.println("Decatur Makers RFID Access System"); 
      DateTime now = rtc.now();
      Serial.print(now.year(), DEC);
      Serial.print('/');
      Serial.print(now.month(), DEC);
      Serial.print('/');
      Serial.print(now.day(), DEC);
      Serial.print("   ");
      Serial.print(now.hour(), DEC);
      Serial.print(':');
      Serial.print(now.minute(), DEC);
      Serial.print(':');
      Serial.print(now.second(), DEC);
      Serial.println();
    }
  } 
}


// EOF


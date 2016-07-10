/*
    Decatur Makers Arduino-based RFID Access Controller
    Written by: Bill Sandidge
    
    v0.1  02/21/2016
    v0.2  02/24/2016    
    v0.3  02/26/2016
    v0.4  02/29/2016  added in fob codes, all fobs activated
    v0.5  07/10/2016  cleaned up code; added comments, descriptions; modified state machine
    
*/

// Included libraries for peripherals,special functions
#include <avr/pgmspace.h> // utilities for accessing program (flash) memory
#include "fobs.h"         // contains the codes for the RFID fobs
#include <EEPROM.h>       // utilities for accessing EEPROM memory
#include <Wire.h>         // i2c utilities for accessing the real time clock (RTC)
#include <RTClib.h>       // RTC utilities for the DS1307 real time clock chip
#include <Time.h>         // date/time utilities

RTC_DS1307 rtc;           // create an instance of the rtc class for timekeeping

char fob_code[16];        // string containing fob codes read from memory for comparison purposes
int num_chars = 0;        // character count from keypad serial port
char ch;                  // 
String last_str;          // last string received from the keypad 
String new_str;           // newest string received from the keypad
int fob_num;              // number of fob being checked
boolean door_open;        // true = open (unlock) the door, false = lock it
int sysmode;              // system state machine mode

long door_tmr;                      // door open timer
const long DOOR_DLY = 10000;        // time in milliseconds to hold door openoutput on
const int DOOR_OPEN_PIN = 4;        // using 4 so L1 led can be easily lit on shield

const int NUM_FOBS = 102; // number of fobs currently coded in the system

// Define sysmode values
#define SYS_INIT  0             // initial mode on startup
#define SYS_READ_KEYPAD 5       // normal mode, continuous scanning of keypad 
#define SYS_OPEN_DOOR 10        // open the door
#define SYS_LOG_EVENTS 30       // log any events
#define SYS_READ_HOST_PORT 50   // read the host port
#define SYS_CLOSE_DOOR 80       // close the door


/*
  Perform various initialization functions... 
*/
void setup() {
  Serial.begin(9600);               // host port init
  Serial1.begin(9600);              // keypad port init
  pinMode(DOOR_OPEN_PIN, OUTPUT);   // set the DOOR_PIN an OUTPUT
  init_rtc();                       // initialize the Real Time Clock, get the current time
  //init_eeprom();                  // clears eeprom ONLY ON NEW BOARD, ONE TIME!

}  // end of setup()

/*
 *  Main loop
 * 
 */
void loop() {
  // finite state machine, controls program flow
  switch (sysmode)
  {
    case SYS_INIT:
      // perform one-time init functions, if necessary...
      sysmode = SYS_READ_KEYPAD;

    case SYS_READ_KEYPAD:
      sysmode = SYS_READ_HOST_PORT;
      // if read_keypad() = 255, nothing happened, else...
      if (read_keypad() < 255)      // some event has occured...
        if (read_keypad() == 0)
        {
          // 0 = open door
          sysmode = SYS_OPEN_DOOR;
        }
        else
        {
          // NOT 0 means some other event (bad fob, inactive fob etc.)
          sysmode = SYS_LOG_EVENTS;
        }
      break;
    
    case SYS_OPEN_DOOR:
      open_door(true);                    // activate the door lock
      door_tmr = millis() + DOOR_DLY;     // set door lock timer
      sysmode = SYS_LOG_EVENTS;           // log the event
      break;

    case SYS_LOG_EVENTS:
      // log the event
      sysmode = SYS_READ_HOST_PORT;
      break;
      
    case SYS_READ_HOST_PORT:
      // read the host serial port
      sysmode = SYS_CLOSE_DOOR;
      break;
      
    case SYS_CLOSE_DOOR:
      if (millis() > door_tmr )
      {
        open_door(false);        
        sysmode = SYS_READ_KEYPAD;
      }
      break;
    
    default:
      sysmode = SYS_READ_KEYPAD;
      break;  
  } 
} // end of main loop()

/*

*/
int read_keypad(void){
  int read_result = 255;

  if (Serial1.available() > 0)    // if there are char's in serial buffer
  {  
    ch = (char)Serial1.read();    // read one char from buffer
    if (ch > 41 && ch < 71)       // if it is a valid character (0-9, A or B)
    {
      new_str += String(ch);      // add to string
      num_chars++;                // increment character counter
    }
    
    if (ch == 13 )   // end of string from reader?
    {
      if (num_chars == 21)    // correct number of characters?
      {
        if (new_str != last_str)    // new string means a fob scanned or key pressed
        {
          if (new_str.charAt(1) == 'F')    // is it a fob?
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
                    read_result = 1;
                    i = NUM_FOBS;                   
                }
                else
                {
                    Serial.print("Fob ");
                    Serial.print(i);
                    Serial.println(" accepted, opening door...");
                    door_open = true;
                    read_result = 0;
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
  return read_result; 
}

/*
 *  
*/
void open_door(boolean door_state)
{
  if (door_state == true)
  {
    digitalWrite(DOOR_OPEN_PIN, HIGH);
  }
  else
  {
    digitalWrite(DOOR_OPEN_PIN, LOW);    
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


/* 
 *  // Initialize the EEPROM, used to init EEPROM in new board ONLY
init_eeprom(void){
   for (int addr=0; addr<4096; addr++)
   {
     EEPROM.write(addr, 0); 
   }
   //EEPROM.write(3,1);    // deactivate fob 3 for test purposes
}
*/


// EOF


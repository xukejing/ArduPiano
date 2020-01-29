/* @file MultiKey.ino
|| @version 1.0
|| @author Mark Stanley
|| @contact mstanley@technologist.com
||
|| @description
|| | The latest version, 3.0, of the keypad library supports up to 10
|| | active keys all being pressed at the same time. This sketch is an
|| | example of how you can get multiple key presses from a keypad or
|| | keyboard.
|| #
*/
#include <MIDI.h>
#include <Keypad.h>
MIDI_CREATE_DEFAULT_INSTANCE();
const byte ROWS = 9; //four rows
const byte COLS = 8; //three columns
byte i=0;
byte j=0;
char keys[ROWS][COLS] = {0};
byte rowPins[ROWS] = {10,11,12,13,A0,A1,A2,A3,A4}; //connect to the row pinouts of the kpd
byte colPins[COLS] = {2,3,4,5,6,7,8,9}; //connect to the column pinouts of the kpd

Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

unsigned long loopCount;
unsigned long startTime;
String msg;


void setup() {
  for(i=0;i<ROWS;i++)
   for(j=0;j<COLS;j++)
   keys[i][j]=i*COLS+j;

  MIDI.begin();
    Serial.begin(115200);
    loopCount = 0;
    startTime = millis();
    msg = "";
}


void loop() {
    loopCount++;
    if ( (millis()-startTime)>5000 ) {
      //  Serial.print("Average loops per second = ");
     //   Serial.println(loopCount/5);
        startTime = millis();
        loopCount = 0;
    }

    // Fills kpd.key[ ] array with up-to 10 active keys.
    // Returns true if there are ANY active keys.
    if (kpd.getKeys())
    {
        for (int i=0; i<LIST_MAX; i++)   // Scan the whole key list.
        {
            if ( kpd.key[i].stateChanged )   // Only find keys that have changed state.
            {
                switch (kpd.key[i].kstate) {  // Report active key state : IDLE, PRESSED, HOLD, or RELEASED
                    case PRESSED:
                    msg = " PRESSED.";
                    MIDI.sendNoteOn(((byte)kpd.key[i].kchar)+36, 100, 1); 
                break;
                    case HOLD:
                    msg = " HOLD.";
                break;
                    case RELEASED:
                    msg = " RELEASED.";
                    MIDI.sendNoteOff(((byte)kpd.key[i].kchar)+36, 0, 1); 
                break;
                    case IDLE:
                    msg = " IDLE.";
                }
             //   Serial.print("Key ");
              //  Serial.print((byte)kpd.key[i].kchar);
              //  Serial.println(msg);
            }
        }
    }
}  // End loop

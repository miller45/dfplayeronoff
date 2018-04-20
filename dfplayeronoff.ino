//#include <SoftwareSerial.h>

#define TRIGGER_PIN 2
#define LED_PIN 13

#include <DFPlayer_Mini_Mp3.h>




bool needslf = false;

//SoftwareSerial sSerial(10, 11); // RX, TX
#define PLSerial Serial1


//
void setup () {
  //Serial.begin (9600);
  pinMode(LED_PIN, OUTPUT);
  pinMode(TRIGGER_PIN, INPUT);
  PLSerial.begin(9600);
  mp3_set_serial (PLSerial);	//set softwareSerial for DFPlayer-mini mp3 module
  delay(1);  //wait 1ms for mp3 module to set volume
  mp3_set_volume (28);
  delay(1);  //wait 1ms for mp3 module to set volume
  mp3_set_reply(LOW);
  delayWithBlink(10000);
  mp3_next();
  delay(3000);
  mp3_stop();
}

int cnt = 1;
int initcnt = 0;
int track = 1;
int stamill = 0;
//
void loop () {
  int mill = millis();
  switch (cnt) {
    case -1:
      if (mill % 200 == 0) {
        digitalWrite(LED_PIN, HIGH);
      } else if (mill % 300 == 0) {
        digitalWrite(LED_PIN, LOW);
      }
    case 0:
      //empty
      break;
    case 1:
      cnt = 2;
      digitalWrite(LED_PIN, HIGH);
      mp3_next();
      delay(200);
      digitalWrite(LED_PIN, LOW);
      break;
    case 2:
      //cnt = 3;
      //wait until serial interpretar switches to 1
      break;
    case 3:
      digitalWrite(LED_PIN, HIGH);
      delay(200);
      mp3_next();
      delay(200);
      digitalWrite(LED_PIN, LOW);
      cnt = 4;
      break;
    case 4:
      //if((mill % 5000000) == 0){
      //        cnt = 1;
      //}
      break;
    case 5:
      mp3_stop();
      cnt=0;
      break;
  }
  checkAndInspectSerial();
  if ((mill % 100) == 0) {
    checkTrigger();
  }

}

byte bytecount = 0;
bool paused = false;

void checkAndInspectSerial() {
  // read from port 0, send to port 1:
  if (PLSerial.available()) {
    byte inByte = PLSerial.read();
    bytecount++;
    if (inByte == 0x7E) {
      bytecount = 0;
    }
    if (bytecount < 10) {
      recv_buf[bytecount] = inByte;
    }

    if (inByte == 0xEF && bytecount > 8) {
      bytecount = 0;
      inspectResult();
    }
  }
}

void inspectResult() {
  switch (recv_buf[3]) {
    case 0x3D: //stopped playback
      cnt = 1;
      break;
    case 0x40: // error
      cnt = -1;
      break;
  }
}

void delayWithBlink(int dmills) {
  for (int i = 0; i < dmills / 1000; i++) {
    digitalWrite(LED_PIN, HIGH);
    delay(800);
    digitalWrite(LED_PIN, LOW);
    delay(200);
  }
}

int lastTrigState=-1;
void checkTrigger() {
  int state = digitalRead(TRIGGER_PIN);
  if(state!=lastTrigState){
    lastTrigState=state;
    if(state==HIGH){
       cnt=1;
    }else{
       cnt=5;
    }
  }
}

/*
   mp3_play ();		//start play
   mp3_play (5);	//play "mp3/0005.mp3"
   mp3_next ();		//play next
   mp3_prev ();		//play previous
   mp3_set_volume (uint16_t volume);	//0~30
   mp3_set_EQ ();	//0~5
   mp3_pause ();
   mp3_stop ();
   void mp3_get_state (); 	//send get state command
   void mp3_get_volume ();
   void mp3_get_u_sum ();
   void mp3_get_tf_sum ();
   void mp3_get_flash_sum ();
   void mp3_get_tf_current ();
   void mp3_get_u_current ();
   void mp3_get_flash_current ();
   void mp3_single_loop (boolean state);	//set single loop
   void mp3_DAC (boolean state);
   void mp3_random_play ();
*/


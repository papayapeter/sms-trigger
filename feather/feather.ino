//papayapeter
//2019
//
//if sms "on" is recieved -> led on; if "off" -> led off

// libraries -----------------------------------------------
#include <Metro.h>
#include <SoftwareSerial.h>
#include "Adafruit_FONA.h"

// pins ----------------------------------------------------
const uint8_t FONA_RX  = 9;
const uint8_t FONA_TX  = 8;
const uint8_t FONA_RST = 4;
const uint8_t FONA_RI  = 7;

const uint8_t LED      = 13;

// constants -----------------------------------------------

// objects -------------------------------------------------
SoftwareSerial fona_ss = SoftwareSerial(FONA_TX, FONA_RX);
SoftwareSerial *fona_serial = &fona_ss;

Adafruit_FONA fona = Adafruit_FONA(FONA_RST);

Metro timer(5000);

// variables -----------------------------------------------
int8_t sms_count;
char replybuffer[255];

// setup ---------------------------------------------------
void setup()
{
  // setup serial (debug)
  Serial.begin(115200);

  // set up sim800 communication
  fona_serial->begin(4800);
  while (!fona.begin(*fona_serial))
  {
    Serial.println("couldn't find fona");
    delay(1000);
  }
  Serial.println("found fona");

  digitalWrite(LED, HIGH);
  delay(2000);
  digitalWrite(LED, LOW);

  // delete all existing sms
  sms_count = fona.getNumSMS();
  for (int i = 1; i <= sms_count; i++)
  {
    fona.deleteSMS(i);
  }
  sms_count = 0;
}

// loop
void loop()
{
  // repeat every 5 seconds
  if (timer.check())
  {
    sms_count = fona.getNumSMS();
    Serial.println(sms_count);

    // if there are new sms
    if (sms_count > 0)
    {
      // read last sms
      uint16_t smslen;
      if (!fona.readSMS(sms_count, replybuffer, 250, &smslen))
      {
        Serial.println("failed");
      }

      // delete all sms
      for (int i = 1; i <= sms_count; i++)
      {
        fona.deleteSMS(i);
      }
      sms_count = 0;

      // convert buffer to string
      String sms = "";
      for (int i = 0; i < smslen; i++)
      {
        sms += replybuffer[i];
      }

      // debug
      Serial.println(replybuffer);
      Serial.println(sms);

      sms.toLowerCase();
      if (sms == "on")
      {
        digitalWrite(LED, HIGH);
      }
      else if (sms == "off")
      {
        digitalWrite(LED, LOW);
      }
    }
  }
}

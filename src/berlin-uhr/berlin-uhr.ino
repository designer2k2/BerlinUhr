// --------------------------------------------------------------------------------
// -- Includes
// --------------------------------------------------------------------------------
#include <Adafruit_TLC5947.h>
#include <DS3231.h>
#include <Wire.h>

// --------------------------------------------------------------------------------
// -- Global Variables
// --------------------------------------------------------------------------------
#define BtnAPin 6
#define BtnBPin 7

#define SecLedPin 0
const int Hour1Pin[4] = {1, 2, 3, 4};
const int Hour5Pin[4] = {5, 6, 7, 8};
const int Min5Pin[11] = {9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19};
const int Min1Pin[4] = {20, 21, 22, 23};

int iBrightness = 50;

// --------------------
// RTC
// --------------------
DS3231 m_oClock;
int m_iMin, m_iHour;
bool m_bH12, m_bPM;

// --------------------
// -- LED Controller
// --------------------
#define Data   10    // PA4
#define Clock   11   // PA5
#define Latch   8   // PA2
#define oe  1      // set to -1 to not use the enable pin (its optional)
Adafruit_TLC5947 oTLC = Adafruit_TLC5947(1, Clock, Data, Latch);

// --------------------------------------------------------------------------------
// -- Setup
// --------------------------------------------------------------------------------
void setup()
{
  // Start the I2C interface
  Wire.begin();
 
  // Start the serial interface
  Serial.begin(57600);

  pinMode(BtnAPin, INPUT_PULLUP);
  pinMode(BtnBPin, INPUT_PULLUP);

  oTLC.begin();
  pinMode(oe, OUTPUT);
  digitalWrite(oe, LOW);

  for (int i = 0; i < 23; i++) {
    oTLC.setPWM(i, 0); //0 = 0%, 4095 = 100%
  }
  oTLC.write();  
}

// --------------------------------------------------------------------------------
// -- Set LEDs
// --------------------------------------------------------------------------------
void setLedOn(int iLedPin)
{
  oTLC.setPWM(iLedPin, iBrightness);
}

void setLedOff(int iLedPin)
{
  oTLC.setPWM(iLedPin, 0);
}

void setSecLed(int iSec)
{
  // Sec even?
  if ((iSec % 2) == 0) {
    // -- LED on
    setLedOn(SecLedPin);
  }
  else {
    // -- LED off
    setLedOff(SecLedPin);
  }
}

void setMinLed(int iMin)
{
  if (iMin == m_iMin) {
    return;
  }

  m_iMin = iMin;

  // --------------------------------------------
  if (m_iMin == 0) {
    for (int i = 0; i < 11; i++) {
      // set Min5 off
      setLedOff(Min5Pin[i]);
    }
    for (int i = 0; i < 4; i++) {
      // set Min1 off
      setLedOff(Min1Pin[i]);
    }
  }
  
  // --------------------------------------------
  int iRest = m_iMin % 5;
  if (iRest == 0) {
    for (int i = 0; i < 4; i++) {
      // set Min1 off
      setLedOff(Min1Pin[i]);
    }
  }
  else {
    for (int i = 0; i < iRest; i++) {
      // set Min1 on
      setLedOn(Min1Pin[i]);
    }
  }

  // --------------------------------------------
  int iCount5 = (m_iMin - iRest) % 5;
  for (int i = 0; i < iCount5; i++) {
    // set Min5 on
    setLedOn(Min5Pin[i]);
  }
}

void setHourLed(int iHour)
{
  if (iHour == m_iHour) {
    return;
  }

  m_iHour = iHour;

  // --------------------------------------------
  if (m_iHour == 0) {
    for (int i = 0; i < 4; i++) {
      // set Hours off
      setLedOff(Hour5Pin[i]);
      setLedOff(Hour1Pin[i]);
    }
  }
  
  // --------------------------------------------
  int iRest = m_iHour % 5;
  if (iRest == 0) {
    for (int i = 0; i < 4; i++) {
      // set Hour1 off
      setLedOff(Hour1Pin[i]);
    }
  }
  else {
    for (int i = 0; i < iRest; i++) {
      // set Hour1 on
      setLedOn(Hour1Pin[i]);
    }
  }

  // --------------------------------------------
  int iCount5 = (m_iHour - iRest) % 5;
  for (int i = 0; i < iCount5; i++) {
    // set Hour5 on
    setLedOn(Hour5Pin[i]);
  }
}

// --------------------------------------------------------------------------------
// -- Main Loop
// --------------------------------------------------------------------------------
void loop()
{
  setSecLed(m_oClock.getSecond());
  setMinLed(m_oClock.getMinute());
  setHourLed(m_oClock.getHour(m_bH12, m_bPM));

  oTLC.write();
}

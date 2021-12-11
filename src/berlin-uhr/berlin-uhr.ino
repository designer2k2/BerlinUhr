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

#define LedPinOffset 3
#define SecLedPin 3
const int Hour1Pin[4] = {23, 24, 7, 6};
const int Hour5Pin[4] = {25, 26, 4, 5};
const int Min5Pin[11] = {22, 21, 18, 17, 16, 15, 14, 13, 11, 9, 8};
const int Min1Pin[4] = {20, 19, 12, 10};

int m_iBright = 50;

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
// -- Set LEDs
// --------------------------------------------------------------------------------
void setLedOn(int iLedPin)
{
  if (iLedPin < LedPinOffset) {
    return;
  }
  
  int iBright = round(4096 * m_iBright / 100);
  
  oTLC.setPWM((iLedPin - LedPinOffset), iBright);

  oTLC.write();
}

void setLedOff(int iLedPin)
{
  oTLC.setPWM(iLedPin, 0);

  oTLC.write();
}

// --------------------------------------------------------------------------------
// -- Set Sec, Min, Hour LEDs
// --------------------------------------------------------------------------------
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
// -- Tests
// --------------------------------------------------------------------------------
void testLedNumSeq()
{
  for (int i = LedPinOffset; i <= (23 + LedPinOffset); i++) {
    setLedOff(i);
  }

  delay(200);

  m_iBright = 25;
  while (m_iBright <= 100) {
    for (int i = LedPinOffset; i <= (23 + LedPinOffset); i++) {
      setLedOn(i);
      delay(200);
      setLedOff(i);
    }

    m_iBright += 25;
  }

  m_iBright = 50;
}

void testLedSecMinHour()
{
  for (int i = LedPinOffset; i <= (23 + LedPinOffset); i++) {
    setLedOff(i);
  }

  delay(200);

  m_iBright = 25;
  while (m_iBright <= 100) {
    setLedOn(SecLedPin);
    delay(200);
    setLedOff(SecLedPin);
    delay(200);
    
    for (int i = 0; i < 4; i++) {
      setLedOn(Hour5Pin[i]);
      delay(200);
      setLedOff(Hour5Pin[i]);
      delay(200);
    }
    
    for (int i = 0; i < 4; i++) {
      setLedOn(Hour1Pin[i]);
      delay(200);
      setLedOff(Hour1Pin[i]);
      delay(200);
    }
    
    for (int i = 0; i < 11; i++) {
      setLedOn(Min5Pin[i]);
      delay(200);
      setLedOff(Min5Pin[i]);
      delay(200);
    }
    
    for (int i = 0; i < 4; i++) {
      setLedOn(Min1Pin[i]);
      delay(200);
      setLedOff(Min1Pin[i]);
      delay(200);
    }

    m_iBright += 25;
  }

  m_iBright = 50;
}

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

  for (int i = LedPinOffset; i <= (23 + LedPinOffset); i++) {
    setLedOff(i);
  }
}

// --------------------------------------------------------------------------------
// -- Main Loop
// --------------------------------------------------------------------------------
void loop()
{
//  testLedNumSeq();

//  delay(200);

  testLedSecMinHour();

  delay(200);
  setSecLed(m_oClock.getSecond());
  setMinLed(m_oClock.getMinute());
  setHourLed(m_oClock.getHour(m_bH12, m_bPM));

  delay(1000);
}

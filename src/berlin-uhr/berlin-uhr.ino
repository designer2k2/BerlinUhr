// --------------------------------------------------------------------------------
// -- Includes
// --------------------------------------------------------------------------------
#include <Adafruit_TLC5947.h>
#include <DS3231.h>
#include <Wire.h>
#include <EEPROM.h>

// --------------------------------------------------------------------------------
// -- Global Variables for Tests
// --------------------------------------------------------------------------------
#define Delay4Tests 500

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

// --------------------
// Light Sensor
// Brightness
// --------------------
#define LightSensorPin 6
#define LightSensorMax 1024
#define BrightMin 20
// 1 = linear, 2 = log, 3 = x^2, 4 = sq(x)
#define BrightCurve 1
// 0 = off, 1 = middle to last, 2 = FIFO
#define BrightSmooth 0
#define BrightFifoNum 10int m_iBright = 5;
int m_iBrightFifo[BrightFifoNum];
int m_iBrightFifoIdx = 0;

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
void setBright()
{
  int iBrightTotal = 0;
  uint32_t uiBright = analogRead(LightSensorPin);

  // brightness curve
  switch (BrightCurve) {
    // linear
    case 1:
    default:
      uiBright = 4 * uiBright;
      break;
      
    // log
    case 2:
      // Read the light sensor value, log convert:
      uiBright = sq(uiBright);
      uiBright = uiBright / 256;
      break;

    // x^2
    case 3:
      uiBright = uiBright * uiBright / 400 + BrightMin;
      break;

    // sq
    case 4:
      uiBright = sq(uiBright) * 5 + BrightMin;
  }

  // set to minimum
  if (uiBright < BrightMin) {
    m_iBright = BrightMin;
  }

  // smoothing
  switch (BrightSmooth) {
    case 1:
      uiBright = (uiBright + m_iBright) / 2;
      break;

    case 2:
      m_iBrightFifoIdx++;
      if (m_iBrightFifoIdx == BrightFifoNum) {
        m_iBrightFifoIdx = 0;
      }
      m_iBrightFifo[m_iBrightFifoIdx] = uiBright;
      for (int i = 0; i < BrightFifoNum; i++) {
        iBrightTotal += m_iBrightFifo[i];
      }
      uiBright = iBrightTotal / BrightFifoNum;
      
      break;

    default:
      break;
  }

  m_iBright = (int)uiBright;
}

void setLedOn(int iLedPin)
{
  if (iLedPin < LedPinOffset) {
    return;
  }
  
  oTLC.setPWM((iLedPin - LedPinOffset), m_iBright);
}

void setLedOff(int iLedPin)
{
  oTLC.setPWM((iLedPin - LedPinOffset), 0);
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
  m_iMin = iMin;

  // --------------------------------------------
  for (int i = 0; i < 11; i++) {
    // set Min5 off
    setLedOff(Min5Pin[i]);
  }
  for (int i = 0; i < 4; i++) {
    // set Min1 off
    setLedOff(Min1Pin[i]);
  }
  
  // --------------------------------------------
  int iRest = m_iMin % 5;
  if (iRest != 0) {
    for (int i = 0; i < iRest; i++) {
      // set Min1 on
      setLedOn(Min1Pin[i]);
    }
  }

  // --------------------------------------------
  int iCount5 = (m_iMin - iRest) / 5;
  for (int i = 0; i < iCount5; i++) {
    // set Min5 on
    setLedOn(Min5Pin[i]);
  }
}

void setHourLed(int iHour)
{
  m_iHour = iHour;

  // --------------------------------------------
  for (int i = 0; i < 4; i++) {
    // set Hours off
    setLedOff(Hour5Pin[i]);
  }
  for (int i = 0; i < 4; i++) {
    // set Hours off
    setLedOff(Hour1Pin[i]);
  }
  
  // --------------------------------------------
  int iRest = m_iHour % 5;
  if (iRest != 0) {
    for (int i = 0; i < iRest; i++) {
      // set Hour1 on
      setLedOn(Hour1Pin[i]);
    }
  }

  // --------------------------------------------
  int iCount5 = (m_iHour - iRest) / 5;
  for (int i = 0; i < iCount5; i++) {
    // set Hour5 on
    setLedOn(Hour5Pin[i]);
  }
}

// --------------------------------------------------------------------------------
// -- change time
// --------------------------------------------------------------------------------
void incMin()
{
  int iMin = m_oClock.getMinute();
  int iHour = m_oClock.getHour(m_bH12, m_bPM);

  if (iMin == 59) {
    iMin = 0;
    iHour++;
    if (iHour == 24) {
      iHour = 0;
    }
  }
  else {
    iMin++;
  }

  m_oClock.setHour(iHour);
  m_oClock.setMinute(iMin);
  m_oClock.setSecond(0);
}

void decMin()
{
  int iMin = m_oClock.getMinute();
  int iHour = m_oClock.getHour(m_bH12, m_bPM);

  if (iMin == 0) {
    iMin = 59;
    iHour--;
    if (iHour < 0) {
      iHour = 23;
    }
  }
  else {
    iMin--;
  }

  m_oClock.setHour(iHour);
  m_oClock.setMinute(iMin);
  m_oClock.setSecond(0);
}

// --------------------------------------------------------------------------------
// -- Tests
// --------------------------------------------------------------------------------
void testLedNumSeq()
{
  for (int i = LedPinOffset; i <= (23 + LedPinOffset); i++) {
    setLedOff(i);

    oTLC.write();
  }

  delay(Delay4Tests);

  m_iBright = 250;
  while (m_iBright <= 1000) {
    for (int i = LedPinOffset; i <= (23 + LedPinOffset); i++) {
      setLedOn(i);
      oTLC.write();
      delay(Delay4Tests);
      setLedOff(i);
      oTLC.write();
    }

    m_iBright += 250;
  }

  m_iBright = 500;
}

void testSim24h()
{
  m_iBright = 500;

  setSecLed(0);
  
  for (int h = 0; h < 24; h++) {
    for (int m = 0; m < 60; m++) {
      setHourLed(h);
      setMinLed(m);
    
      oTLC.write();
    
      delay(200);
    }
  }
}

// --------------------------------------------------------------------------------
// -- Setup
// --------------------------------------------------------------------------------
void setCompileTime()
{
  const int iAddr = 0;
  const int iFirstStart = 1;

  // TODO: check RTC time is older than compile time
  
  if (EEPROM.read(iAddr) != iFirstStart) {
    DateTime oDT(__DATE__, __TIME__);
    
    m_oClock.setYear(oDT.year());
    m_oClock.setMonth(oDT.month());
    m_oClock.setDate(oDT.day());
    m_oClock.setHour(oDT.hour());
    m_oClock.setMinute(oDT.minute());
    m_oClock.setSecond(0);
    
    EEPROM.write(iAddr, iFirstStart); 

    delay(500);
  }
}

void setup()
{
  // Start the I2C interface
  Wire.begin();
  Wire.setClock(400000);

  pinMode(BtnAPin, INPUT_PULLUP);
  pinMode(BtnBPin, INPUT_PULLUP);

  m_oClock.setClockMode(false);
  
  oTLC.begin();
  pinMode(oe, OUTPUT);
  digitalWrite(oe, LOW);

  m_iMin = 0;
  m_iHour = 0;

  setCompileTime();
  
  for (int i = LedPinOffset; i <= (23 + LedPinOffset); i++) {
    setLedOff(i);
  }

  for (int i = 0; i < BrightFifoNum; i++) {
    m_iBrightFifo[i] = 20;
  }
}

// --------------------------------------------------------------------------------
// -- Main Loop
// --------------------------------------------------------------------------------
void loop()
{
/*
  testSim24h();
*/

  if (!digitalRead(BtnAPin)) {
    incMin();
  }
  if (!digitalRead(BtnBPin)) {
    decMin();
  }

  setBright();

  setSecLed(m_oClock.getSecond());
  setMinLed(m_oClock.getMinute());
  setHourLed(m_oClock.getHour(m_bH12, m_bPM));

  oTLC.write();
 
  delay(200);
}

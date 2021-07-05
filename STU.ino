/*
This code is to be integrated in a large codebase
- procede any variable or function name with an
  abbreviation related to your sub task
*/
#include <Servo.h>

/* Likely to be used by other sub teams */
// Time
unsigned long currentMillis = 0;
// Serail Number
int SerialPort = 9600;
/* ---------------- END ----------------*/

// we define the horizontal servomotors
Servo STU_servohori;
const byte STU_servohoriPIN = 10;
int STU_servoh = 0;
int STU_servohLimitHigh = 160;
int STU_servohLimitLow = 60;
// we define the vertical servomotors
Servo STU_servoverti;
const byte STU_servovertiPIN = 11;
int STU_servov = 0;
int STU_servovLimitHigh = 160;
int STU_servovLimitLow = 60;
// Photoresist pin
int STU_ldrtopl = A10;  //top left
int STU_ldrtopr = A9;   //top right
int STU_ldrbotl = A11;  // bottom left
int STU_ldrbotr = A8;   // bottom right
// Intial Angel 
int DefaultAngel = 60;
// Measure loop Control
unsigned long STU_Measure_interval = 50;
unsigned long STU_Measure_previousMills = 0;
// Effect delay
unsigned long STU_Effect_interval = 10;
unsigned long STU_Effect_previousMills = 0;

void setup ()
{
  Serial.begin(SerialPort);
  // REVIEWME:

  STU_servohori.attach(STU_servohoriPIN);
  STU_servohori.write(DefaultAngel);
  STU_servoverti.attach(STU_servovertiPIN);
  STU_servoverti.write(DefaultAngel);

  // STU team requesting intialdelay of 500 at least before loop
}

void loop()
{
  SolarTrackingUnit();
}

void SolarTrackingUnit() {
  // [1]: Get current angel
  STU_servoh = STU_servohori.read();
  STU_servov = STU_servoverti.read();

  // [2]: Get Analog value of the photoresistor
  int topl = analogRead(STU_ldrtopl);
  int topr = analogRead(STU_ldrtopr);
  int botl = analogRead(STU_ldrbotl);
  int botr = analogRead(STU_ldrbotr);

  Serial.println(topl);
  // [3]: Get average
  int avgtop   = (topl + topr); //average of top
  int avgbot   = (botl + botr); //average of bottom
  int avgleft  = (topl + botl); //average of left
  int avgright = (topr + botr); //average of right

  // [4]: Get current time
  // REVIEWME:
  currentMillis = millis();
  if ((unsigned long)(currentMillis - STU_Measure_previousMills) >= STU_Measure_interval) {
    // [5]: Operate Vertically
    STU_OperateVertical(avgtop, avgbot);
    // [6]: Operate Horizentally
    STU_OperateHorizental(avgleft, avgright);
    STU_Measure_previousMills = millis();
  }
}

void STU_OperateVertical(int avgtop, int avgbot) {
  if (avgtop < avgbot)
  {
    STU_servoverti.write(STU_servov + 1);
    if (STU_servov > STU_servovLimitHigh)
    {
      STU_servov = STU_servovLimitHigh;
    }
  }
  else if (avgbot < avgtop)
  {
    STU_servoverti.write(STU_servov - 1);
    if (STU_servov < STU_servovLimitLow)
    {
      STU_servov = STU_servovLimitLow;
    }
  }
  else
  {
    STU_servoverti.write(STU_servov);
  }
}

void STU_OperateHorizental(int avgleft, int avgright) {
  if (avgleft > avgright)
  {
    STU_servohori.write(STU_servoh + 1);
    if (STU_servoh > STU_servohLimitHigh)
    {
      STU_servoh = STU_servohLimitHigh;
    }
  }
  else if (avgright > avgleft)
  {
    STU_servohori.write(STU_servoh - 1);
    if (STU_servoh < STU_servohLimitLow)
    {
      STU_servoh = STU_servohLimitLow;
    }
  }
  else
  {
    STU_servohori.write(STU_servoh);
  }
}

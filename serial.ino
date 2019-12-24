
/*
 * GPS TEST
 * MKRGSM1400
 * SERCOM TEST (MAKE NEW SERIAL PORT)
 * 
 */
#include <Arduino.h>
/*

//NEW SERIAL
// | 11         | SDA              |  PA08  |                 |   NMI  | *16 |     | X00 |     |  *0/00  |   2/00  | TCC0/0 | TCC1/2 | I2S/SD1  |          |
// | 12         | SCL              |  PA09  |                 |   09   | *17 |     | X01 |     |  *0/01  |   2/01  | TCC0/1 | TCC1/3 | I2S/MCK0 |          |
Uart Serial3 (&sercom0, 11, 12, SERCOM_RX_PAD_0, UART_TX_PAD_2);
void SERCOM0_Handler() { Serial3.IrqHandler(); }
//IN SETUP
//pinPeripheral(11, PIO_SERCOM);
//pinPeripheral(12, PIO_SERCOM);
*/

#define SerialGPS Serial1  // RX&TX PINS
//#define SerialGPS Serial3 // SCL&SDA PINS


void setup() {
//pinPeripheral(11, PIO_SERCOM);
//pinPeripheral(12, PIO_SERCOM);
  Serial.begin(9600); //USB
  SerialGPS.begin(9600); //TEST GPS.

  while(!Serial) { ; } //WAIT FOR SERIAL USB.
  Serial.print("\nTEST SOMETHING\n\n");
  
}

#define CHECK_INTERVAL 5  //seconds.
unsigned long previousTest;
bool mGPS_got_line = false, mGPS_paused = false;
uint8_t mGPS_idx=0;
char mGPS_TempLine[120];

void loop() {

if (millis() - previousTest >= CHECK_INTERVAL*1000UL) 
{
  previousTest = millis();
  Serial.print("\n\nGET NEW LINE\n\n");
  if (mGPS_paused) mGPS_paused = false; 
}
char mGPS = 0;
if ((SerialGPS.available() > 0) && !mGPS_got_line && !mGPS_paused) 
{
  mGPS = SerialGPS.read();
  //Serial.write(mGPS);   //DEBUG
  if (mGPS == '\n') 
  {
    mGPS_TempLine[mGPS_idx] = 0; mGPS_idx = 0; mGPS_got_line = true;
  } 
  else
  {
     mGPS_TempLine[mGPS_idx++] = mGPS;
     if (mGPS_idx >= 120) mGPS_idx = 119;
  }
}
if (mGPS_got_line)
{
  if (strstr(mGPS_TempLine, "$GPRMC"))
  {
    Serial.print("-->");Serial.println(mGPS_TempLine);
    //DO SOMETHING WITH THE LINE.
    
    //done parsed.
    mGPS_paused = true; mGPS_got_line = false; //Reset to get a new after paused has been reset.
  }
  else
  {
    mGPS_got_line = false; //wrong line not GPRMC. get another.
  }
}

} //END LOOP

/*

  Arduino Dashboard
  
*/


#include "U8glib.h"
#include "stdlib.h"
// setup u8g object, please remove comment from one of the following constructor calls
// IMPORTANT NOTE: The following list is incomplete. The complete list of supported 
// devices with all constructor calls is here: https://github.com/olikraus/u8glib/wiki/device



int enA = 7;
int inA1 = 22;
int inA2 = 24;
int inB1 = 26;
int inB2 = 28;
int enB = 6;



U8GLIB_SH1106_128X64 u8g(10, 9, 12, 11, 8);

void demoOne()
{
  // this function will run the motors in both directions at a fixed speed
  // turn on motor A
  digitalWrite(inA1, HIGH);
  digitalWrite(inA2, LOW);
  // set speed to 200 out of possible range 0~255
  analogWrite(enA, 200);
  // turn on motor B
  digitalWrite(inB1, HIGH);
  digitalWrite(inB2, LOW);
  // set speed to 200 out of possible range 0~255
  analogWrite(enB, 200);
  delay(2000);
  // now change motor directions
  digitalWrite(inA1, LOW);
  digitalWrite(inA2, HIGH);  
  digitalWrite(inB1, LOW);
  digitalWrite(inB2, HIGH); 
  delay(2000);
  // now turn off motors
  digitalWrite(inA1, LOW);
  digitalWrite(inA2, LOW);  
  digitalWrite(inB1, LOW);
  digitalWrite(inB2, LOW);
}
void demoTwo()
{
  // this function will run the motors across the range of possible speeds
  // note that maximum speed is determined by the motor itself and the operating voltage
  // the PWM values sent by analogWrite() are fractions of the maximum speed possible 
  // by your hardware
  // turn on motors
  digitalWrite(inA1, LOW);
  digitalWrite(inA2, HIGH);  
  digitalWrite(inB1, LOW);
  digitalWrite(inB2, HIGH); 
  // accelerate from zero to maximum speed
  for (int i = 0; i < 256; i++)
  {
    analogWrite(enA, i);
    analogWrite(enB, i);
    delay(20);
  } 
  // decelerate from maximum speed to zero
  for (int i = 255; i >= 0; --i)
  {
    analogWrite(enA, i);
    analogWrite(enB, i);
    delay(20);
  } 
  // now turn off motors
  digitalWrite(inA1, LOW);
  digitalWrite(inA2, LOW);  
  digitalWrite(inB1, LOW);
  digitalWrite(inB2, LOW);  
}



void draw(void) {
  // graphic commands to redraw the complete screen should be placed here  
  u8g.setFont(u8g_font_unifont);
  //u8g.setFont(u8g_font_osb21);
  u8g.drawStr( 15, 22, "Hello World!");
  u8g.drawStr( 15, 50, "Dashboard Test");
  u8g.drawCircle(20, 20, 14);
  
}

void drawAnimation(int x, int y){
  u8g.setFont(u8g_font_unifont);
  u8g.drawStr( 45, 60, "Y:");
  u8g.drawStr( 5, 60, "X:");
  u8g.drawStr( 5, 15, "Vin:");
  u8g.drawStr( 83, 15, "V");
  u8g.drawStr( 93, 15, "DC");
  u8g.drawStr( 60, 60, itoa(y, "%d", 10));
  u8g.drawStr( 20, 60, itoa(x, "%d", 10));
  char myBuffer[20] = "";
  float myVoltage = readVoltage();
  u8g.drawStr( 50, 15, dtostrf(myVoltage, 4, 1, myBuffer));
  
  

  u8g.drawCircle(x, y, 14);
  
//  Serial.print("X is: ");
//  Serial.print(x);
//  Serial.print("    Y is: ");
//  Serial.println(y);
  //draw();
}


int voltmeterAnalogInput = 0; //Use Analog Pin 0 for Voltmeter
float R1 = 30000.0;
float R2 = 7500.0;


float readVoltage(void){
  int sValue = analogRead(voltmeterAnalogInput);
  float vOut = (sValue * 5.0) / 1024.0;
  float vIn = vOut / (R2/(R1+R2));
//  Serial.print("Voltage is: ");
//  Serial.print(vIn);
//  Serial.println(" Volts DC");
  return vIn;
}



void setup(void) {
  // flip screen, if required
  // u8g.setRot180();
  
  // set SPI backup if required
  //u8g.setHardwareBackup(u8g_backup_avr_spi);

  // assign default color value
  if ( u8g.getMode() == U8G_MODE_R3G3B2 ) {
    u8g.setColorIndex(255);     // white
  }
  else if ( u8g.getMode() == U8G_MODE_GRAY2BIT ) {
    u8g.setColorIndex(3);         // max intensity
  }
  else if ( u8g.getMode() == U8G_MODE_BW ) {
    u8g.setColorIndex(1);         // pixel on
  }
  else if ( u8g.getMode() == U8G_MODE_HICOLOR ) {
    u8g.setHiColorByRGB(255,255,255);
  }
  Serial.begin(9600);
  pinMode(8, OUTPUT);
  pinMode(voltmeterAnalogInput, INPUT);
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(inA1, OUTPUT);
  pinMode(inA2, OUTPUT);
  pinMode(inB1, OUTPUT);
  pinMode(inB2, OUTPUT);
}

void powerBacklight(int modifier){
  digitalWrite(inA1, LOW);
  digitalWrite(inA2, HIGH);
  analogWrite(enA, modifier);
  //Serial.print("Modifier is: ");
  Serial.println(modifier);
  //delay(1000);
  digitalWrite(inA1, LOW);
  digitalWrite(inA2, LOW);
}

int xOffset = 0;
int yOffset = 0;


void loop(void) {
  // picture loop
  readVoltage();  
  u8g.firstPage();  
  do {
    drawAnimation(xOffset, 20 + 20 * sin(3.14/180*yOffset*10));
    powerBacklight(254 * abs(sin(3.14/180*yOffset*1)));
  } while( u8g.nextPage() );
  if (xOffset > 250) {
    xOffset = 0;
  }
  //analogWrite(2, 50);
  analogWrite(2, abs(xOffset));
  yOffset++;
  xOffset++;
  
   
  
  // rebuild the picture after some delay
  //delay(50);
}


 
#include "Energia.h"
#include "inc/hw_ints.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"

static const unsigned Hz=1;
String inputString = "";         // a string to hold incoming data
String time = "";
boolean stringComplete = false;  // whether the string is complete
unsigned int second = 0;
unsigned int minute = 0;
unsigned int hour = 0;
unsigned int analog_data = 0;
unsigned int average_samples = 256;

void initTimer(unsigned Hz)
{
  SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0); 
  TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC); 
  unsigned long ulPeriod = (SysCtlClockGet() / Hz) / 1; 
  TimerLoadSet(TIMER0_BASE, TIMER_A, ulPeriod -1); 
  IntEnable(INT_TIMER0A); 
  TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT); 
  TimerIntRegister(TIMER0_BASE, TIMER_A, Timer0IntHandler); 
  TimerEnable(TIMER0_BASE, TIMER_A);
}
 
void Timer0IntHandler()
{
  // Clear the timer interrupt
  TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
  second = second + 1;
  if(second == 60){
    second = 0;
    minute = minute + 1;
    if(minute == 60){
      minute = 0;
      hour = hour +1;
      if(hour == 24){
        hour = 0;        
      }
    }
  }
  time = "Elapsed time: " + String(hour);
  time = time + "h ";
  time = time + String(minute);
  time = time + "min ";
  time = time + String(second);
  time = time + "s"; 
  
}


 


/*================================================================
*                Main routines
*===============================================================*/
void setup()
{    
  pinMode(BLUE_LED,OUTPUT);
  initTimer(Hz);  
  Serial.begin(1000000); // msp430g2231 must use 4800  
  inputString.reserve(200); // reserve 200 bytes for the inputString:
}
 
void loop() {
  // print the string when a newline arrives:
  if (stringComplete) {
    
    // verify and execute the imput function:
   
   // LED_BLUE:
   if(inputString.substring(0,5) == "led_b"){
     String number = inputString.substring(5,8);
     int brightness = number.toInt();
     
     if(brightness > 100){
       brightness = 100;
     }
     if(brightness < 0){
       brightness = 0;
     }
     
     // set the brightness of blue led:
     analogWrite(BLUE_LED, brightness);
   }
   
   // LED_RED:
   if(inputString.substring(0,5) == "led_r"){
     String number = inputString.substring(5,8);
     int brightness = number.toInt();
     
     if(brightness > 100){
       brightness = 100;
     }
     if(brightness < 0){
       brightness = 0;
     }
     
     // set the brightness of red led:
     analogWrite(RED_LED, brightness);
   }
   
   // LED_GREEN:
   if(inputString.substring(0,5) == "led_g"){
     String number = inputString.substring(5,8);
     int brightness = number.toInt();
     
     if(brightness > 100){
       brightness = 100;
     }
     if(brightness < 0){
       brightness = 0;
     }
     
     // set the brightness of green led:
     analogWrite(GREEN_LED, brightness);
   }
   
   // Analog read 0
   if(inputString.substring(0,5) == "ain_0"){
     for(int x = 0; x < average_samples; x = x + 1){
       int sensorValue = analogRead(A0);
       analog_data = analog_data + sensorValue;
     }
     analog_data = analog_data/average_samples;
     Serial.println(analog_data);
   }
   
   // Analog read 1
   if(inputString.substring(0,5) == "ain_1"){
     for(int x = 0; x < average_samples; x = x + 1){
       int sensorValue = analogRead(A1);
       analog_data = analog_data + sensorValue;
     }
     analog_data = analog_data/average_samples;
     Serial.println(analog_data);
   }
   
   // Analog read 2
   if(inputString.substring(0,5) == "ain_2"){
     for(int x = 0; x < average_samples; x = x + 1){
       int sensorValue = analogRead(A2);
       analog_data = analog_data + sensorValue;
     }
     analog_data = analog_data/average_samples;
     Serial.println(analog_data);
   }
   
   // Analog read 3
   if(inputString.substring(0,5) == "ain_3"){
     for(int x = 0; x < average_samples; x = x + 1){
       int sensorValue = analogRead(A3);
       analog_data = analog_data + sensorValue;
     }
     analog_data = analog_data/average_samples;
     Serial.println(analog_data);
   }
   
   // Analog read ldr
   if(inputString.substring(0,5) == "ldrin"){
     for(int x = 0; x < average_samples; x = x + 1){
       int sensorValue = analogRead(A11);
       analog_data = analog_data + sensorValue;
     }
     analog_data = analog_data/average_samples;
     Serial.println(analog_data);
   }
   
   if(inputString.substring(0,5) == "times"){
     Serial.println(time);
   }
   
    //Serial.println(inputString);
    // clear the string:
    inputString = "";
    stringComplete = false;
    
  }
}

void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    //Serial.print(inChar);
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == ';') {
      //Serial.print('\n');    
      stringComplete = true;
    } 
  }
}

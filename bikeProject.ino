/*************************************************************************
 * ALL OUTPUTS TO RELAY ARE REVERSED!
 * This has to be done because the two channel relay has a reverse
 * input, enabled on low, disable on high. All outputs start HIGH and if
 * enabled go to low
 *************************************************************************/

//#define DEBUG
#include "DebugUtils.h"
#include <Wire.h>
#include <RTClib.h>

RTC_DS1307 rtc;

//Constants
const int  buttonPin = 8;    //the pin that the button is attached to
const int  relayPin = 5;     //the pin that the relay is attached to
const int  garagePin = 3;    //the pin for the garage door opener

//Variables
int buttonPushCounter = 0;   //counter for the number of button presses, default 0
int buttonState = 0;         //current state of the button, default 0
int lastButtonState = 0;     //previous state of the button, default 0
bool override = false;       //used to toggle LED override
bool relayMessage = true;
unsigned long startTime;     //used to store start-time loop
DateTime now;                //used to hold current time polled every couple milliseconds

void setup()
{
  //initialize the button pin as an input
  pinMode(buttonPin, INPUT);
  
  //initialize the relay as an output
  pinMode(relayPin, OUTPUT);
  digitalWrite(garagePin, HIGH);
  
  //initialize the garage door pin as an output
  pinMode(garagePin, OUTPUT);
  digitalWrite(garagePin, HIGH);
  
  // initialize serial communication
  Serial.begin(9600);

  #ifdef AVR
    Wire.begin();
  #else
    Wire1.begin(); // Shield I2C pins connect to alt I2C bus on Arduino Due
  #endif
  rtc.begin();

  //If debug mode is un-commented then this message will display
  DEBUG_PRINT("Debug Mode Enabled");
}


void loop()
{

  //variable of type DateTime
  now = rtc.now();
  
  startTime = 0; // start time set/reset to 0

  //read the pushbutton input pin:
  buttonState = digitalRead(buttonPin);

  //compare the buttonState to its previous state
  if (buttonState != lastButtonState)
  {
    //if the state has changed, increment the counter
    if (buttonState == HIGH)
    {
      //if the current state is HIGH then the button has been pushed
      
      buttonPushCounter++;
      DEBUG_PRINT("button pushed, counter incrementing");
      
      startTime = millis(); //used to calculate how long to remain in while loop
      DEBUG_PRINT("startTime Set");
    }
    
    // Delay a little bit to avoid bouncing
    delay(50);
  }
  
  //save the current state as the last state for rerun
  lastButtonState = buttonState;

  //Loop for 1 second, unless startTime equals 0
  //(button has not been pressed yet)
  while(millis() - startTime < 1000 && startTime != 0)
  {
    buttonState = digitalRead(buttonPin);
    
    //compare the buttonState to its previous state
    if (buttonState != lastButtonState)
    {
      //if the state has changed, increment the counter
      if (buttonState == HIGH)
      {
        //if the current state is HIGH then the button has been pushed
        buttonPushCounter++;
        DEBUG_PRINT("button pushed inside millis() loop, counter incrementing");
      }
      // Delay a little bit to avoid bouncing
      delay(50);
    }
    // save the current state as the last state,
    //for next time through the loop
    lastButtonState = buttonState;
  } //while(millis() - startTime < 1000 && startTime != 0)

  // toggles pin on the second button press
  if(buttonPushCounter == 1)
  {
    DEBUG_PRINT("garage door opener");
    digitalWrite(garagePin, LOW);
    DEBUG_PRINT("pin low");
    delay(750);
    digitalWrite(garagePin, HIGH);
    DEBUG_PRINT("pin high");
    
    DEBUG_PRINT("Delaying 15 seconds");
    delay(15000);

    digitalWrite(garagePin, LOW);
    DEBUG_PRINT("pin low");
    delay(750);
    digitalWrite(garagePin, HIGH);
    DEBUG_PRINT("pin high");
  }
  //toggles pin on 1 button press
  else if(buttonPushCounter == 2)
  {
    digitalWrite(relayPin, !digitalRead(relayPin));
    override = !override;
    DEBUG_PRINT("relay toggled");
  }
  else if(buttonPushCounter > 2)
  {
    DEBUG_PRINT("buttonPushCounter > 2");
  }

  buttonPushCounter = 0;

  if((now.hour() >= 19 || now.hour() < 7) && !override)
  {
    if(relayMessage)
    {
      DEBUG_PRINT("Time is later then 7PM and earlier then 7AM: Toggling Relay On");
      relayMessage = false;
    }
    digitalWrite(relayPin, LOW);
  }
  else if((now.hour() < 19 || now.hour() >= 7) && !override)
  {
    if(!relayMessage)
    {
      DEBUG_PRINT("Time is later then 7AM and earlier then 7PM: Toggling Relay Off");
      relayMessage = true;
    }
    digitalWrite(relayPin, HIGH);
  }
}

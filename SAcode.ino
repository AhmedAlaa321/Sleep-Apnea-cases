#include <SoftwareSerial.h>
SoftwareSerial BTserial (1,0);
#include <LiquidCrystal.h>
#define USE_ARDUINO_INTERRUPTS true    // Set-up low-level interrupts for most acurate BPM math.
#include <PulseSensorPlayground.h>     // Includes the PulseSensorPlayground Library.   
LiquidCrystal lcd(12,11,5,4,3,7);
//  Variables
int flowPin = 2;    //This is the input pin on the Arduino
double flowRate;    //This is the value we intend to calculate. 
volatile int count; //This integer needs to be set as volatile to ensure it updates correctly during the interrupt process. 
float Total = 0;
float Avg = 0; 
float totalMyBpm=0;
float counter=0;
float totalAvrage;
float totalpmAvrage;
const int PulseWire = 0;       // PulseSensor PURPLE WIRE connected to ANALOG PIN 0
const int LED13 = 13;
int LED1 = 6 ;
int LED0 = 9 ;
int Threshold = 550;
int buzzer=8;// Determine which Signal to "count as a beat" and which to ignore.
int Signal;                               // Use the "Gettting Started Project" to fine-tune Threshold Value beyond default setting.
                               // Otherwise leave the default "550" value. 
String status="normal";
                               
PulseSensorPlayground pulseSensor;  // Creates an instance of the PulseSensorPlayground object called "pulseSensor"


void setup() {   
  BTserial.begin (9600);
  Serial.begin(9600);          // For Serial Monitor
  pinMode(flowPin, INPUT);           //Sets the pin as an input
  attachInterrupt(0, Flow, RISING); 
  // Configure the PulseSensor object, by assigning our variables to it. 
  pulseSensor.analogInput(PulseWire);   
  pinMode(LED1,OUTPUT);
  pinMode(LED0,OUTPUT);
  pinMode(LED13,OUTPUT);  
  pulseSensor.setThreshold(Threshold);   
  lcd.begin(16,2);
  lcd.clear( );
  pinMode(buzzer,OUTPUT);
  // Double-check the "pulseSensor" object was created and "began" seeing a signal. 
   if (pulseSensor.begin()) {
    Serial.println("We created a pulseSensor Object !");  //This prints one time at Arduino power-up,  or on Arduino reset.  
  }
}



void loop() {
 count = 0;      // Reset the counter so we start counting from 0 again
  interrupts();   //Enables interrupts on the Arduino
  delay (1000);   //Wait 1 second 
  noInterrupts(); //Disable the interrupts on the Arduino
  
  //Start the math
  flowRate = (count * 2.25);        //Take counted pulses in the last second and multiply by 2.25mL 
  flowRate = flowRate * 60;         //Convert seconds to minutes, giving you mL / Minute
  flowRate = flowRate / 1000;       //Convert mL to Liters, giving you Liters / Minute
  Serial.print("the float rate is ");
  Serial.println(flowRate);         //Print the variable flowRate to Serial
  Total = Total + flowRate;
 /* Avg = Total / 60;
  Serial.print("Avg : "); 
  Serial.println(Avg);*/ 
  counter++;
  if(counter == 20){
     totalAvrage = avrage(Total);
     Serial.print("the flow avg = ");
     Serial.println(totalAvrage);
     BTserial.print(totalAvrage);
     BTserial.print(" L/min");
     BTserial.print(",");
     Total=0;
     
    }else{
      Serial.println("the counter is not 20 yet");
      }
 int myBPM = pulseSensor.getBeatsPerMinute();  // Calls function on our pulseSensor object that returns BPM as an "int".
 totalMyBpm+=myBPM;
  if(counter == 20){
     totalpmAvrage = avrage(totalMyBpm);
     Serial.print("my Bpm avg = ");
     Serial.println(totalpmAvrage);
     BTserial.println(totalpmAvrage);
     BTserial.println(" Bpm");
     BTserial.println(",");
     totalMyBpm=0;
     
    }else{
      Serial.println("the counter is not 20 yet");
      }
 
   Signal = analogRead(PulseWire);  // Read the PulseSensor's value. 
                                              // Assign this value to the "Signal" variable.

   Serial.println(Signal);                    // Send the Signal value to Serial Plotter.
     
                                         // "myBPM" hold this BPM value now. 

if (pulseSensor.sawStartOfBeat()) {            // Constantly test to see if "a beat happened". 
 
 if(counter ==20) 
 {
 if(totalAvrage<5)
 {
  if(totalpmAvrage>60)
  {
  lcd.println("s to severe");
  status="subjected to severe apenia";
  BTserial.println("subjected to severe apenia");
  BTserial.println(";");
  digitalWrite(LED13,HIGH);
 digitalWrite(LED0,LOW);
 digitalWrite(LED1,LOW); 
 for(int i=0;i<=2;i++)
    {
            tone(buzzer,2000);// Send 2KHz sound signal...
            digitalWrite (LED13 , HIGH );
            delay(500);
            noTone(buzzer);// Stop sound...
            digitalWrite (LED13 , LOW );
            delay(500);
            
            }    
  }
  else 
  {
    lcd.println("severe");
    status="has severe apenia";
  BTserial.println("has severe apenia");
  BTserial.println(";");
  
  for(int i=0;i<=2;i++)
    {
            tone(buzzer,2000);// Send 2KHz sound signal...
            digitalWrite (LED13 , HIGH );
            delay(500);
            noTone(buzzer);// Stop sound...
            digitalWrite (LED13 , LOW );
            delay(500);
            
            }    
    }
  }
  else if(totalAvrage>=5 && totalAvrage<15){
    if(totalpmAvrage>60){
    lcd.println("s moderate");
    status="subjected to be moderate";   
    BTserial.println(";");
    BTserial.println("subjected to be moderate");
    BTserial.println(";");
    digitalWrite(LED1,HIGH);
    digitalWrite(LED13,LOW);
    digitalWrite(LED0,LOW);
    }
    else{      
    lcd.print("Moderate Apnea");
    status="Moderate Apnea";
    BTserial.println("Moderate Apnea");
    BTserial.println(";");
     for(int i=0;i<=2;i++)
    {
            tone(buzzer,2000);// Send 2KHz sound signal...
            digitalWrite (LED1 , HIGH );
            delay(500);
            noTone(buzzer);// Stop sound...
            digitalWrite (LED1 , LOW );
            delay(500);
            
            }
    }
    }
    else if(totalAvrage>=15 && totalAvrage<30){
      if(totalpmAvrage>60)
      {
        lcd.println("s mild");
        status="subjected to mild";
      BTserial.println(";");
      BTserial.println("Mild Apnea");
      digitalWrite(LED0,HIGH);
      digitalWrite (LED1 , LOW );
      digitalWrite(LED13,LOW);
        }
        else {
      lcd.print("Mild Apnea");
      status="Mild Apnea";
      BTserial.println(";");
      BTserial.println("Mild Apnea");
        for(int i=0;i<=2;i++)
    {
            tone(buzzer,2000);// Send 2KHz sound signal...
            digitalWrite (LED0 , HIGH );
            delay(500);
            noTone(buzzer);// Stop sound...
            digitalWrite (LED0 , LOW );
            delay(500);    
            }
        }
      }
      else{
        status="normal";
        lcd.print("normal");
         BTserial.println("normal");
         BTserial.println(";");
        }      
  counter =0;
   totalAvrage=0;
    totalpmAvrage=0;
 }
 lcd.setCursor(0,0);
 lcd.print("rate= ");
 lcd.println(myBPM);
 lcd.setCursor(0,10);
 lcd.print("BPM");
 delay(500); 
  delay(100);
}
}

 void Flow()
{
   count++; //Every time this function is called, increment "count" by 1
}

float avrage(float total){
  float avg =0;
  avg =total /20;
  return avg;
  
  } 

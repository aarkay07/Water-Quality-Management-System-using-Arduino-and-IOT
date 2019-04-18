#include<Wire.h>
#include <SoftwareSerial.h>                           //we have to include the SoftwareSerial library, or else we can't use it.
#include <Servo.h>

Servo my_servo;
#define rx 2                                          //define what pin rx is going to be.
#define tx 3                                          //define what pin tx is going to be.
int i;
SoftwareSerial myserial(rx, tx);                      //define how the soft serial port is going to work.

String inputstring = "";                              //a string to hold incoming data from the PC
String pH;                                            //pH of water in char
int t;                                              //turbidity of water
String sensorstring = "";                             //a string to hold the data from the Atlas Scientific product
boolean input_stringcomplete = false;                 //have we received all the data from the PC
boolean sensor_stringcomplete = false;                //have we received all the data from the Atlas Scientific product
float ph; 



void setup() { 
  pinMode(8 , OUTPUT);
  Serial.begin(9600);                                 //set baud rate for the hardware serial port_0 to 9600
  my_servo.attach(10);
  myserial.begin(9600);                               //set baud rate for software serial port_3 to 9600
  inputstring.reserve(10);                            //set aside some bytes for receiving data from the PC
  sensorstring.reserve(30);                           //set aside some bytes for receiving data from Atlas Scientific product
  Wire.begin();
  // put your setup code here, to run once:
  my_servo.write(0);

}



void serialEvent() 
{                                                     //if the hardware serial port_0 receives a char
  char inchar = (char)Serial.read();                  //get the char we just received
  inputstring += inchar;                              //add it to the inputString
 
  if (inchar == '\r') 
  {                               
    input_stringcomplete = true;                      //if the incoming character is a <CR>, set the flag
  }
}


void loop() {
  // put your main code here, to run repeatedly:
  if (input_stringcomplete) {                         //if a string from the PC has been received in its entirety                     
    myserial.print(inputstring);                      //send that string to the Atlas Scientific product
    inputstring = "";                                 //clear the string
    input_stringcomplete = false;                     //reset the flag used to tell if we have received a completed string from the PC      
  }

  if (myserial.available() > 0) {                     //if we see that the Atlas Scientific product has sent a character.
    char inchar = (char)myserial.read();              //get the char we just received
    sensorstring += inchar;
    
    if (inchar == '\r') {
      sensor_stringcomplete = true;                   //if the incoming character is a <CR>, set the flag
    }
  }

  if (sensor_stringcomplete) {                        //if a string from the Atlas Scientific product has been received in its entirety
    //Serial.println(sensorstring);                     //send that string to the PC's serial monitor
    //pH = sensorstring.substring(4,9);
    //Serial.println(sensorstring.substring(4,9).toFloat()   );
    //Serial.println(pH);

    Wire.beginTransmission(9);
    Wire.write(sensorstring.substring(4,9).c_str());
    Serial.println(sensorstring.substring(4,9).c_str());
    Wire.endTransmission();  
    
    //delay(1000);
    //Serial.println(analogRead(A0));
    //ph = pH.toFloat();                      //convert the string to a floating point number so it can be evaluated by the Arduino
    //Serial.println(ph);
    
    if(sensorstring.substring(4,9).toFloat()  == 0)     //< 7.3)
    {
      my_servo.write(0);
      digitalWrite(8, LOW );
    }
    else
    {
      my_servo.write(180);
      digitalWrite(8, HIGH);
    }
    sensorstring = "";                                //clear the string:
    sensor_stringcomplete = false;                    //reset the flag used to tell if we have received a completed string from the Atlas Scientific product
  }
}

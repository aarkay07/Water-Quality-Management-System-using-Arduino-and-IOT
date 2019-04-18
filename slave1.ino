#include<SoftwareSerial.h> //to interface esp8266 with arduino through serial communication
#include<Wire.h>

SoftwareSerial esp8266(3,4);//set the software serial pins RX=3,TX=4
#define SID"aarkay"
#define PASS"isisisis"

void sendAT(String command,const int timeout){
  String response="";
  esp8266.print(command);
  long int time=millis();
  while((time+timeout)>millis()){
    while(esp8266.available()){
      char c = esp8266.read();
      response +=c;
    }
  }

}

void connectwifi(){
  delay(1000);
  sendAT("AT\r\n",2000);
  sendAT("AT+CWMODE=1\r\n",2000);//call sendAT function to set esp8266 to station mode
  sendAT("AT+CWJAP=\""SID"\",\""PASS"\"\r\n",2000);//AT command to connect with wifi network
  while(!esp8266.find("OK")){
    //wait for connection
  }
  sendAT("AT+CIFSR\r\n",2000);//AT command to print IP address on serial monitor
  sendAT("AT+CIPMUX=0\r\n",2000);//AT command to set esp8266 to single connection
}
 

void setup() {
  esp8266.begin(115200);//begin the software serial communication with baud rate 115200
  sendAT("AT+RST\r\n",2000);//call sendAT function to send reset AT command
  connectwifi();
  Serial.print("Connected");
  Wire.begin(9); 
  Wire.onReceive(receiveEvent);
  Serial.begin(9600);  
}


String final;
String pH;
//String ph;
void receiveEvent(int howMany) {
  //Serial.println("Test");
  while (Wire.available()) { // loop through all but the last
    char c = Wire.read(); // receive byte as a character
    //Serial.print("C:");
    //Serial.println(c);
    final = final+c;
    
  }
  pH = final;
  //delay(1000);
  //Serial.println("before");
  //delay(1000);
  //updateThinkspeak(final);//call the function to update Thingspeak channel
  //Serial.println("after");
  final = "";
}

void loop() {
  //int converted to string so that values can be passed to cloud
 /*
  String Temperature=String(0);//convert integer to string data type

  while (Wire.available()) { // loop through all but the last
    char c = Wire.read(); // receive byte as a character
    //Serial.print("C:");
    //Serial.println(c);
    final = final+c;
    //Serial.println(final);
  }
  pH = final.toFloat();
  ph = final;*/
  //int turb = analogRead(A0);
  Serial.println(pH);
  //Serial.println(turb);
//  String t = String(turb);
 updateThinkspeak(pH);//call the function to update Thingspeak channel
  delay(100);
  
}

void updateThinkspeak(String aa){
   Serial.println(aa);
   sendAT("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80\r\n",2000);//tcp connection with domain name and port no.
   //delay(2000);
   String cmdlen;
   String cmd = "GET /update?api_key=X9833M52Y0AVPJII&field1="+aa+"\r\n";//update the temperature,humidity and dewpoint values to thingspeak channel
   cmdlen=cmd.length();
   sendAT("AT+CIPSEND="+cmdlen+"\r\n",2000);
   esp8266.print(cmd);
   sendAT("AT+CIPCLOSE\r\n",2000);
   //delay(1000);
    
}

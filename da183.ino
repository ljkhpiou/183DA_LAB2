#include <ESP8266WiFi.h>
#include<Servo.h>

WiFiServer server(80); //Initialize the server on Port 80
Servo servoR;//actually the servo motor
Servo servo2;//actually the servo-R motor
int pos=0;
const int stand=1400; //when Servo2 is not moving, getting this number by testing. 
void setup() {
WiFi.mode(WIFI_AP); //Our ESP8266-12E is an AccessPoint
WiFi.softAP("ljkhpiou", "12345678"); // Provide the (SSID, password); .
server.begin(); // Start the HTTP Server
//Looking under the hood
Serial.begin(115200);
IPAddress HTTPS_ServerIP= WiFi.softAPIP(); // Obtain the IP of the Server
Serial.print("Server IP is: "); // Print the IP to the monitor window
Serial.println(HTTPS_ServerIP);
pinMode(LED_PIN, OUTPUT); //GPIO16 is an OUTPUT pin;
digitalWrite(LED_PIN, LOW); //Initial state is ON
servoR.attach(2);
servo2.attach(4);

}

void hit(int n){
// hit the object at angle n.
servo2.writeMicroseconds(stand);
  int p=servoR.read();
  //make the moving to desired angular position smoothly.
  //by increment 1 degree a time. 
  while(p<=n){
    servo2.writeMicroseconds(stand);//make servo-R motor not moving. 
    servoR.write(p);
    servo2.writeMicroseconds(stand);
    delay(10);
    p++;
  }
  while(p>n){
    servo2.writeMicroseconds(stand);
    servoR.write(p);
    servo2.writeMicroseconds(stand);
    delay(10);
    p--;
  }
  delay(50);
  servo2.write(80);//hit 
  delay(66);
  servo2.write(100);//return back to original position. 
  delay(35);
  servo2.writeMicroseconds(stand);
}
void loop(){
WiFiClient client = server.available();
if (!client) { //high pitch at 0 degree, low pitch at 60 degree, mid pitch at 30 degree. 
  hit(0);//auto play from high pitch to low pitch and repeat. 
  hit(30);
  hit(60);
  hit(60);
  hit(30);
  hit(0);
return; 
} 
Serial.println("Somebody has connected :)");
//Read what the browser has sent into a String class and print the request to the monitor
String request = client.readStringUntil('\r');
//Looking under the hood 
Serial.println(request);
// Handle the Request

if (request.indexOf("/HIGH") != -1){ 
hit(0); }//high pitch at angle 0
else if (request.indexOf("/MID") != -1){ 
hit(30); //mid ptch at anlgle 30
}
else if(request.indexOf("/LOW") != -1){
  hit(60);//low pitch at angle 60
}
String s = "HTTP/1.1 200 OK\r\n";
s += "Content-Type: text/html\r\n\r\n";
s += "<!DOCTYPE HTML>\r\n<html>\r\n";
s += "<br><input type=\"button\" name=\"b1\" value=\"play HIGH\"";
s += " onclick=\"location.href=’192.168.4.1/HIGH’;\">";
s += "<br><br><br>";
s += "<br><input type=\"button\" name=\"b1\" value=\"play MID\"";
s += " onclick=\"location.href=’192.168.4.1/MID’\">";
s += "<br><br><br>";
s += "<br><input type=\"button\" name=\"b1\" value=\"play LOW\"";
s += " onclick=\"location.href=’192.168.4.1/LOW’\">";
s += "</html>\n";

//Serve the HTML document to the browser.
client.flush(); //clear previous info in the stream
client.print(s); // Send the response to the client
delay(1);
Serial.println("Client disonnected"); //Looking under the hood


}


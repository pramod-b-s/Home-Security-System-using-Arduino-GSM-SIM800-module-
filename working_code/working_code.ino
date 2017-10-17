#include<SoftwareSerial.h>

SoftwareSerial mySerial(9, 10);

const int trigPin1 = 6;
const int echoPin1 = 7;
const int trigPin = 11;
const int echoPin = 12;
const int redLedPin=4;
const int greenLedPin=13;

char inchar;

// defines variables
long duration,duration1;
int distance,distance1;
int sms_count=0, flag=0, activate=0,flag1=0,flag2=0,flag3=0,f=0;

unsigned long timevar;

void setup()
{
  sms_count=0;
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  pinMode(trigPin1, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin1, INPUT); // Sets the echoPin as an Input
  pinMode(redLedPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);
  mySerial.begin(9600);
  Serial.begin(9600);
}

void loop()
{
  if(!activate){
    //start
    if(f==0){
      delay(1000);
      RecieveMessage();
      f=1;
    }
  if(flag1 && flag2 && flag3){
    Serial.write("activated");
    activate=1;
    SendTextMessageInit();
  }

  if (mySerial.available()>0){
  inchar=mySerial.read();
    Serial.write(inchar);
    if(inchar=='#'){
      flag1=1;
    }
    if(inchar=='$'){
      flag2=1;
    }
    if(inchar=='*'){
      flag3=1;
    }
 }
  //end
  }

  if(activate){
// Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
// Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(50);
  digitalWrite(trigPin, LOW);
// Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  duration1 = pulseIn(echoPin, HIGH);
// Calculating the distance
  distance= duration*0.034/2;
  distance1= duration*0.034/2;
// Prints the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.println(distance);
  Serial.print("Distance1: ");
  Serial.println(distance1);

  if(flag==1){
    if(millis()>timevar+900000){
      sms_count=0;
      flag=0;  
    }
  }
  else if(sms_count==3){
    timevar = millis();
    flag=1;
  }
  else if(sms_count<3 && distance<20){
    if(sms_count==0){
      Serial.println("SMS sent !!");
      redLedOn();
      greenLedOff();
      SendTextMessage();
    }
    else{
      Serial.println("SMS sent !!");
      redLedOn();
      greenLedOff();
      SendTextMessage();
    }
  }
  else{
    redLedOff();
    greenLedOn();
    //Serial.println("Two SMS already sent !!");
  }
  }
}

void SendTextMessage()
{
  mySerial.println("AT+CMGF=1");    //To send SMS in Text Mode
  delay(1000);
  mySerial.println("AT+CMGS=\"+91xxxxxxxxxx\"\r"); // change to the phone number you using 
  delay(1000);
  mySerial.println("ALERT !!! Some intruder has entered your house. If it's not you, kindly call 100.");//the content of the message
  delay(200);
  mySerial.println((char)26);//the stopping character
  delay(1000);
  sms_count++;
}

void SendTextMessageInit()
{
  mySerial.println("AT+CMGF=1");    //To send SMS in Text Mode
  delay(1000);
  mySerial.println("AT+CMGS=\"+91xxxxxxxxxx\"\r"); // change to the phone number you using 
  delay(1000);
  mySerial.println("Burglar alarm system activated !!");//the content of the message
  delay(200);
  mySerial.println((char)26);//the stopping character
  delay(1000);
}

void RecieveMessage()
{
  mySerial.println("AT+CMGF=1");    //To send SMS in Text Mode
  delay(1000);
  mySerial.println("AT+CNMI=2,2,0,0,0"); // AT Command to receive a live SMS
  delay(1000);
}

void redLedOn(){
  digitalWrite(redLedPin,HIGH);  
}

void redLedOff(){
  digitalWrite(redLedPin,LOW);  
}

void greenLedOn(){
  digitalWrite(greenLedPin,HIGH);  
}

void greenLedOff(){
  digitalWrite(greenLedPin,LOW);  
}

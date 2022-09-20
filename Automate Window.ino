#define BLYNK_PRINT Serial
#define SENSOR_PIN D6
#define TIMER_PIN D2 //GPIO 16
int ledPin = D3; 
int SENSOR_VALUE = 0;
int flag = 0;
int val;

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Servo.h>


//Panjang Pin Digital, Pendek Pin GND
//Konfigurasi LED
pinMode(ledPin, OUTPUT);
digitalWrite(ledPin, LOW);

Servo servo;
WidgetLCD lcd(V1);
BlynkTimer timer;


char auth[] = "s";
char ssid[] = "polije";
char pass[] = "polijebanget";

void setup()
{
  //Konfigurasi Wifi
  Serial.println( ssid );
  WiFi.begin(ssid,password);
  
  Serial.println();
  Serial.print("Mencoba terhubung ke jaringan WiFi");
  while( WiFi.status() != WL_CONNECTED ){
      delay(500);
      Serial.print(".");        
  }
  Serial.println();
  Serial.println("Wifi berhasil terkoneksi!");
  digitalWrite( ledPin , HIGH);
  Serial.print("Wemos D1 IP Address : ");
  Serial.println(WiFi.localIP() );
  ///////////////////////////////////
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  servo.attach(D5); // NodeMCU D8 pin
  pinMode(SENSOR_PIN, INPUT);
  timer.setInterval(1000L,basah); 
  timer.setInterval(1000L,kering); 
 }

void loop()
{
  Blynk.run();
  timer.run();

void basah(){
  val = digitalRead(SENSOR_PIN);
  if (val==0 && flag==0) {
    servo.write(180);
    Serial.println("Diluar hujan!Jendela ditutup!");
    Blynk.notify("Diluar hujan!Jendela ditutup!");
    lcd.print(0,0, "Jendela tertutup");
    lcd.print(0,1, "Sekarang hujan ");
    flag=1;
  }
}

void kering(){
  int val = digitalRead(SENSOR_PIN);
  if (val==1 && flag==1) {
    servo.write(0);
    Serial.println("Diluar cerah!Jendela dibuka!");
    Blynk.notify("Diluar cerah!Jendela dibuka!");
    lcd.print(0,0, "Jendela terbuka ");
    lcd.print(0,1, "Sekarang kering");
    flag=0;
  }
}

BLYNK_WRITE(V3)
{
  servo.write(param.asInt());
  lcd.clear();
  lcd.print(0,0, "Jendela tertutup");
}
BLYNK_WRITE(V4)
{
  servo.write(param.asInt());
  lcd.clear();
  lcd.print(0,0, "Jendela tebuka ");
}

//Timer
BLYNK_WRITE(V5)
{
  servo.write(param.asInt());

  if(param.asInt()==180){
    lcd.clear();
    lcd.print(0,0, "Jendela tertutup");
    }
  else if(param.asInt()==0){
    lcd.clear();
    lcd.print(0,0, "Jendela terbuka ");
    }
}
BLYNK_WRITE(V6)
{
  val = digitalRead(SENSOR_PIN);
  if (val==0) {
      lcd.print(0,1, "Sekarang hujan ");
    }
  else if(val==1){
    lcd.print(0,1, "Sekarang kering");
    }
}
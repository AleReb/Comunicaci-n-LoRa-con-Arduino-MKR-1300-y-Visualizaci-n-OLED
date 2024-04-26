#include <MKRWAN_v2.h>
LoRaModem modem;

String appEui = "3000000000000060";
String appKey = "7466CCC6064E3B6EA0FB9BFF654E9585";
int contRetry = 0;

/////SD
#include <SPI.h>
#include <SD.h>
const int chipSelect = 7;  

////////////////// variables sD
File uploadFile;
String mensajeSD ="Temperatrua,Humedad,Veces enviado,Conectado";
char filename[16]; //para construir el archivo csv base cada vez que se prenda y guarde
bool firstrun = true;
bool Conectado = true;
#include <U8g2lib.h>
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
///bme 
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#define SEALEVELPRESSURE_HPA (1016.25)
Adafruit_BME280 bme; // I2C 

float tempInt ;
float humInt;
float pressInt;
float altitud;
int err;
int contador = 0; 
void setup() {
 Serial.begin(115200);
 u8g2.begin();
  delay(5000); 
 Serial.println("Prueba arduino wan lora TTN SD");
  bool status;
 // default settings
   status = bme.begin(0x76);    //76
   if (!status) {
   Serial.println("Could not find a valid BME280 sensor, check wiring!");
   delay(1000);
   }else{
   Serial.println("valid BME280 sensor, OK");
  delay(1000);
    } 
    //     while (!Serial);
 u8g2.clearBuffer();          // clear the internal memory
 u8g2.setFont( u8g2_font_crox4hb_tr);  // choose a suitable font grande
 u8g2.drawStr(0, 18, "TEST lora");  // write something to the internal memory
 Serial.println("Conectando...");
 u8g2.sendBuffer();    
 if (!modem.begin(AU915)) {
 Serial.println("Failed to start module");
 u8g2.drawStr(0, 50, "FAIL MODEM");  // write something to the internal memory
 u8g2.sendBuffer();    
 delay(1000);
  };
  
  Serial.println("Conectando...");
  u8g2.drawStr(0, 33, "conectando...");  // write something to the internal memory
  u8g2.sendBuffer(); 
  Serial.print("Your module version is: ");
  Serial.println(modem.version());
  Serial.print("Your device EUI is: ");
  Serial.println(modem.deviceEUI());
 int connected = modem.joinOTAA(appEui, appKey);
 
 while(!connected){
 Serial.println("Retry...");
 u8g2.clearBuffer();          // clear the internal memory
 u8g2.drawStr(0, 18, "TEST lora");  // write something to the internal memory
 u8g2.drawStr(0,46, "retry: ");  // write something to the internal memory
 u8g2.setCursor(60, 47);
 u8g2.print(contRetry);  
 u8g2.sendBuffer(); 
 if(!modem.joinOTAA(appEui, appKey)){
 Serial.print("Fail counter: ");
 contRetry = contRetry+1;
 u8g2.drawStr(0,64, "FAIL!!");  // write something to the internal memory
 u8g2.sendBuffer(); 
 Serial.println(contRetry);
 u8g2.sendBuffer();
  if(contRetry > 15){
   Conectado = false;
   break;
     }
    }
 else{
 Conectado = true;
 break;
    }
  };
  
  // Set poll interval to 60 secs.
  modem.minPollInterval(60);
  // NOTE: independently by this setting the modem will
  // not allow to send more than one message every 2 minutes,
  // this is enforced by firmware and can not be changed.

  Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    delay(1000);
  }else{  
  int n = 0;
  snprintf(filename, sizeof(filename), "data%03d.csv", n); // includes a three-digit sequence number in the file name
  
  while(SD.exists(filename)) {
    n++;
    snprintf(filename, sizeof(filename), "data%03d.csv", n);
  }
  File dataFile = SD.open(filename,FILE_READ);
  Serial.println(n);
  Serial.println(filename);
  dataFile.close();
    
  Serial.println("card initialized.");
  }
}

void loop() {

if (Serial.available() > 0){
  //This line is clearing the serial buffer
  String msg = Serial.readStringUntil('\r');


 Serial.println();
  Serial.print("Sending: " + msg + " - ");
  for (unsigned int i = 0; i < msg.length(); i++) {
    Serial.print(msg[i] >> 4, HEX);
    Serial.print(msg[i] & 0xF, HEX);
    Serial.print(" ");
  }
  Serial.println();
 Serial.println();

 modem.setPort(1);
 modem.beginPacket();
 modem.print(msg);
 err = modem.endPacket(true);
 Serial.print("Message state: ");
 Serial.println(err);

 switch (err) {
     case -1:
         Serial.println("Timeout");
         break;

     case 1:
         Serial.println("Message sent !");
         break;
     default:
         Serial.print("Error while sending message with code: ");
         Serial.println(err);
         break;
}

 Serial.println();
}
   tempInt   = bme.readTemperature();
   humInt    = bme.readHumidity();
   pressInt  = bme.readPressure() / 100.0F;
   altitud   = bme.readAltitude(SEALEVELPRESSURE_HPA);
   u8g2.clearBuffer();          // clear the internal memory
   u8g2.setCursor(0, 14);
   u8g2.print("TEMP/HUM");
   u8g2.setCursor(0, 30);
   u8g2.print(String(bme.readTemperature()) + " \xb0""C");
   u8g2.setCursor(0, 46);
   u8g2.print(String(bme.readHumidity()) + " %H ");
   u8g2.setCursor(0, 64);
   u8g2.print(String(contador) + " enV");
   u8g2.sendBuffer();
   static unsigned long Tmen=millis();       //every 30 seconds send men
    if (millis() - Tmen >= 30000) 
    {
      Tmen=millis();
 String mensaje = "T"+String(bme.readTemperature())+",H"+String(bme.readHumidity())+",C"+String(contador);
 Serial.println(mensaje);
 contador = contador+1;
 modem.setPort(1);
 modem.beginPacket();
 modem.print(mensaje);
 err = modem.endPacket(true);
 Serial.print("Message state: ");
 Serial.println(err);

 switch (err) {
     case -1:
         Serial.println("Timeout");
         break;

     case 1:
         Serial.println("Message sent !");
         break;
     default:
         Serial.print("Error while sending message with code: ");
         Serial.println(err);
         break;
}

 Serial.println();
if (firstrun ==true){ 
}else{
mensajeSD =String(bme.readTemperature())+","+String(bme.readHumidity())+","+String(contador)+","+String(Conectado);
}
// open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open(filename, FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(mensajeSD);
    dataFile.close();
    // print to the serial port too:
    Serial.println(mensajeSD);
    firstrun = false; 
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  } 
}
} 

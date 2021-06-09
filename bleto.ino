
#include <SoftwareSerial.h> //Librería que permite establecer comunicación serie en otros pins
#include <Wire.h>
#include <SHT2x.h>
#include <Servo.h>
#include <Stream.h>
//Aquí conectamos los pins RXD,TDX del módulo Bluetooth.
SoftwareSerial BT(10,11); //10 RX, 11 TX.
const int rele1 = 2 ;
char var;
unsigned long tiempo1 = 0;
unsigned long tiempo2 = 0;
unsigned long tiempo3 = 0;
unsigned long tiempo4 = 0;
unsigned long t2 = 0;
unsigned long tvolt=10800000; //t de volteo en ms
unsigned long tvoltr=tvolt; //respaldo t de volteo en ms
float cal=0; //calibracion de temp
float tempinf=27.7; //temp inf
float tempinfr=tempinf; //respaldo temp inf
int cont1=1;
int si=1;
//float tempsup=38; temp superior en caso de tener otro ventilador
Servo Servo_1;  
int Angulo = 0;
int c=1;//contador
void setup()
{
 BT.begin(9600); //Velocidad del puerto del módulo Bluetooth
 Serial.begin(9600); //Abrimos la comunicación serie con el PC y establecemos velocidad
 pinMode(rele1,OUTPUT);
 tiempo1 = millis();
 Wire.begin();
 Servo_1.attach(5);
 
}

void loop()
{
inicio();

digitalWrite(rele1,HIGH);
if((SHT2x.GetTemperature()+cal)<tempinf){
  if(c==1){
    tiempo3 = millis();
    }
  tiempo4=millis();
  c=2;
  if(tiempo4 > (tiempo3+2000)){  //Si ha pasado 2 segundo ejecuta la comprobacion de la temperatura
  while((SHT2x.GetTemperature()+cal)<tempinf){
    if(BT.available()>0){
      if(cont1==1){
    BT.print("\nHumedad: ");
    BT.println(SHT2x.GetHumidity());
    BT.print("Temperatura(C): ");
    BT.println(SHT2x.GetTemperature());
    BT.println();
    cont1=2;
        }
      }
    digitalWrite(rele1,LOW);
    }
  tiempo3 = millis();
  }
}
cont1=1;
if((SHT2x.GetTemperature()+cal)>tempinf){
  c=1;
}
tiempo2 = millis();
t2=tiempo1+tvolt;
if(tiempo2 > t2){  //Si ha pasado 1 segundo ejecuta el IF
  tiempo1 = millis();
  control2();
  }
}

void control(int val) {
switch (val) {
  case 1:
    BT.print("Humedad: ");
    BT.println(SHT2x.GetHumidity());
    BT.print("Temperatura(C): ");
    BT.println(SHT2x.GetTemperature());
    BT.print("Tiempo de volteo: ");
    BT.print(tvolt/1000);
    BT.print(" s\n");
    BT.print("Tiempo transcurrido: ");
    BT.print(tiempo2/1000);
    BT.print(" s\n");
    si=1;
    break;
  case 2:
   BT.print("Ingrese el tiempo de volteo en s: \n(tienes 8 segundos)\n");
   delay(8000);                              // Esperar 
   tvoltr=tvolt;
   tvolt=BT.parseInt()*1000;
   if (tvolt<25000){
     tvolt=tvoltr;
    }
   BT.println("\nTiempo configurado en: ");
   BT.print(tvolt/1000);
   BT.print(" segundos\n");
   si=1;
   break;
   case 3:
   BT.print("Ingrese la temperatura base \n(tienes 8 segundos)\n");
   delay(8000);                              // Esperar 
   tempinfr=tempinf;
   tempinf=BT.parseFloat();
   if (tempinf<15){
     tempinf=tempinfr;
    }
   BT.println("\nTemperatura base configurado en: ");
   BT.print(tempinf);
   BT.print(" C\n");
   break;
  default:
   break;
  }
  }
void control2() {
  for(Angulo= 90;Angulo < 180; Angulo += 1)  //Incrementar de 1 en 1 el angulo que va de 0° hasta 180°
  {                                  
    Servo_1.write(Angulo);                  // Decirle al Servo_1 que se mueva los grados que estan en la variable angulo
    delay(45);                              // Esperar 15 ms para que el servo alcance la posicion
  } 
  delay(500);                                //Esperar 500 ms 
  for(Angulo = 180; Angulo>0; Angulo-=1)    // Decrementar de 1 en 1 el angulo que va de 180° hasta 0°
  {                                
    Servo_1.write(Angulo);                   // Decirle al Servo_1 que se mueva los grados que estan en la variable angulo
    delay(45);                              // Esperar 15 ms para que el servo alcance la posicion
  } 
  delay(500);                        //Esperar 500 ms 
    for(Angulo = 0; Angulo<90; Angulo+=1)    // Decrementar de 1 en 1 el angulo que va de 180° hasta 0°
  {                                
    Servo_1.write(Angulo);                   // Decirle al Servo_1 que se mueva los grados que estan en la variable angulo
    delay(45);                              // Esperar 15 ms para que el servo alcance la posicion
  } 
  delay(500);                        //Esperar 500 ms 
}

void inicio(){
  if(BT.available()>0){
  if(si==1){
    BT.println("\n\nDesea ver la temperatura y humedad: 1");
    BT.println("Desea ajustar tiempo de volteo: 2");
    BT.println("Desea ajustar la temperatura limite: 3");
  }
  var=BT.parseInt();
  si=5;
  control(var);
}
  }

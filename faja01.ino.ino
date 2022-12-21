/*PROYECTO FAJA SENSADORA DE COLOR*/

/*PROYECTO FAJA SENSADORA DE COLOR*/

#include <RBDdimmer.h>                    // Liberia Dimmer
#include <DallasTemperature.h>            // Liberia Convertidor sensor
#include <OneWire.h>                      // Liberia Sensor 
//#include <LiquidCrystal.h>              //Libreria LCD
#include <Wire.h>                         //Libraria Sensor
#include <LiquidCrystal_I2C.h>            //Libreria I2C ALCD
#include <ESP32Servo.h>

//LiquidCrystal lcd(22,23,5,18,19,21);  // Define conexion(rs,enable,d4,d5,d6,d7)

bool Salir = false;             //variable tipo booleana
int segundos = 0;
int Hornosegundos = 10;       //segundos que debe mantenerse en el horno

LiquidCrystal_I2C lcd(0x27,16,2);

hw_timer_t *My_timer = NULL;      //interrumcion timer
void IRAM_ATTR onTimer()    //Se llama a esta funcion cada vez que se produce una interrupcion en el timer
{
 segundos++;    // se incrementa la variable en 1, como el intervalo esta configurado cada segundo, entonces esta variable es un segundero
}

//DIMMER
const int zeroCrossPin  = 25;    //Asignar pin GPIO 25 al paso por cero del dimmer
const int acdPin  = 26;          //Asignar pin GPIO 26 a potencia del foco
const int oneWireBus = 5;            // Puerto GPIO 4 Sensor 

//TSC3200 Definnir pines de funcionamiento
#define S0 33    
#define S1 27    
#define S2 14    
#define S3 12    
#define OU 4  

//Decalrar variables usadas por el sensor
int rojo;
int verde;
int azul;
int Settemp = 17; //empezar en 17 para que pueda correr el programa al inicio si no el valor es cero y no funciona
String ecolor;  

//Variables
int PinHornoSen = 0;
int Pinvent=13;  
int Motor=2; 
int sm1=15;
int sm2=18;                
float temp;                           
float potencia = 17;  //17 se usa como CERO en el dimmer

//Objetos

dimmerLamp acd(acdPin,zeroCrossPin); 
OneWire oneWire(oneWireBus);
DallasTemperature sensors (&oneWire);
Servo servo1;

//Boleanos
bool faja = false;
//bool servo1 = false;
//bool servo2 = false;
bool foco = false;
bool ValMax = false;
bool pl= false;
bool vent= false;
bool Senhorno = false;
bool Senfoco = false;
bool timerON = false;

// Published values for SG90 servos; adjust if needed
int minUs = 500;
int maxUs = 2400;

//servo
int servo1Pin = 23;
int servopin1  = 15;
int servopin2  = 18;
int pos = 0;      // position in degrees

//Servo myservo;
int angle = 0;  

int frequency = 0;

void setup() 
{
  //Iniciar comunicacion Serial - Configuraciones iniciales y parametros del LCD, sensor y dimmer
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();
  sensors.begin();
  acd.begin(NORMAL_MODE, ON);       
  pinMode (Pinvent,OUTPUT);        //Asignamos GPIO 13 como salida para el ventilador
  
  digitalWrite(Pinvent, LOW);  

  pinMode(Motor,OUTPUT);

  pinMode(S0, OUTPUT);    // Pin  como salida
  pinMode(S1, OUTPUT);    // Pin  como salida
  pinMode(S2, OUTPUT);    // Pin  como salida
  pinMode(S3, OUTPUT);    // Pin  como salida
  pinMode(OU, INPUT);     // Pin  como salida
  
  digitalWrite(S0,HIGH);  // Establece frecuencia de salida
  digitalWrite(S1,HIGH);  // Del modulo al 100 por ciento*

  pinMode(PinHornoSen,INPUT);

/******* Configuracion de Timer *************/
  My_timer = timerBegin(3, 80, true);               //Se inicia el timer 03 porque no hace conflicto con los demas
  timerAttachInterrupt(My_timer, &onTimer, true);   //A donde debe dirigirse cuando el timer haga la interrupcion
  timerAlarmWrite(My_timer, 1000000, true);         //Se configura una interrupcion cada segundo
  timerAlarmEnable(My_timer);                       //Arrancar el Timer
  /****************************************/      

  servo1.attach(servo1Pin, minUs, maxUs);
  pinMode (servopin1 ,OUTPUT);   
  pinMode (servopin2 ,OUTPUT);   

  /**** Inicio de los servos ****/
  digitalWrite(servopin1, HIGH);
  digitalWrite(servopin2, LOW);
  delay(500);
  servo1.write(10);
  delay(1500);
  servo1.write(90);
  delay(1500);
  digitalWrite(servopin1, LOW);
  digitalWrite(servopin2, HIGH);
  servo1.write(10);
  delay(1500);
  servo1.write(90);
  delay(1500);
  digitalWrite(servopin1, LOW);
  digitalWrite(servopin2, LOW);

}

void loop() 
//*BLOQUE DE PROCESOS CONDICIONALES DEL PROYECTO
//==================================================
{
  //Parametros iniciales de operacion
  faja = true; 
  //servo1 = false; servo2 = false;
  ValMax = false; pl= false; vent= false;
  Senhorno = false; Senfoco = false; Salir = false; timerON = false;
  potencia=17;
  digitalWrite(Motor,LOW);
  acd.setPower(potencia);
  Settemp = 17;
/*******************el siguiente loop es para probar cada una de las partes independientemente *****/
// while (1)
// {
//    acd.setPower(18);
//    faja= false; digitalWrite(Motor,HIGH);
    // digitalWrite(servopin1, HIGH);
    //  digitalWrite(servopin2, HIGH);

    // servo1.write(0);
    // delay(2000);
    // servo1.write(90);
    // delay(2000);
//   imprimir();
//   Serial.print(Salir);
//   Serial.print("  ");
//   Serial.println(segundos);

 //Settemp = lectcol();
 //Serial.println(Settemp);

/****** presenta los valores que lee el sensor descomentar solo cuando se requiera hacer pruebas**************/
/**
digitalWrite(S2,LOW);
digitalWrite(S3,LOW);
frequency = pulseIn(OU, LOW);
Serial.print("R= ");//printing name
Serial.print(frequency);//printing RED color frequency
Serial.print(" ");
delay(100);
digitalWrite(S2,HIGH);
digitalWrite(S3,HIGH);
frequency = pulseIn(OU, LOW);
Serial.print("G= ");//printing name
Serial.print(frequency);//printing RED color frequency
Serial.print(" ");
delay(100);
digitalWrite(S2,LOW);
digitalWrite(S3,HIGH);
frequency = pulseIn(OU, LOW);
Serial.print("B= ");//printing name
Serial.print(frequency);//printing RED color frequency
Serial.println(" ");
delay(100);
**/
/**************/
//  temp = LeerTemperatura();
//  digitalWrite(Pinvent,HIGH);
//  imprimir();
//  delay(250);
//}
/****  fin de loop *****************/

  while (Settemp  == 17)
  {
    Settemp = lectcol();    //Se guarda el valor correspondiente a cada color
    temp = LeerTemperatura();
    // Aqui mostrar LCD y Serial
    imprimir();
  }

  faja= false; 
  digitalWrite(Motor,HIGH);  //con high detiene la faja

  while (!Salir)
  {         
      temp = LeerTemperatura();    //Se guarda e valor del sensor de temperatura
      potencia = (1-temp/Settemp)*700; //900
      //Estableciendo limites aduados para el funcionamiento del modulo dimmer
      if (potencia > 93) 
      {
        potencia = 93;
      }
      else 
      { 
        if (potencia < 65 ) 
        {
          potencia = potencia + 35 ;  //30
        }
        if (potencia < 28) 
        {
          potencia = 17;
        }
      }  

      //Impresion Serial
      Serial.print("R:");   
      Serial.print(rojo);   
      Serial.print("\t");   

      Serial.print("V:");   
      Serial.print(verde); 
      Serial.print("\t");     

      Serial.print("A:");   
      Serial.print(azul);   
      Serial.print("\t");     

      Serial.print(temp);
      Serial.print(" C   ");
      Serial.print(Settemp);
      Serial.print("  ");
      Serial.println(potencia);
      imprimir();
      if ((temp<Settemp) && !ValMax)
      {
        foco=true;
        acd.setPower(potencia);
        pl=true;
        digitalWrite(Pinvent,LOW);
      } else 
      {
        if ((temp>Settemp) && !ValMax && !pl)
        {
          foco=false;
          potencia =17;
          acd.setPower(potencia);
          vent=true;
          digitalWrite(Pinvent,HIGH);
        } else 
        {
          foco=false;
          ValMax=true;
          acd.setPower(potencia);

          if (!faja)
          {
            digitalWrite(Motor,LOW);
            faja = true;
          }
          
          if (temp> Settemp)
          {
            vent = true;
            potencia =17;
            digitalWrite(Pinvent,HIGH);
          }else
          {
            vent = false;
            digitalWrite(Pinvent,LOW);
          }

          //if (Senhorno)
          if (!digitalRead(PinHornoSen))  //¿ya llego al sensor del honro?
          {
            digitalWrite(Motor,HIGH);
            if (!timerON)
            {
              timerON = true;
              segundos = 0;  //Reiniciar el contador de segundos
            }else
            {
              if (segundos == Hornosegundos)  //comparar si ya se cumplieron los segundos solicitados
              {
                Salir = true; // y salir de Bucle porque se termino el proceso de horno
              }
            }
          }
        }
	    }
  }   // Fin del loop de la bandera [Salir]

digitalWrite(Motor,LOW);
lcd.clear();  
lcd.setCursor(0, 0);
lcd.print("Seleccionando");
lcd.setCursor(0, 1);
lcd.print("Caja ");

switch (Settemp)
{
  case 30:
    lcd.print("Roja");
    digitalWrite(servopin1, HIGH);
    digitalWrite(servopin2, LOW);
    delay(500);
    servo1.write(10);
    delay(6500);
    servo1.write(90);
    delay(2000);
  break;
  case 40:
    lcd.print("Verde");
    digitalWrite(servopin1, LOW);
    digitalWrite(servopin2, HIGH);
    delay(500);
    servo1.write(10);
    delay(8500);
    servo1.write(90);
    delay(2000);
  break;
  case 50:
    lcd.print("Azul");
    delay(10500);
  break;
}
  
  digitalWrite(servopin1, LOW);
  digitalWrite(servopin2, LOW);
}

//==================================================
  float LeerTemperatura()       //Subrutina  Leer temperatura
  {
    sensors.requestTemperatures();
    float temperatureC = sensors.getTempCByIndex(0);
    return temperatureC;
  }

  void imprimir()      //Subrutina imprimir LCD
{
    lcd.clear();  
    lcd.setCursor(0, 0);          // Posionamos el cursor en 0,0
    lcd.print("Color : ");        // Imprime "Color : "
    lcd.print(Settemp);            // Imprime "Rojo"
    lcd.setCursor(0, 1);          // Posionamos el cursor en 0,1
    lcd.print("T: ");
    lcd.print(temp);
    lcd.print(" C  ");
    if (timerON)
    {
      lcd.print(segundos);
      lcd.print("s");
    }else
    {
      lcd.print("00s");
    }
}





  int lectcol()     //Subrutina Leer color
  {
    digitalWrite(S2,LOW);     // Establece fotodiodos
    digitalWrite(S3,LOW);     // Con filtro rojo
    delay(20); 
    rojo = pulseIn(OU, LOW); // Obtiene duracion de pulso de salida del sensor
    delay(20);       // Demora de 200 mseg
    
    digitalWrite(S2,HIGH);    // Establece fotodiodos
    digitalWrite(S3,HIGH);    // Con filtro verde
    delay(20); 
    verde = pulseIn(OU, LOW);  // Obtiene duracion de pulso de salida del sensor
    delay(20);       // Demora de 200 mseg
    
    digitalWrite(S2,LOW);     // Establece fotodiodos
    digitalWrite(S3,HIGH);    // Con filtro azul
    delay(20); 
    azul = pulseIn(OU, LOW); // Obtiene duracion de pulso de salida del sensor
    delay(20);               // Demora de 200 mseg
    
      //Rojo
  //  if ( rojo< verde && verde> azul && rojo>102 && rojo<152 && verde>210 && verde<260 && azul>150 && azul<210){   // si valores dentro del rango
      //if ( 56<rojo<76 && 155<verde<175 && 103<azul<123){   // si valores dentro del rango
    if (rojo > 70 && rojo < 100 ){ 
      Serial.println("Rojo");       // Muestra texto en serial monitor
      return 30;   
    } 
    
      //Verde  
  //  if ( rojo>110 && rojo<160 && verde>120 && verde<160 && azul>130 && azul<180){  // si valores dentro del rango
  if (rojo > 110 && rojo < 180 && azul >100){
      //if ( 53<rojo<75 && 48<verde<72 && 66<azul<88){
      
      Serial.println("Verde");      // Muestra texto en serial monitor
      return 40;
    } 
    
      //Azul
   // if (rojo<verde && verde >azul && rojo>190 && rojo<245 && verde>200 && verde<260 && azul>120 && azul<180 ){  // si valores dentrol del rango  
   if (rojo > 155 && rojo < 200 && azul <85){
      //if ( 166<rojo<186 && 145<verde<65 && 74<azul<94){

      Serial.println("Azul");       // Muestra texto en serial monitor
      return 50;
    } 
    //delay(200);                    // Demora de 1000 mseg
   // if (rojo>280 && verde>340 && azul>250){  // si valores dentrol del rango  
   if (rojo > 190){
      //if ( 166<rojo<186 && 145<verde<65 && 74<azul<94){
      return 17;
    }
  }

#define CAYENNE_PRINT Serial 

#include <CayenneMQTTESP8266.h> //se incluye la libreria para conectar con cayenne
#include "Ultrasonic.h" //se incluye la libreria para el ultrasonico (HC-

#define VIRTUAL_CHANNEL 7 // se define el canal para conectar con Cayenne

Ultrasonic ultrasonic (14,12);//se definen los pines trig pin 14(d5) y echo pin 12 (D6)

//datos del wifi
char ssid[] = "HUAWEI P smart 2019"; //nombre del wifi
char wifiPassword[] = "123456789"; //contraseña del wifi

// Autenticación de Cayennne
char username[] = "6af98ea0-0323-11ea-a4a3-7d841ff78abf"; //nombre de usuario MQTT Cayenne 
char password[] = "9cbd8166dbdef123e4230a8880d524f8f9ea3f3a"; // contraseña de usuario MQTT Cayenne
char clientID[] = "908de480-089c-11ea-a4a3-7d841ff78abf"; //ClientID de usuario MQTT Cayenne


int GIu_Ultrasonic_Dist_CM=0; 
int buzzer=16; //se define el buzzer en el pin 16(D0)

void setup() {
  Serial.begin(9600);
	Cayenne.begin(username, password, clientID, ssid, wifiPassword);
  pinMode(buzzer,OUTPUT);//se define el buzzer como salida
}

void loop() {
   Cayenne.loop(); 
   
   GIu_Ultrasonic_Dist_CM=ultrasonic.Ranging(CM); //lee la distancia para el ultrasonico
   Cayenne.virtualWrite(V7, GIu_Ultrasonic_Dist_CM); //envia la distancia a MQTT Cayenne
   
   Serial.print(GIu_Ultrasonic_Dist_CM); //imprime en el monitor la distancia
   Serial.println(" cm");
   delay(500);//espera medio segundo
   
  //condicional para el buzzer
  if(GIu_Ultrasonic_Dist_CM <10 ){ //si la distancia es menor a 10
    Serial.println("Garbox lleno"); //imprime en el monitor
    delay(500);//espera medio segundo
   analogWrite(buzzer,128); //emite sonido
     delay(500); //espera medio segundo

  }else{
     digitalWrite(buzzer, LOW); //deja de emitir
     delay(500);//espera medio segundo 
       }
}

CAYENNE_OUT_DEFAULT(){      
      // envia la información del canal a Cayenne
        CAYENNE_LOG("Send data for Virtual Channel %d", VIRTUAL_CHANNEL);
        Cayenne.virtualWrite(VIRTUAL_CHANNEL, millis() / 1000);
}

CAYENNE_IN_DEFAULT(){
	CAYENNE_LOG("Channel %u, value %s", request.channel, getValue.asString());
  }

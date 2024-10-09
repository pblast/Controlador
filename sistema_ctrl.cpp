#include <Arduino.h>
#include <HardwareSerial.h>
#include <string.h>

#include "comunicacion.h"
#include "sistema_ctrl.h"

/* VARIABLES */
unsigned long t=0;
unsigned long t0=0;
int periodo=5000;
unsigned long retardo=10000;


struct comando com;

struct PID pid1;
struct simul sim1;
struct parametros sistema;
void setup(){
  pinMode(13,OUTPUT);
  Serial.begin(9600);
  if(_DEBUG)
    Serial.write("Inicializando....\r\n");

  /* Se cargan parametros iniciales */
  pid1.kp=10;
  pid1.ki=10;
  pid1.kd=10;

  sim1.t0=1000;
  sim1.T=100;

  sistema.p1=pid1;
  sistema.s1=sim1;
}

void loop(){


  if(millis()>(t+periodo)){
    t=millis()-t0;

    if(t<retardo)
      Serial.write('0');
    else
      Serial.write('1');

    if(_DEBUG)
      Serial.write("Estoy vivo...\n\r");
  }

  /* Si se ha recibido un mensaje se almacena */
  if(Serial.available())
    if(recibeMSG(Serial.read(),&com)){
      switch(com.cmd){
      case 0: /* Ayuda */
	enviaAyuda();
	break;

      case 1: /* Lee parametros */
	enviaParametro(com.param,&sistema);
	
	break;

      case 'w':
	break;
      }
    }

  
}


#include <Arduino.h>
#include <HardwareSerial.h>
#include <string.h>

#include "comunicacion.h"
#include "sistema_ctrl.h"

/* VARIABLES */
unsigned long t=0;
unsigned long t0=0;
int periodo=1000;
unsigned long retardo=10000;
float salida=0;

bool estado=0;

struct comando com;

struct PID pid1;
struct simul sim1;
struct parametros sistema;

/* Señales */
float sp=0;
float e=0;
float y=0;
float y2=0;

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
  sim1.T=10;

  sistema.p1=pid1;
  sistema.s1=sim1;
}

void loop(){

  //if(_DEBUG)
  //  Serial.write("Estoy vivo...\n\r");
  
  /* Si se está funcionando se gestionan las señales */
  if(estado==1){
    /* Se actualiza el vector de tiempos */
    t=millis()-t0;
    
    /* Se se ha cumplido el retardo se actualiza la señal escalon */
    if(t>sim1.t0)
      sp=1;
    e=sp-y2;
    if(e>100)
      e=100.0;
    
    y=G(sp,(t-sim1.t0)/1000.0);
    
    // Sistema realimentado
    y2=G(e,(t-sim1.t0)/1000.0);
    /* Si es el periodo de medida se grafican */
    if(t%sim1.T==0){
      
	Serial.print("sp: "); Serial.print(sp); Serial.print("  ");
  Serial.print("Y: "); Serial.print(y); Serial.print("  ");
  //erial.print("e: "); Serial.print(e); Serial.print("  ");
    Serial.print("y2: "); Serial.print(y2); Serial.print("  ");
    Serial.println("uT");
      
    }
  }
  /**********************************************************/
  

  /* Se atienden los mensajes recibidos */
    if(Serial.available()){
      if(recibeMSG(Serial.read(),&com)){
	switch(com.cmd){
	case 0: /* Ayuda */
	  enviaAyuda();
	  break;

	case 1: /* Lee parametros */
	  enviaParametro(com.param,&sistema);
	  break;

	case 2: /* Escribe parámetros */
	  break;

	case 3: /* Inicio */
	  if(estado==0){
	    t0=millis();
	    sp=0;
	    y=0;
	    estado=1;

	  }
	  break;

	case 4: /* Fin */
	  estado=0;
	  break;
	}
      }
    }
}
  /************************************************************/

float G(float input,float t){
  float fprev=y;
  float f;
  float a=5;
  float R=5e6;
  float C=5e-6;
  
  f=a*(1-exp(-t))*input;

  return f;
  
}

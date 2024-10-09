#ifndef _COMUNICACION_H
#define _COMUNICACION_H

#include <Arduino.h>
#include <HardwareSerial.h>

#include "sistema_ctrl.h"

int recibeMSG(char,struct comando*);
int enviaAyuda(void);
int getIndice(char* arr[], int n, char* target);
int enviaParametro(int, struct parametros*);
struct comando{
  int cmd;
  int param;
  float valor;
};

#endif

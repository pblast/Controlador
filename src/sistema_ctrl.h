#ifndef _SIST_CTRL_H
#define _SIST_CTRL_H

#define _DEBUG 1


/* VARIABLES DE PARÁMETROS DEL SISTEMA */
//unsigned long t=0;

/* Estructura con los parametros del controlador */
struct PID{
  float kp;
  float ki;
  float kd;
};

/* Estructura con los parametros de la simulacion */
struct simul{
  unsigned long t0;
  unsigned long T;
};

/* Estructura con todos los parametros del programa */
struct parametros{
  struct simul s1;
  struct PID p1;
};

#endif

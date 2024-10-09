#include "comunicacion.h"
#include "sistema_ctrl.h"

int i=0;
int j=0;

int  recibiendoCMD=0;
bool recibidoCMD=0;
char textoCMD[20];
char temp[10];

char* coms[]={"?","r","w","i","f"};
char* params[]={"per","del","k_p","k_i","k_d"};


/* FUNCION PARA RECIBIR UN COMANDO */
/* Esta funcion se llama cada vez que se detecta que se ha recibido un caracter */
int recibeMSG(char c,struct comando* msg){

  /* Si se reciben 20 caracteres es un comando erroneo */
  if(i>20){
    textoCMD[0]=0;
    recibiendoCMD=0;
    i=0;
    Serial.write("NACK\n\r");
    return 0;
  }

  switch (recibiendoCMD){
    /* AUN NO SE ESTA RECIBIENDO MENSAJE */
  case 0:
    
    textoCMD[0]=c;
    textoCMD[1]=0;
    msg->cmd=getIndice(coms,sizeof(coms)/sizeof(coms[0]),textoCMD);


    if(msg->cmd!=-1){
      if(_DEBUG)
	Serial.write("Se ha recibido un comando válido\r\n");
      
      recibiendoCMD=1;
      textoCMD[i]=c;
      //i++;
      i=0;
      msg->param=0;
      msg->valor=0;
      
    }
      
    break;      
     
  /* SE ESTÁ RECIBIENDO EL PARAMETRO */
  case 1:
    textoCMD[i]=c;

    /* Se ha acabado de recibir el parametro */
    if((c=='=')&&(msg->cmd==1)){
      textoCMD[i]=0;

      msg->param=getIndice(params,sizeof(params)/sizeof(params[0]),textoCMD)+1;

      recibiendoCMD=2;
      j=0;
    }
      else if((c==';')&&(msg->cmd==0)){
      textoCMD[i]=0;


      msg->param=getIndice(params,sizeof(params)/sizeof(params[0]),textoCMD)+1;

      goto FIN;
    }
    else{

      i++;

      
    }
	  
    break;

    /* SE ESTÁ RECIBIENDO EL VALOR */
    case 2:
      textoCMD[i]=c;
      i++;
	  
    if(c!=';'){
      temp[j]=c;
      j++;
    }
    else{
      /* Fin de mensaje */
      msg->valor=atof(temp);
      goto FIN;

	break;
    }
  }
        
  return 0;

 FIN:
  
      Serial.write ("ACK...");
      if(_DEBUG)
	Serial.println(textoCMD);

      textoCMD[0]=0;
      recibiendoCMD=0;
      i=0;
      j=0;
	    
      return 1;
}


/* FUNCION PARA ENVIAR RESPUESTA DE UN COMANDO */
int enviaAyuda(void){

    Serial.println("********** COMANDOS **********");
    Serial.println("? - Ayuda");
    Serial.println("r<param> - Escribe parámetro");
    Serial.println("w<param> - Lee parámetro");
    Serial.println("i - Inicio");
    Serial.println("f - Fin");
    
    Serial.println("\n\r********** PARAMETROS **********");
    Serial.println("k_p-Proporcional");
    Serial.println("k_i-Integral");
    Serial.println("k_d-Derivativa");
    Serial.println("per-Periodo de muestreo(ms)");
    Serial.println("del-Retraso del escalon");
    Serial.println("amp-Amplitud del escalon");
    Serial.println("********************");	

    return 1;
}

int enviaParametro(int i,struct parametros* p){
  switch(i){
    case 0: /* Se muestran todos */
    Serial.write("Periodo=");
    Serial.println(p->s1.T,DEC);
    Serial.write("Retardo=");
    Serial.println(p->s1.t0,DEC);
    Serial.write("kp=");
    Serial.println(p->p1.kp,DEC);
    Serial.write("ki=");
    Serial.println(p->p1.ki,DEC);
    Serial.write("kd=");
    Serial.println(p->p1.kd,DEC);
    break;

  case 1:
    Serial.write("Periodo=");
    Serial.println(p->s1.T,DEC);
    break;

  case 2:
    Serial.write("Retardo=");
    Serial.println(p->s1.t0,DEC);
    break;

  case 3:
    Serial.write("kp=");
    Serial.println(p->p1.kp,DEC);
    break;

  case 4:
    Serial.write("ki=");
    Serial.println(p->p1.ki,DEC);
    break;

  case 5:
    Serial.write("kd=");
    Serial.println(p->p1.kd,DEC);
    break;
  }
  return 1;
}


/* Funcion auxiliar para convertir comandos y parametros en indices, mas manejables */
int getIndice(char* arr[], int n, char* target) {  
  
  for (int i = 0; i< n; i++) {
    Serial.print(arr[i]);
    Serial.write("\n\r");
    if (strcmp(arr[i],target)==0) {
      return i;  // Return the index if the target is found  
    }  
  }
  return -1;  // Return -1 if the target is not found  
}  

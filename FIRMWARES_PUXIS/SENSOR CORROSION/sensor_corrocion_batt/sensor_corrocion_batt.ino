#include <SoftwareSerial.h>
//#include <MemoryFree.h>

char DATO_CORR[15];                    // Obtiene toda la cadena de 15 caracteres del string 
char PULG[3];                          // guarda solo 2 caracteres de los 15 acarcteres (del string de corrocion) dado por el sensor 
int x=0;                               // cuenta las posiciones de la cadena de 15 caracteres para obtener una por una
float MPY=0;                           // variable regresa para la convcersion de char a entero
float MPY1=0;                          // variable regresa el valor de la ecuacion (de PULAGGADA a MICRAS) 

float voltaje_batt=0;
float SUMA=0;
float VOLT=0;

SoftwareSerial RS485(8,7);          //(RX,TX)   asigna el serial por software

void setup(){
   
   Serial.begin(2400);          
   RS485.begin(2400);
    
}  
    
void loop(){
       
   CORROCION();
   VOLTAJE_BATTERY();
   
   
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////
void VOLTAJE_BATTERY(){
  voltaje_batt=0;
    for (int x=0;x<2000;x++)
                                {     
         VOLT=analogRead(A5)*0.0048875855;                              //
           SUMA=SUMA+VOLT;
                                }
           
       voltaje_batt=(SUMA/2000)+0.2;   
    Serial.write("\n\n");   
    Serial.print("BATTERY = ");              // imprime la cadena DATO_CORR
    Serial.print(voltaje_batt);      
    SUMA=0;  
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
void CORROCION(){
   RS485.write(01);                // envia el valor 1 en binario por el SERIAL (direccion del sensor- seleccionada en el bit swich de sensor)
   RS485.write("\n");              // envia ENTER para obtener el valor del RS-485  
   
    delay(5000);  
    while(RS485.available()>0){           // detecta la variable de regreso por el sensor  
  //  Serial.write(RS485.read()); 
    
     for(int x=0;x<15;x++){               // asigna la posicion de la cadena dada por el sensor (del 0 al 15 "15 caracteres")

      DATO_CORR[x]=RS485.read();          // guarda en variable DATO_CORR la cadena que regresa el sensor 
       PULG[0]=DATO_CORR[5];              // guarda en la posicion 0 de la cadena PULG - la posicion 5 de la cadena DATO_CORR
       PULG[1]=DATO_CORR[6];              // guarda en la posicion 1 de la cadena PULG - la posicion 6 de la cadena DATO_CORR
       
    }
    Serial.write("\n\n"); 
    Serial.print("DATO_CORR = ");              // imprime la cadena DATO_CORR
    Serial.print(DATO_CORR);     
    
    MPY=atol(PULG);                            // convierte el char (PULG)- en entero(MPY
    MPY1=((MPY/100000)*25400);                 // hace la conversion de PULGADAS a MICRAS
    
    Serial.write("\n"); 
    Serial.print("PULG = ");                   // imprime la cadena PULG
    Serial.print(PULG); 
    
   Serial.write("\n"); 
   Serial.print("MPY = ");                      // imprime la cadena MPY
   Serial.print(MPY1);
    }  
}  


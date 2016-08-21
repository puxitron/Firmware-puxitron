#include <EEPROM.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <SoftwareSerial.h> //
#include <avr/wdt.h>
#include <avr/sleep.h>
#include <avr/power.h>

SoftwareSerial RS485(8,7);              //(RX,TX)   asigna el serial por software
SoftwareSerial SIM5320(9, 15);          // RX, TX

OneWire TMP_DALLAS(19);
DallasTemperature sensors(&TMP_DALLAS);
//////////////////////////////////////  VARIABLES CORROCION ////////////////////////////////////////////////////////////////////

char DATO_CORR[15];                    // Obtiene toda la cadena de 15 caracteres del string 
char PULG[3];                          // guarda solo 2 caracteres de los 15 acarcteres (del string de corrocion) dado por el sensor 
int x=0;                               // cuenta las posiciones de la cadena de 15 caracteres para obtener una por una
float MPY=0;                           // variable regresa para la convcersion de char a entero
float MPY1=0;                          // variable regresa el valor de la ecuacion (de PULAGGADA a MICRAS) 
char   BUFFER_MPY1[8];
String MPY1_STR;
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////  
byte  INIT_EEPROM; 
long int    Puxitron=0; 
int    INT_TIMER16;
char   WEB_TIEMPO[6];
String TIEMPO_INT;
String IMEI;
String IP="192.168.1.1";
String PH;
String TMP="36.00";
String OXI="36.00";
String BAT_V;
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int long acumulador=0;
int long SUMA=0;
float    PROM=0;                                            //ADC REGISTROS Y PROMEDIO
float    INTER=0;
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 int     X=0;                                                                        
 int     I;
 int     D;
 int     P;                                          //REGISTROS DEL SERIAL
 int     Z;
 int     COPY;
 int     COPY_TIME;
 int     ZZ;
 char    BUFFER_USART;
 char    BUFFER_USART2[150];
 char    TIME_BUFFER[22];
 int     FLAG;
 int     FREQ_CHECK;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void setup() { 

  delay(1000);
  sensors.begin();                                  //INICIA EL SENSOR DALLAS TEMPERATURA
  sensors.setResolution(TEMP_10_BIT);               //10BITS DE RESOLUCION EN LA TEMPERATURA
////////////////////////////////////////////////////////////////////////////
  MCUSR &= ~(1<<WDRF);
  WDTCSR |= (1<<WDCE) | (1<<WDE);
  WDTCSR = 1<<WDP0 | 1<<WDP3; 
  WDTCSR &= ~(_BV(WDIE));
//////////////////////////////////////////////////////////////////////////////  
  SREG    =0x80;                                    //INT GLOBAL SOLO ACTIVA LA DEL UART
  UCSR0B  =0x98;                                    //ACTIVA TX Y RX A SI COM LA INT DEL RX
  UCSR0C  =0x06;                                    //CONFIGURAMOS A 8BITS DE DATOS SERIAL
  UBRR0H  =0x00;                                    //VALOR QUE SE LE CARGA AL REGISTRO HIGH 0
  UBRR0L  =0x67;                                    //VALOR QUE SE LE CARGA AL REGISTRO LOW 0x67 QUE ES IGUAL A 103 PARA 9600
//////////////////////////////////////////////////////////////////////////////
  RS485.begin(2400);
  SIM5320.begin(9600);
  pinMode(15, OUTPUT);                              //SOFTWARE SERIAL TX SALIDA A1
  pinMode(14, INPUT);                              // SESONR PH A0
  pinMode(10,OUTPUT);    
  pinMode(9, INPUT);                              //
 // pinMode(8, INPUT);                               //
 // pinMode(7, OUTPUT);                              //
  pinMode(6, INPUT); 
  pinMode(5, OUTPUT);                              //
  pinMode(4, INPUT);                               //
  pinMode(3, OUTPUT);                              //ENCENDIDO DEL SIM5320
  pinMode(2, OUTPUT);                              
  pinMode(1, OUTPUT); 
  pinMode(0, INPUT);                               //
  analogReference(DEFAULT);
////////////////////////////////////////

  SIM900_ON(); //ENCIENDO EL SIM 
  delay(20000);

  //if(INIT_EEPROM=EEPROM.read(6) == 0x00){
     // EEPROM.write(6,10);                               //TIEMPO POR DEFAULT WEB
        //SIM5320.println("AT+CIPS=ESCEE");  //
          //delay(2000);
                                                  //}
  SETUP_SIM5320();
}





void loop() {

CORROSION();   

WEB();

}
///////////////////////////////////////////////////////////////////////////
ISR (USART_RX_vect)
{
   //while(!(UCSR0A&(1<<UDRE0))){
    //                              };     //SI TERMINO DE RECIVIR LO ESCRIBE EN TX PARA ENVIAR
    
    X++;
    BUFFER_USART = UDR0;                // LEE EL BUFFER RX                    
    
    if(COPY!=0&&X>I+2&&X<D+4){
    Z++;
    BUFFER_USART2[Z]= BUFFER_USART;  
                       
                                   } 
    
    if(FREQ_CHECK!=0&&BUFFER_USART == '{'){
      FLAG=1;
                                                   }
    
                    if(COPY_TIME!=0&&FLAG!=0){
                      ZZ++;
                          TIME_BUFFER[ZZ]= BUFFER_USART;  
                                          if(TIME_BUFFER[ZZ] == '}'){
                                            COPY_TIME=0;
                                              FLAG=0;
                                                ZZ=-1;
                                                 FREQ_CHECK=0;                       
                                                                       }
                                                                               } 

    //UDR0 = BUFFER_USART;                // ESCRIBE EN EL BUFFER TX  

} 
//////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
void WEB()                                  //  
{
   
int x=0;
byte EEPROM_DEFAULT;
int JSON_3DATOS=0;
int JSON_BASE1=86;

int SEND=136;

  SIM5320.listen();

  sensors.requestTemperatures();        //Prepara el sensor para la lectura

  COPIA_BUFFER(18,22);
  SIM5320.println("AT+CBC");          //VOLTAJE BATERIA
  delay(500);
  LIMPIA_BUFFER();
  BAT_V=BUFFER_USART2;
  
  SIM5320.println("AT+CSQ");          //CALIDAD DE CONSULTA DE SEÑAL
  delay(500);

  while (x<6) {
  COPIA_BUFFER(1,90);
  SIM5320.println("AT+CIPOPEN=0,\"TCP\",\"vtechph.herokuapp.com\",80");  //vtechph.herokuapp.com
  delay(6000);
  LIMPIA_BUFFER();                                         
  x++;                                               

                                                           if (x == 5) {              
                                                              SIM900_ON(); 
                                                                 delay(2000); 
                                                                    software_reset (); 
                                                                                                            }//IF  
  
                                                                                                         



                                                            if (strcasestr(BUFFER_USART2, "+CIPOPEN: 0,0")) { 
                                                                  break;  
                                                                                                              }//IF
                                                                                                             
                                                                      
                                                                      
        
                                                                      
                                                                      
                                                                      
                                                                      } //WHILE
                                                            

  
  
   
   JSON_3DATOS=JSON_BASE1+IMEI.length()+IP.length()+BAT_V.length()+MPY1_STR.length()+4;                         //CALCULA LA LONGITUD DEL JSON
   SEND+=JSON_3DATOS;                                                                   //CALCULA LA LONGITUD DE LO QUE VA ENVIAR DESPUES DEL PROMPT(SEND)
   
   SIM5320.print("AT+CIPSEND=0,");  //
   delay(100);
   SIM5320.println(SEND);  //
   delay(2000);
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////  
   SIM5320.println(F("POST /vtechinfos/ HTTP/1.1"));
   delay(50);
   SIM5320.println(F("Content-Type: application/json"));
   delay(50);
   SIM5320.println(F("Host: vtechph.herokuapp.com"));
   delay(50);
   SIM5320.println(F("Connection: keep-alive"));
   delay(50);
   SIM5320.print(F("Content-Length:"));   
   delay(50);
   SIM5320.println(JSON_3DATOS);   
   delay(50);
   SIM5320.println("");
///////////////////////////////////////////////////////////////   
   SIM5320.print(F("[{\"imei\":\""));
   delay(50);
   SIM5320.print(IMEI);
   delay(50);
   SIM5320.print(F("\",\"ip\":\""));
   delay(50);
   SIM5320.print(IP);                                                           //JSON PH
   delay(50);
   SIM5320.print(F("\",\"kpi\":\"corr\",\"kpivalue\":\""));
   delay(50);
   SIM5320.print(MPY1_STR);
   delay(50);
   SIM5320.print(F("\",\"batterystate\":\""));
    delay(50);
   SIM5320.print(BAT_V);
   delay(50);
   SIM5320.print(F("\",\"kpiunitmeasure\":\"corr\"}]"));
   delay(50);
 
   SIM5320.write(0x1A);
   TIME_JSON();
   delay(15000);   
//////////////////////////////////////////////////////////////  
   SIM5320.println(F("AT+CIPCLOSE=0"));                                          
   delay(3000); 

         
 if (!strcasestr(TIME_BUFFER,"\"frequencetime\":0.5")) {        
                
                 
                 if (strcasestr(TIME_BUFFER,"\"frequencetime\":")) {
                                for (int x=17;x<19;x++) {
                                      WEB_TIEMPO[x-17]=TIME_BUFFER[x];             
             
                                                                          } 
                 
                                          TIEMPO_INT=WEB_TIEMPO;             
                                                  INT_TIMER16=TIEMPO_INT.toInt();
                                                         if(INT_TIMER16 == 60){
                                                                     INT_TIMER16=66; 
                                                                                         }
                                        
                                                            if(INT_TIMER16 == 1){
                                                                  Puxitron=1; 
                                                                                     }
                                        
                                                              else {
                                                                  Puxitron=INT_TIMER16*6;
                                                                                 }

                             /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                                  EEPROM_DEFAULT= EEPROM.read(6);                                      
                                  if (EEPROM_DEFAULT != INT_TIMER16) {         ///UNICAMENTE ESCRIBE EN LA EEPROM SI EL VALOR CAMBIO DE LA WEB 
                                        EEPROM.write(6,INT_TIMER16);
                                    EEPROM_DEFAULT= EEPROM.read(6);   
                                    //SIM5320.print("AT+GRABO_EEPROM:");      //  
                                    //SIM5320.println(EEPROM_DEFAULT);      //      
                                                                      
                                                                       
                                                                       } 
                            //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////                                                
          
                            SIM5320.println(F("AT+NETCLOSE"));      //
                              delay(2000);
                                  SIM900_ON();                      //APAGA EL SIMCOM
                                    delay(2000);
                                      ATMEGA328_DUERME();                                                             
                                          software_reset ();                                                         
                                                                    }//IF QUE BUSCA EL TIEMPO                      
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 Puxitron= EEPROM.read(6); 
                                                         if(Puxitron == 60){
                                                                      Puxitron=66; 
                                                                                         }
                                                             if(Puxitron == 1){
                                                                       Puxitron=1; 
                                                                                     }

                                                                             else{
                                                                                Puxitron=Puxitron*6;                         
                                                                                                           }
                                                                                         
  
      //SIM5320.print("AT+GRABO_EEPROM:");  
        //SIM5320.println(Puxitron);                                       
          SIM5320.println(F("AT+NETCLOSE"));    //
            delay(2000);
              SIM900_ON();                      //APAGA EL SIMCOM    
                   delay(2000);
                      ATMEGA328_DUERME();       //SI EL WEVSERVER NO ENVIO NADA LO DUERMO CON EL VALOR DE LA EEPROM                                                           
                         software_reset ();  
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////                                      
                                                                           
                                                                                                                 }//IF PRINCIPAL

 
 
 } 

void SIM900_ON()                                  //  
{
  digitalWrite(2, HIGH);
  delay(2000);//                                     //ENCIENDE EL SIM5320
  digitalWrite(2, LOW);
  delay(2000);
 } 
////////////////////
void PH_ADC() {
char PH_CHAR[10];                          //BUFFER DONDE SE GUARDA LA CONVERCION DEL FLOAT 
PROM=0;
SUMA=0;
INTER=0;
     
for (int x=0;x<4000;x++) {
           
      acumulador=analogRead(A0);  
      if(acumulador>480){
        //bitSet(acumulador,0);
        //bitSet(acumulador,1);
                               }
       SUMA+=acumulador;
                                                  } 

PROM=SUMA/4000; 
PROM=PROM*0.0048666;             //promedio es el voltaje

INTER=((PROM-1.980)*(4-10)/(3.00-1.980))+10;
 
   if(INTER>10.00){
              INTER=10.0000;
                  }

   if(INTER<4.00){
              INTER=4.0000;
                  }               
     
dtostrf(INTER,5,2,PH_CHAR);                  //CONVIERTE DE FLOTANTE A CHAR

PH=PH_CHAR;
//Serial.print(INTER,4);
//Serial.println(" PH");
//PROM=0;
//SUMA=0;
//INTER=0;
//delay(1000);
//Serial.println(" ");
}


////////////////////////////////////////////////////////////
void COPIA_BUFFER(int P,int W)                       // FUNCION QUE LIMPIA TODO EL STRING QUE GUARDA LAS VARIABLES DE RX
{
  //K=0;
  COPY=1;                                 
    X=0;
      Z=-1;
      for (int i=0; i<99;i++)
        {BUFFER_USART2[i]=NULL;}                  //CADA POSICION DEL STRING LE PONE CARACTER NULL 00
          I=P; 
            D=W;
 //return K;
}

void LIMPIA_BUFFER()                          // FUNCION QUE LIMPIA TODO EL STRING QUE GUARDA LAS VARIABLES DE RX
{
  COPY=0;
    X=0;
      Z=0;
        I=0;
          D=0;
}

void TIME_JSON()                              // FUNCION QUE LIMPIA TODO EL STRING QUE GUARDA LAS VARIABLES DE RX
{
  COPY_TIME=1;
      ZZ=-1;
       FREQ_CHECK=1;

}
/////////////////////////////////////////////////////////////////////////////
void SETUP_SIM5320()                       // FUNCION QUE LIMPIA TODO EL STRING QUE GUARDA LAS VARIABLES DE RX
{
  SIM5320.listen();
    
  SIM5320.println(F("AT+CFUN=1"));    //
  delay(250);

  SIM5320.println(F("AT+CVAUXV=61"));    //GPS APGADO PARA NO GASTAR PILA
  delay(250);  
  
  SIM5320.println(F("AT+CGPS=0"));    //GPS APGADO PARA NO GASTAR PILA
  delay(500);

  SIM5320.println(F("AT+CNAOP=0"));    //
  delay(250);
  
   
   SIM5320.println(F("AT+CNMP=13"));  //automatico

  //  SIM5320.println(F("AT+CNMP=13"));  //gsm
  //delay(250);

  // SIM5320.println(F("AT+CNMP=14"));  //wcdma
   delay(250);
  
  
  
  SIM5320.println(F("AT+CNBP=0x0000000000200000"));  //3G EN 850MHZ EN TELCEL
 
  //SIM5320.println(F("AT+CNBP=0x0000000004000000"));  //3G EN 850MHZ EN TELCEL
  delay(500);
  

  
  //SIM5320.println(F("AT+IPREX=9600"));  //
  //delay(4000);

  //SIM5320.println(F("AT+CIPTIMEOUT=30000,30000,30000"));  //
  //delay(500);

  delay(10000);
  SIM5320.println(F("AT+CSQ"));  //PREGUNTO LA CALIDAD DE LA SEÑAL 
  delay(250);

  SIM5320.println(F("AT+CREG?"));  //REGISTRO DE RED, ME TIENE QUE DAR UN 1
  delay(250);
  
  SIM5320.println(F("AT+COPS?"));  //SELECCION DE OPERADOR
  delay(250);
  //////////////////////////////////
  ////SIM5320.println(F("AT+CGSOCKCONT=1,\"IP\",\"web.iusacellgsm.mx\""));  //DEFINO EL ENCHUFE QUE EN ESTE CASO ES TELCEL
  //delay(4000);

  SIM5320.println(F("AT+CGSOCKCONT=1,\"IP\",\"internet.itelcel.com\""));  //DEFINO EL ENCHUFE QUE EN ESTE CASO ES TELCEL
  delay(250);

  SIM5320.println(F("AT+CNSMOD?")); //MUESTRA EL TIPO DE RED EN EL QUE ENTRO EL SISTEMA
  delay(250);

  SIM5320.println(F("AT+CSOCKSETPN=1")); //ACTIVA EL PDP
  delay(250);

  SIM5320.println(F("AT+CIPMODE=0"));  //MODO TRANSPARENTE, EN ESTE CASO ES MODO NO TRANSPARENTE
  delay(250);

  SIM5320.println(F("AT+CNBP?"));  //ESTADO DE BANDA DE PREFERENCIA
  delay(250);

  SIM5320.println(F("AT+NETOPEN")); //ACTIVE PDP CONTEXT PROFILE NUMBER
  delay(8000);

  SIM5320.println(F("AT+IPADDR"));  //CONSULTA LA DIRECCION IP LOCAL
  delay(500);


 COPIA_BUFFER(8,22);
 SIM5320.println(F("AT+CGSN"));  //IMEI
 delay(500);
 LIMPIA_BUFFER();
 IMEI=BUFFER_USART2;

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void software_reset()
{
  
  wdt_reset(); 
  wdt_enable(WDTO_8S);
  while  (1);{
                }
}  
////////////////////////////
void ATMEGA328_DUERME(void)                 //FUNCION PARA QUE EL ATMEGA ENTRE EN SLEEP Y AHORRE MUCHOS mA
{
for(int x=0;x<Puxitron;x++){
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);     //TIPO DE AHORRO DE ENERGIA (SON 5 TIPOS )
  sleep_enable();                          //ACTIVO EL MODO SLEEP
  WDTCSR |= _BV(WDIE);                     //ACTIVA LA INT DEL PERRO GUARDIAN       
  sleep_mode();                            //SE VA A DORMIR                        
  sleep_disable();                         //REGRESA A ESTE PUNTO DESPUES DE DORMIR PUESTO QUE EL WD LO DESPIERTA 
  power_all_enable();                      //ACTIVA TODOS LOS PERIFERICOS ON                       
                          }                

} 
////////////////////////////
ISR(WDT_vect)                              //FUNCION DEL WD
{
  
  WDTCSR &= ~(_BV(WDIE));
 //bitClear(WDTCSR,6);                     //DESACTIVA LAS INT DEL WD   

} 
//////////////////////   FUNCION DE SENSOR DE CORROCION  //////////////////////////////////// 
void CORROSION(){
   RS485.listen();
   
   RS485.write(01);                // envia el valor 1 en binario por el SERIAL (direccion del sensor- seleccionada en el bit swich de sensor)
   RS485.write("\n");              // envia ENTER para obtener el valor del RS-485  
   
    delay(10000);  
    while(RS485.available()>0){           // detecta la variable de regreso por el sensor  
  //  Serial.write(RS485.read()); 
    
     for(int x=0;x<15;x++){               // asigna la posicion de la cadena dada por el sensor (del 0 al 15 "15 caracteres")

      DATO_CORR[x]=RS485.read();          // guarda en variable DATO_CORR la cadena que regresa el sensor 
       PULG[0]=DATO_CORR[5];              // guarda en la posicion 0 de la cadena PULG - la posicion 5 de la cadena DATO_CORR
       PULG[1]=DATO_CORR[6];              // guarda en la posicion 1 de la cadena PULG - la posicion 6 de la cadena DATO_CORR
       
    }
    // Serial.write("\n\n"); 
     //Serial.print("DATO_CORR = ");              // imprime la cadena DATO_CORR
     //Serial.write(DATO_CORR);     
    
    MPY=atol(PULG);                            // convierte el char (PULG)- en entero(MPY
    MPY1=((MPY/100000)*25400);                 // hace la conversion de PULGADAS a MICRAS
        
    dtostrf(MPY1,5, 2, BUFFER_MPY1);

    MPY1_STR=BUFFER_MPY1;
    
    // Serial.write("\n\n"); 
    // Serial.print("PULG = ");                   // imprime la cadena PULG
    // Serial.write(PULG); 
    
    //Serial.write("\n\n"); 
    //Serial.print("MPY = ");                      // imprime la cadena MPY
    //Serial.print(MPY1);
    }   
}   


      
      //dtostrf(MPY1,5, 2, BUFFER_MPY1);

      //MPY1_STR=BUFFER_MPY1;

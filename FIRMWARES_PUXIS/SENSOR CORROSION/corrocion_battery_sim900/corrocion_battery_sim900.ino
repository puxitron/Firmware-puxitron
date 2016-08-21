#include <EEPROM.h>
#include <avr/wdt.h>
#include <OneWire.h> 
#include <DallasTemperature.h>
#include <SoftwareSerial.h>


SoftwareSerial RS485(8,7);          //(RX,TX)   asigna el serial por software
SoftwareSerial SIM900(15, 16);      //(RX,TX)

int    INT_TIMER16;
char   WEB_TIEMPO[6];
String TIEMPO_INT;
int long Puxitron;
byte EEPROM_DEFAULT;

int RESET=0;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int  z=0;
float buffer2    [21];
float buffer     [6];
float DATOS      [10]; 
int   ARETE_N=0;
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
String APN_TELCEL_1  =("AT+SAPBR=3,1,\"APN\",\"internet.itelcel.com\"");
String APN_TELCEL_2 = ("AT+SAPBR=3,1,\"USER\",\"webgprs\"");
String APN_TELCEL_3  =("AT+SAPBR=3,1,\"PWD\",\"webgprs2003\"");

String APN_IUSACELL_1=("AT+SAPBR=3,1,\"APN\",\"web.iusacellgsm.mx\"");
String APN_IUSACELL_2=("AT+SAPBR=3,1,\"USER\",\"Iusacellgsm\"");
String APN_IUSACELL_3=("AT+SAPBR=3,1,\"PWD\",\"iusacellgsm\"");
String VPN_IUSACELL=  ("AT+CSTT=\"web.iusacellgsm.mx\",\"Iusacellgsm\",\"iusacellgsm\"");
String VPN_TELCEL  =  ("AT+CSTT=\"internet.itelcel.com\",\"webgprs\",\"webgprs2003\"");

char   COMPANIA [10];                                //GUARDA EL NOMBRE DE LA COMPAÑIA A LA QUE PERTNECE EL CHIP
String CHIP;                                        //GUARDA EL NOMBRE DE LA COMPAÑIA 


char   FECHA_[25];
String FECHA_SIM;


String RESPUESTA_SIM;

char   ADC_SIM  [5]; 
float  ADC_PROMEDIO;


unsigned long int ADC_SIM900; 

//////////////////////////////////////  VARIABLES CORROCION ////////////////////////////////////////////////////////////////////

long previousMillis=0;            
void CORROCION();
char DATO_CORR[15];                    // Obtiene toda la cadena de 15 caracteres del string 
char PULG[3];                          // guarda solo 2 caracteres de los 15 acarcteres (del string de corrocion) dado por el sensor 
int x=0;                               // cuenta las posiciones de la cadena de 15 caracteres para obtener una por una
float MPY=0;                           // variable regresa para la convcersion de char a entero
float MPY1=0;                          // variable regresa el valor de la ecuacion (de PULAGGADA a MICRAS) 
int veces=0;
char   BUFFER_MPY1[8];
String MPY1_STR;
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 int     ZZ; 
 int     COPY_TIME;
 int     X=0;                                                                        
 int     I;
 int     D;
 int     P;                                          //REGISTROS DEL SERIAL
 int     Z;
 int     COPY;
 char    BUFFER_USART;
 char    BUFFER_USART2[150];
 char    TIME_BUFFER[22];
 int     FLAG;
 int     FREQ_CHECK;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
float   data;
#define ONE_WIRE_BUS 4                               //TEMPERATURA D18B20
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
float V;                                            //VOLTAJE BATERIA
float VOLTAJE_BATERIA_DIVISOR;
float SUMA;
float POSTE;  
float V_POSTE;
float SUMA_POSTE;
float VOLTAJE_BATERIA=0;
float voltaje_batt=0;
//float SUMA=0;
float VOLT=0;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 void setup() {
  sensors.begin();
  sensors.setResolution(TEMP_10_BIT);               //SETEO 10BITS DE RESOLUCION   
 ///////////////////////////////////////////////////////////////////////////////////////////   
  SREG    =0x80;                                    //INT GLOBAL SOLO ACTIVA LA DEL UART
  UCSR0B  =0x98;                                    //ACTIVA TX Y RX A SI COM LA INT DEL RX
  UCSR0C  =0x06;                                    //CONFIGURAMOS A 8BITS DE DATOS SERIAL
  UBRR0H  =0x00;                                    //VALOR QUE SE LE CARGA AL REGISTRO HIGH 0
  UBRR0L  =0x67;                                    //VALOR QUE SE LE CARGA AL REGISTRO LOW 0x67 QUE ES IGUAL A 103 PARA 9600
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////  
  SIM900.begin(9600); 
  RS485.begin(2400);
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
  pinMode(A5, INPUT);                               //ADC VOLTAJE BATERIA SYTEM 
  pinMode(A4, INPUT);                               //ADC VOLTAJE POSTE 
  pinMode(17, INPUT);                               //ADC 3
  //pinMode(16, INPUT);                             
  pinMode(A5, INPUT); 
  pinMode(14, INPUT);                               //ADC 0
  

  pinMode(10,OUTPUT);    
  pinMode(9, OUTPUT);                              //CONFIGURACION I/O PINES
  pinMode(6, INPUT);                               //SELECCIONA LA RED EN LA QUE VA OPERAR
  pinMode(5, OUTPUT);                              //ACTIVA EL SIM900 
  pinMode(4, INPUT);                               // 
 /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////      
  analogReference(DEFAULT);                       //VOLTAJE DE REFERENCIA DEL ADC        
  SIM900_ON(); 
  delay(8000);
  SETUP_SIM900();

                                                 }  
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////                                                 

 void loop() {
   
    CORROSION();       
    TEMPERATURA();
    VOLTAJE_BATTERY();
    ENVIO_WEB();  
    SIM900.print("AT+CIPMODE=");   
    SIM900.println(INT_TIMER16);
    delay(Puxitron);

 } 

/////////////////////////  VOLTAJE DE BATERIA //////////////////////////////////////////////////
void VOLTAJE_BATTERY(){
  voltaje_batt=0;
    for (int x=0;x<2000;x++)
                                {     
         VOLT=analogRead(A5)*0.0048875855;                              //
           SUMA=SUMA+VOLT;
                                }
           
       voltaje_batt=(SUMA/2000)+0.2;       
    SUMA=0;  
}                                                     
//////////////////////   FUNCION DE SENSOR DE CORROCION  //////////////////////////////////// 
void CORROSION(){
   veces++;
  RS485.listen();

  RS485.write(01);                // envia el valor 1 en binario por el SERIAL (direccion del sensor- seleccionada en el bit swich de sensor)
  RS485.write("\n");              // envia ENTER para obtener el valor del RS-485  

  //delay(10000);  
  while(RS485.available()>0){           // detecta la variable de regreso por el sensor  

    for(int x=0;x<15;x++){               // asigna la posicion de la cadena dada por el sensor (del 0 al 15 "15 caracteres")
    
      DATO_CORR[x]=RS485.read();          // guarda en variable DATO_CORR la cadena que regresa el sensor 
      PULG[0]=DATO_CORR[5];              // guarda en la posicion 0 de la cadena PULG - la posicion 5 de la cadena DATO_CORR
      PULG[1]=DATO_CORR[6];              // guarda en la posicion 1 de la cadena PULG - la posicion 6 de la cadena DATO_CORR

      MPY=atol(PULG);                            // convierte el char (PULG)- en entero(MPY
      MPY1=((MPY/100000)*25400);                 // hace la conversion de PULGADAS a MICRAS
     
      //SIM900.print("MPY1 = ");                      // imprime la cadena MPY
      //SIM900.print(MPY1);
     
      dtostrf(MPY1,5, 2, BUFFER_MPY1);

      MPY1_STR=BUFFER_MPY1;
    }     
  }  
}     
 
///////////////////////////////////////////////////////////////////////////
ISR (USART_RX_vect)
{
    X++;
    BUFFER_USART = UDR0;                // LEE EL BUFFER RX                    
    
    if(COPY!=0&&X>I&&X<D){
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




} 
////////////////////////////////////////////////////////////////////////


void ENVIO_WEB() 
{
int JSON=124;
int X=0;  

  

while(X < 6){   
  COPIA_BUFFER(1,120);
  SIM900.println("AT+CIPSTART=\"tcp\",\"vtechc.herokuapp.com\",\"80\"");  
  delay(10000);                   
  LIMPIA_BUFFER();
  X++;                        if (strcasestr(BUFFER_USART2, "CONNECT FAIL")) {           //BUSCA ERROR EN EL ARRAY CHAR DEL SERIAL SI LO ENCUENTRA APAGA EL SIM900 
                                SIM900_OFF();  
                                  delay(2000);  
                                    asm volatile ( "jmp 0");     }
                               
                               
                              if (strcasestr(BUFFER_USART2, "ERROR")) {           //BUSCA ERROR EN EL ARRAY CHAR DEL SERIAL SI LO ENCUENTRA APAGA EL SIM900 
                                SIM900_OFF();  
                                  delay(2000);  
                                    asm volatile ( "jmp 0");  

                                                                                     } 
                                                                                     
                              if (strcasestr(BUFFER_USART2, "CONNECT OK")) {           //BUSCA ERROR EN EL ARRAY CHAR DEL SERIAL SI LO ENCUENTRA APAGA EL SIM900 
                                break;

                                                                                     }                                                       

                          }//WHILE
  COPIA_BUFFER(1,70);
  SIM900.println("AT+CIPSEND");                                                   
  delay(2000);
  LIMPIA_BUFFER();
                              if (strcasestr(BUFFER_USART2, "ERROR")) {           //BUSCA ERROR EN EL ARRAY CHAR DEL SERIAL SI LO ENCUENTRA APAGA EL SIM900 
                              SIM900_OFF();  
                            
                              asm volatile ( "jmp 0");  
                                                                  
                                                                
                                                                          
                                                                        
                                                                                     } 

  SIM900.println("POST /vtechinfos/ HTTP/1.1");
  delay(100);
  SIM900.println("Content-Type: application/json");
  delay(100);
  SIM900.println("Host: vtechc.herokuapp.com");
  delay(100);
  SIM900.println("Connection: keep-alive");
  delay(100);
  SIM900.print("Content-Length:");
  delay(100);
  SIM900.println(JSON);
  delay(100);
 
  SIM900.println("");
  delay(100);  
///////////////////////////////////////////////////////////////   
                                012813006323523
   SIM900.print(F("[{\"imei\":\"013950006783907"));
   delay(50);
   //SIM5320.print(IMEI);
   delay(50);
   SIM900.print(F("\",\"ip\":\"192.168.1.1"));
   delay(50);
   SIM900.print(F("\",\"kpi\":\"corr\",\"kpivalue\":\""));
   delay(50);
   SIM900.print(MPY1,2);
   delay(50);
   SIM900.print(F("\",\"batterystate\":\""));
    delay(50);
   SIM900.print(voltaje_batt,2);
   delay(50);
   SIM900.print(F("\",\"kpiunitmeasure\":\"corr\"}]"));
   delay(50);
       TIME_JSON();
        SIM900.write(0x1A);
           
            delay(15000);

  SIM900.println(F("AT+CIPCLOSE"));                                                
  delay(4000);
 
                
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
                                                                  Puxitron=50000; 
                                                                                     }
                                        
                                                              else {
                                                                  Puxitron=INT_TIMER16*60000;
                                                                                 }

                             /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                                  EEPROM_DEFAULT= EEPROM.read(6);                                      
                                  if (EEPROM_DEFAULT != INT_TIMER16) {         ///UNICAMENTE ESCRIBE EN LA EEPROM SI EL VALOR CAMBIO DE LA WEB 
                                        EEPROM.write(6,INT_TIMER16);
                                    Puxitron= EEPROM.read(6);   

                                                                       
                                                                       } 
                            //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////                                                

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////                                      
                                                                           
                                                                                                                 }//IF PRINCIPAL 
}

void RESPUESTA_SIM900()                       // FUNCION QUE LIMPIA TODO EL STRING QUE GUARDA LAS VARIABLES DE RX
{
  for (int i=0; i<80;i++) {
             FECHA_[i]=BUFFER_USART2[i];
                            }
              
             FECHA_SIM=FECHA_;
  
}
///////////////////////////////////////////////////////////////////////////////////////////
void TIME_JSON()                              // FUNCION QUE LIMPIA TODO EL STRING QUE GUARDA LAS VARIABLES DE RX
{
  COPY_TIME=1;
      ZZ=-1;
       FREQ_CHECK=1;

}


void COPIA_BUFFER(int P,int W)                       // FUNCION QUE LIMPIA TODO EL STRING QUE GUARDA LAS VARIABLES DE RX
{
  //K=0;
  COPY=1;                                 
    X=-1;
      Z=-1;
      for (int i=0; i<99;i++)
        {BUFFER_USART2[i]=NULL;}                  //CADA POSICION DEL STRING LE PONE CARACTER NULL 00
          I=P; 
            D=W;
 //return K;
}

void LIMPIA_BUFFER()                       // FUNCION QUE LIMPIA TODO EL STRING QUE GUARDA LAS VARIABLES DE RX
{
  COPY=0;
    X=0;
      Z=0;
        I=0;
          D=0;
}

void FECHA_LOCAL()                       // FUNCION QUE LIMPIA TODO EL STRING QUE GUARDA LAS VARIABLES DE RX
{
  //COPY=0;
  //X=0;
  //Z=0;
  //K=0;
  for (int i=0; i<22;i++) {
             FECHA_[i] =BUFFER_USART2[i];
                            }
              
             FECHA_SIM=FECHA_;
  
}
void TEMPERATURA()                       // FUNCION QUE 
{


  sensors.requestTemperatures(); // Send the command to get temperatures
  data=sensors.getTempCByIndex(0);
  //SIM900.println(data,1);

}


void SIM900_ON()                       // FUNCION QUE LIMPIA TODO EL STRING QUE GUARDA LAS VARIABLES DE RX
{
  digitalWrite(5, HIGH);
  delay(2000);//                                     //ENCIENDE EL SIM900
  digitalWrite(5, LOW);
  delay(2000);
  
}

void SIM900_OFF()                       // FUNCION QUE LIMPIA TODO EL STRING QUE GUARDA LAS VARIABLES DE RX
{
  digitalWrite(5, HIGH);
  delay(2000);//                                     //ENCIENDE EL SIM900
  digitalWrite(5, LOW);
  delay(2000);

}


void CHECK_COMPANIA_CHIP()                       // 
{
  for(int sim=0;sim<10;sim++){
     if(BUFFER_USART2[sim] != 34){
       COMPANIA[sim]=BUFFER_USART2[sim];
                                          }                            
   
                                                }
          CHIP=COMPANIA;                             //COPIA EL CONTENIDO DEL ARRAY CHAR COMPANIA EN EL STRING CHIP PARA DEJARLO COMO UN OBJETO Y MANIPULARLO DE
                                                     //MUCHAS FORMAS
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SETUP_SIM900()                                                 // FUNCION QUE 
{

//SIM900.println(F("AT+CBAND=\"PCS_MODE\""));
//  delay(1000);
   
  delay(20000);                                                    //BUSCANDO SEÑAL 

  SIM900.println(F("AT+CGSN"));                                        //TESTEA EL NIVEL DE LA SEÑAL  
  delay(1000);

  
  COPIA_BUFFER(23,35);
  SIM900.println(F("AT+COPS?"));
  delay(400);
  CHECK_COMPANIA_CHIP();
  LIMPIA_BUFFER();   

  delay(15000);    
    //if(CHIP == "Iusacell"){
      //SIM900.println("AT+CBAND=\"GSM850_MODE\"");                   //CONFIGURACION DE LA BANDA 850MHZ IUSACELL 2G
        //delay(1000);   
          //                        }
          //else{
            //  SIM900.println("AT+CBAND=\"PCS_MODE\"");              //CONFIGURACION DE LA BANDA 1900MHZ TELCEL 2G
              //      delay(1000);   
                //                        }   
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  SIM900.println(F("AT+CSQ"));                                        //TESTEA EL NIVEL DE LA SEÑAL  
  delay(1000);
    
    
  for(int i = 0;i<3;i++){
    SIM900.println(F("AT+CGATT?"));                                   //VERIFICA CONEXION  DEL SERVICIO GPRS
      delay(1000); 
                                            } 
  
  SIM900.println(F("AT+CBAND?"));                                     //VERIFICA EN QUE BANDA OPERA 850MHZ O 1900MHZ
  delay(400); 
  
  SIM900.println(F("AT+CIPMODE=0"));                                  //MODO 0 ES NORMAL,MODO 1 ES TRANSPARENTE (0=ENVIA PURO ASCII 
  delay(400);                                                         //AL SERVIDOR WEB,1=ENVIA PURO HEXADECIMAL AL SERVIDOR WEB)
    
  //SIM900.println("AT+CBAND=\"PCS_MODE\"");
  //delay(1000);
  
  //SIM900.println("AT+CBAND=\"GSM850_PCS_MODE\"");
  //delay(1000);
 
  //SIM900.println("AT+CBAND=\"GSM850_MODE\"");
  //delay(1000);  
  
   //SIM900.println("AT+CBAND=\"ALL_BAND\"");
   //delay(1000);
  
 ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
/*  SIM900.println(F("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\""));                                    
  delay(1000); 
  
  if(CHIP == "Iusacell"){
      SIM900.println(APN_IUSACELL_1);                                //CONFIGURACION APN DE TELCEL
      delay(1000); 
      SIM900.println(APN_IUSACELL_2);                                //CONFIGURACION APN DE TELCEL
      delay(1000); 
      SIM900.println(APN_IUSACELL_3);                                //CONFIGURACION APN DE TELCEL
      delay(1000); 
                                  }
          else{
              SIM900.println(APN_TELCEL_1);                          //CONFIGURACION APN DE IUSACELL
              delay(1000);  
              SIM900.println(APN_TELCEL_2);                          //CONFIGURACION APN DE IUSACELL
              delay(1000);  
              SIM900.println(APN_TELCEL_3);                          //CONFIGURACION APN DE IUSACELL
               delay(1000);   
                                        }  

  delay(5000);   
  

  SIM900.println(F("AT+SAPBR=1,1"));                                     
  delay(1000); 
  
  SIM900.println(F("AT+SAPBR=2,1"));                                    
  delay(1000); 
  
  SIM900.println(F("AT+SAPBR=0,1"));                                     
  delay(4000); 
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////                                                    
  
  SIM900.println(F("AT+CIPMUX=0"));
  delay(2000); 
  
  if(CHIP == "Iusacell"){
      SIM900.println(VPN_IUSACELL);                               //CONFIGURACION VPN DE TELCEL
        delay(1000);   
                                  }
          else{
              SIM900.println(VPN_TELCEL);                          //CONFIGURACION VPN DE IUSACELL
                    delay(1000);   
                                        }                              
  
  COPIA_BUFFER(1,70); 
  SIM900.println(F("AT+CIICR"));                                    //ENCIENDE EL RADIO 
    delay(4000); 
                  if (strcasestr(BUFFER_USART2, "ERROR")) {           //BUSCA ERROR EN EL ARRAY CHAR DEL SERIAL SI LO ENCUENTRA APAGA EL SIM900 
  
                               //SIM900_OFF(); 
                               //soft_restart ();                         
                               
                               //MCUSR=0x00;              //LIMPIA TOADAS LAS BANDERAS DEL REGISTRO    
                               //wdt_enable(WDTO_15MS); 
                               //for(;;) { 
              
                                               // }
                                                                  }    
  LIMPIA_BUFFER();
  
  
  SIM900.println(F("AT+CIFSR"));                                    //MUESTRA LA IP 
  delay(4000);  


}


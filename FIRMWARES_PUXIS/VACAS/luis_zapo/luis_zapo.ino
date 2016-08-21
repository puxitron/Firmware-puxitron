#include <avr/wdt.h>
#include <OneWire.h> 
#include <DallasTemperature.h>
#include <SoftwareSerial.h>
 


SoftwareSerial SIM900(15,16);          //(RX,TX)
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

char   GPS [42];                                     //COORDENADAS
String GPS_JSON;

char   FECHA_[25];
String FECHA_SIM;


String RESPUESTA_SIM;

char   ADC_SIM  [5]; 
float  ADC_PROMEDIO;


unsigned long int ADC_SIM900; 
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 int     X=0;                                                                        
 int     I;
 int     D;
 int     P;                                          //REGISTROS DEL SERIAL
 int     Z;
 int     COPY;
 char    BUFFER_USART;
 char    BUFFER_USART2[100];
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
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 

  pinMode(17, OUTPUT);                               //ADC 3
  pinMode(16, OUTPUT);                              //tx sower                             
  pinMode(15, INPUT); 
  pinMode(14, INPUT);                               //ADC 0
  

  pinMode(10,OUTPUT);    
  pinMode(9, OUTPUT);                              //CONFIGURACION I/O PINES
  pinMode(6, INPUT);                               //SELECCIONA LA RED EN LA QUE VA OPERAR
  pinMode(5, OUTPUT);                              //ACTIVA EL SIM900 
  pinMode(4, INPUT);                               // 
 /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////      
  analogReference(DEFAULT);                       //VOLTAJE DE REFERENCIA DEL ADC        
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 

  SIM900_ON(); 

  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
  delay(8000);


                                                 }  
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////                                                 

 void loop() {
   SETUP_SIM900();
   TEMPERATURA();
   ENVIO_WEB();  
   //FTP(); 
   //SIM900_OFF(); 
   delay(30000);
   //SIM900_ON();
   //SETUP_SIM900(); 
                                             
                                              
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
    UDR0 = BUFFER_USART;                // ESCRIBE EN EL BUFFER TX  

} 
////////////////////////////////////////////////////////////////////////


void ENVIO_WEB() 
{
int JSON=162;
int X=0;  
 
  SIM900.println("AT+CIPSTATUS");
  delay(800);
  
  COPIA_BUFFER(18,41);
  SIM900.println("AT+CCLK?");
  delay(2000);
  FECHA_LOCAL();
  LIMPIA_BUFFER();
  
while(X < 6){   
  COPIA_BUFFER(1,70);
  SIM900.println("AT+CIPSTART=\"tcp\",\"sensapp.herokuapp.com\",\"80\"");  
  delay(16000);                   
  LIMPIA_BUFFER();
  X++;                             
                               if (strcasestr(BUFFER_USART2, "CONNECT FAIL")) {           //BUSCA ERROR EN EL ARRAY CHAR DEL SERIAL SI LO ENCUENTRA APAGA EL SIM900 
                               
                               delay(60000);

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
  
  
  SIM900.println("POST /datos/ HTTP/1.1");
  delay(100);
  SIM900.println("Content-Type: application/json");
  delay(100);
  SIM900.println("Host: sensapp.herokuapp.com");
  delay(100);
  SIM900.println("Connection: keep-alive");
  delay(100);
  SIM900.print("Content-Length:");
  delay(100);
  SIM900.println(JSON);
  delay(100);
 
  SIM900.println("");
  delay(100);  
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////  
   SIM900.print("{\"capturedate\":");
   delay(100);
   SIM900.print(FECHA_SIM);
   delay(100);
   SIM900.print(",\"tokenslave\":\"1\",\"tokenmaster\":\"BLUE\"");
   delay(100);
   SIM900.print(",\"kpi\":\"TMP\",\"kpivalue\":");                                          
   delay(100);
   SIM900.print(data,2);
   delay(100);
   SIM900.print(",\"batterystate\":");                                          
   delay(100);
   SIM900.print(data,2);                                          
   delay(100);
   SIM900.print(",\"kpiunitmeasure\":\"Voltaje\",\"gps\":");                                          
   delay(100);
   SIM900.print("\"");                                          
   delay(100);
   //SIM900.print(GPS_JSON);                                          
   delay(100);  
   SIM900.print("\"");                                          
   delay(100);
   SIM900.println("}");                                          
   delay(100);
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////  
  //SIM900.println("");
  //delay(100);
  SIM900.write(0x1A);
  delay(12000);
   
  
  //SIM900.println("");
  //delay(100);   
   
  SIM900.println(F("AT+CIPCLOSE"));                                                
  delay(4000);
  
  
  //SIM900.println(F("AT+CIPSHUT"));                                              
 // delay(4000);
 
  //SIM900.println("");
  //delay(100);
 
  //SIM900.println("");
  //delay(100); 
  
}

void RESPUESTA_SIM900()                       // FUNCION QUE LIMPIA TODO EL STRING QUE GUARDA LAS VARIABLES DE RX
{
  for (int i=0; i<80;i++) {
             FECHA_[i]=BUFFER_USART2[i];
                            }
              
             FECHA_SIM=FECHA_;
  
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

  delay(60000);// 


}

void GPS_()                       // 
{
  for(int x=0;x<40;x++){

       GPS[x]=BUFFER_USART2[x];
                                                                     
   
                                                }
          //CHIP=COMPANIA;                             //COPIA EL CONTENIDO DEL ARRAY CHAR COMPANIA EN EL STRING CHIP PARA DEJARLO COMO UN OBJETO Y MANIPULARLO DE
         GPS_JSON=GPS;                                            //MUCHAS FORMAS
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

SIM900.println(F("AT+CBAND=\"PCS_MODE\""));
  delay(1000);
   
  delay(33000);                                                    //BUSCANDO SEÑAL 
  
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
  delay(4000);
    
    
  for(int i = 0;i<7;i++){
    SIM900.println(F("AT+CGATT?"));                                   //VERIFICA CONEXION  DEL SERVICIO GPRS
      delay(2000); 
                                            } 
  
  SIM900.println(F("AT+CBAND?"));                                     //VERIFICA EN QUE BANDA OPERA 850MHZ O 1900MHZ
  delay(400); 
  
  SIM900.println(F("AT+CIPMODE=0"));                                  //MODO 0 ES NORMAL,MODO 1 ES TRANSPARENTE (0=ENVIA PURO ASCII 
  delay(400);                                                      //AL SERVIDOR WEB,1=ENVIA PURO HEXADECIMAL AL SERVIDOR WEB)
    
  //SIM900.println("AT+CBAND=\"PCS_MODE\"");
  //delay(1000);
  
  //SIM900.println("AT+CBAND=\"GSM850_PCS_MODE\"");
  //delay(1000);
 
  //SIM900.println("AT+CBAND=\"GSM850_MODE\"");
  //delay(1000);  
  
   //SIM900.println("AT+CBAND=\"ALL_BAND\"");
   //delay(1000);
  
 ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
  SIM900.println(F("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\""));                                    
  delay(4000); 
  
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
  delay(4000); 
  
  SIM900.println(F("AT+SAPBR=2,1"));                                    
  delay(4000); 
  
  SIM900.println(F("AT+SAPBR=0,1"));                                     
  delay(4000); 
 
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



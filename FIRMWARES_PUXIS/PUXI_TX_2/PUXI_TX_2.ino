#include <SoftwareSerial.h> //

SoftwareSerial SIM5320(9,15); // RX, TX


  
byte  INIT_EEPROM; 
long int    Puxitron=0; 
int    INT_TIMER16;
char   WEB_TIEMPO[6];
String TIEMPO_INT;
String IMEI;

String GPS_JSON;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int long acumulador=0;
int long SUMA=0;
float    PROM=0;                                     //ADC REGISTROS Y PROMEDIO
float    INTER=0;
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 int     X=0;                                                                        
 int     I;
 int     D;
 int     P;                                          //REGISTROS DEL SERIAL
 int     Z;
 int     COPY;
 int     COPY_TIME;
 int     ZZ=-1;
 char    BUFFER_USART;
 char    BUFFER_USART2[100];
 char    TIME_BUFFER[100];
 int     FLAG;
 int     FREQ_CHECK;
////////////////////////////////////////////////////////////////////////////
void setup() { 

  delay(1000);
////////////////////////////////////////////////////////////////////////////  
  SREG    =0x80;                                    //INT GLOBAL SOLO ACTIVA LA DEL UART
  UCSR0B  =0x98;                                    //ACTIVA TX Y RX A SI COM LA INT DEL RX
  UCSR0C  =0x06;                                    //CONFIGURAMOS A 8BITS DE DATOS SERIAL
  UBRR0H  =0x00;                                    //VALOR QUE SE LE CARGA AL REGISTRO HIGH 0
  UBRR0L  =0x67;                                    //VALOR QUE SE LE CARGA AL REGISTRO LOW 0x67 QUE ES IGUAL A 103 PARA 9600
//////////////////////////////////////////////////////////////////////////////
 SIM5320.begin(9600);

  pinMode(2, OUTPUT);                              //ENCENDIDO DEL SIM5320

//////////////////////////////////////////////////////////////////////////////

  SIM900_ON(); //ENCIENDO EL SIM 
  delay(5000);
  SETUP_SIM5320();
}





void loop() {

GPS();
WEB();

}
///////////////////////////////////////////////////////////////////////////
ISR (USART_RX_vect)
{   
    X++;
    BUFFER_USART = UDR0;                // LEE EL BUFFER RX                    
    
    if(COPY!=0&&X>I+2&&X<D+4){
    Z++;
    BUFFER_USART2[Z]= BUFFER_USART;  
                       
                                   } 
    
   
    
    
    if(FREQ_CHECK == 1&&BUFFER_USART == '$'){
      FLAG=1;
                                                   }
    
                    if(COPY_TIME == 1&&FLAG == 1){
                      ZZ++;
                          TIME_BUFFER[ZZ]= BUFFER_USART;  
                                          if(TIME_BUFFER[ZZ] == '*'){
                                            
                                            COPY_TIME=0;
                                              FLAG=0;
                                                ZZ=-1;
                                                 FREQ_CHECK=0;                       
                                                                       }
                                                                               } 

} 
////////////////////////////////////////////////////////////////////////



 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
void WEB()                                  //  
{
   
int x=0;
int LONG_JSON=36;
int SEND=153+14;
 
  //SIM5320.println("AT+CBC");          //VOLTAJE BATERIA
  //delay(500);

  
  //SIM5320.println("AT+CSQ");          //CALIDAD DE CONSULTA DE SEÑAL
  //delay(500);

  while (x<6) {
  COPIA_BUFFER(1,99);
  SIM5320.println("AT+CIPOPEN=0,\"TCP\",\"taxiup.herokuapp.com\",80");  //vtechph.herokuapp.com
  delay(5000);
  LIMPIA_BUFFER();                                         
  x++;                                               
                                                                                                         

                                                           if (x == 5) {              
                                                           SIM900_ON(); 
                                                           delay(2000); 
                                                            
                                                                                                             }//IF

                                                            if (strcasestr(BUFFER_USART2,"+CIPOPEN: 0,0")) { 
                                                            break;  
                                                                                                              }//IF

                                                            //if (strcasestr(BUFFER_USART2,"+CIPOPEN: 0,4")) { 
                                                            ///break;  
                                                            //                                                  }//IF                                                     
                                                                                                             
                                                                      
                                                                      
                                                                      } //WHILE
                                                            

  
  
   
   
   LONG_JSON+=GPS_JSON.length();
   SEND+=GPS_JSON.length();                              //CALCULA LA LONGITUD DE LO QUE VA ENVIAR DESPUES DEL PROMPT(SEND)
   
   SIM5320.print("AT+CIPSEND=0,");  //
   delay(100);
   SIM5320.println(SEND);  //
   delay(1000);
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////  
   SIM5320.println(F("POST /tracks/ HTTP/1.1"));
   delay(50);
   SIM5320.println(F("Content-Type: application/json"));
   delay(50);
   SIM5320.println(F("Host: taxiup.herokuapp.com"));
   delay(50);
   SIM5320.println(F("Connection: keep-alive"));
   delay(50);
   SIM5320.print(F("Content-Length:"));   
   delay(50);
   SIM5320.println(LONG_JSON);   
   delay(50);
   SIM5320.println("");
///////////////////////////////////////////////////////////////   
   SIM5320.print(F("{\"Imei\":\""));
   delay(50);
   SIM5320.print(IMEI);
   delay(50);
   SIM5320.print(F("\",\"nmea\":\""));
   delay(50);
   SIM5320.print(GPS_JSON);
   delay(50);
   SIM5320.println(F("\"}"));
   delay(50);
   SIM5320.write(0x1A);
   delay(8000);   
   //{"Imei":"012813006323523","nmea":""}                         //36 base json
         
//////////////////////////////////////////////////////////////  
   SIM5320.println(F("AT+CIPCLOSE=0"));                                          
   delay(2000); 
         
 } 

void SIM900_ON()                                  //  
{
  digitalWrite(2, HIGH);
  delay(2000);//                                     //ENCIENDE EL SIM5320
  digitalWrite(2, LOW);
  delay(2000);
 } 
////////////////////
void GPS() {
 
   TIME_JSON();
   SIM5320.println(F("AT+CGPSINFOCFG=1,1"));
    delay(666);
      GPS_JSON=TIME_BUFFER;
          //delay(10000);
            SIM5320.println(F("AT+CGPSINFOCFG=0,1"));

      
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
       FREQ_CHECK=1;

              }
/////////////////////////////////////////////////////////////////////////////
void SETUP_SIM5320()                       // FUNCION QUE LIMPIA TODO EL STRING QUE GUARDA LAS VARIABLES DE RX
{
   
  SIM5320.println(F("AT+CFUN=1"));    //
  delay(250);

  SIM5320.println(F("AT+CVAUXV=61"));    //GPS APGADO PARA NO GASTAR PILA
  delay(250);  
  
  SIM5320.println(F("AT+CGPSCOLD"));    //GPS APGADO PARA NO GASTAR PILA
  delay(500);
 
  SIM5320.println(F("AT+CNBP=0x0000000004000000"));  //3G EN 850MHZ EN TELCEL
  delay(500);
  
  SIM5320.println(F("AT+CNMP=2"));  //
  delay(250);
  
  //SIM5320.println(F("AT+IPREX=9600"));  //
  //delay(4000);

  //SIM5320.println(F("AT+CIPTIMEOUT=30000,30000,30000"));  //PREGUNTO LA CALIDAD DE LA SEÑAL 
  //delay(500);

  delay(10000);
  SIM5320.println(F("AT+CSQ"));  //PREGUNTO LA CALIDAD DE LA SEÑAL 
  delay(250);

  SIM5320.println(F("AT+CREG?"));  //REGISTRO DE RED, ME TIENE QUE DAR UN 1
  delay(250);
  
  SIM5320.println(F("AT+COPS?"));  //SELECCION DE OPERADOR
  delay(250);
  //////////////////////////////////
  //SIM5320.println(F("AT+CGSOCKCONT=1,\"IP\",\"internet.itelcel.com\""));  //DEFINO EL ENCHUFE QUE EN ESTE CASO ES TELCEL
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
  delay(10000);

  SIM5320.println(F("AT+IPADDR"));  //CONSULTA LA DIRECCION IP LOCAL
  delay(500);


   COPIA_BUFFER(8,22);
   SIM5320.println(F("AT+CGSN"));  //IMEI
   delay(500);
   LIMPIA_BUFFER();
   IMEI=BUFFER_USART2;

}


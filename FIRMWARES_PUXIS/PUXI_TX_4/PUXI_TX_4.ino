#include <SoftwareSerial.h> //

SoftwareSerial SIM5320(3,4); // RX, TX


  
byte  INIT_EEPROM; 
long int    Puxitron=0; 
int    INT_TIMER16;
char   WEB_TIEMPO[6];

String TIEMPO_INT;
String IMEI;
String GPS_JSON;
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 int     X;                                                                        
 int     I;
 int     D;
 int     P;                                          //REGISTROS DEL SERIAL
 int     Z;
 int     COPY; 
 char    BUFFER_USART2[100];  
 char    BUFFER_USART; 
 ///////////////////////////////                  
 int     FLAG_GPS;
 int     GPS_ZZ=-1;  
 int     GPS_USART;
 char    GPS_BUFFER[80];
 ///////////////////////////////
 int     FLAG_RESPONSE;
 int     RESPONSE_ZZ=-1;
 int     RESPONSE;                  
 char    RESPONSE_BUFFER[80];                             
////////////////////////////////////////////////////////////////////////////
void setup() { 

  delay(1000);
////////////////////////////////////////////////////////////////////////////  
  SREG    =0x80;                                    //INT GLOBAL SOLO ACTIVA LA DEL UART
  UCSR0B  =0x98;                                    //ACTIVA TX Y RX A SI COM LA INT DEL RX
  UCSR0C  =0x06;                                    //CONFIGURAMOS A 8BITS DE DATOS SERIAL
  UBRR0H  =0x00;                                    //VALOR QUE SE LE CARGA AL REGISTRO HIGH 0
  UBRR0L  =0x67;                                    //VALOR QUE SE LE CARGA AL REGISTRO LOW 0x67 QUE ES IGUAL A 103 PARA 9600 08 PARA 115200
//////////////////////////////////////////////////////////////////////////////
 SIM5320.begin(9600);

  pinMode(5, OUTPUT);                              //ENCENDIDO DEL SIM5320
  //pinMode(4, OUTPUT);                              //ENCENDIDO DEL SIM5320
  //pinMode(3, INPUT);                              //ENCENDIDO DEL SIM5320
  pinMode(1, OUTPUT);                              //ENCENDIDO DEL SIM5320
  pinMode(0, INPUT);                              //ENCENDIDO DEL SIM5320

//////////////////////////////////////////////////////////////////////////////

  SIM5320_ON(); //ENCIENDO EL SIM 
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
      BUFFER_USART = UDR0;                              // LEE EL BUFFER RX                    
    
        if(COPY == 1&&X >= I&&X <= D){
            Z++;
               BUFFER_USART2[Z]= BUFFER_USART;  

                                               if(X == D){
                                                        COPY=0;
                                                           X=0;
                                                             Z=-1;
                                                                I=0;
                                                                   D=0;
                                                                     }

                                             
                                              
                                                  } //*/
    
   
//////////////////////////////////////////////////////////////////////////////////////////       
    
   if(GPS_USART == 1&&BUFFER_USART == '$'){
      FLAG_GPS=1;
                                               }     
    
                    if(FLAG_GPS == 1){
                      GPS_ZZ++;
                          GPS_BUFFER[GPS_ZZ]= BUFFER_USART;  
                                          if(GPS_BUFFER[GPS_ZZ] == '*'){
                                            
                                              FLAG_GPS=0;
                                                GPS_ZZ=-1;
                                                 GPS_USART=0;                       
                                                                          }
                                                                               

                                                                                          }//*/

//////////////////////////////////////////////////////////////////////////////////////////     
     if(RESPONSE == 1&&BUFFER_USART == '{'){
      FLAG_RESPONSE=1;
                                               }     
    
                    if(FLAG_RESPONSE == 1){
                      RESPONSE_ZZ++;
                          RESPONSE_BUFFER[RESPONSE_ZZ]= BUFFER_USART;  
                                                   if(RESPONSE_BUFFER[RESPONSE_ZZ] == '}'){                                            
                                                     
                                                        FLAG_RESPONSE=0;
                                                          RESPONSE_ZZ=-1;
                                                            RESPONSE=0;                       
                                                                               }
                                                                               

                                                                                            }//*/
                                                                                          
} 
//////////////////////////////////////////////////////////////////////////////////////////



 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
void WEB()                                  //  
{
int CLOSE=0; 
int RESPONSE_WHILE=0;   
int TCP=0;

int LONG_JSON=36;
int SEND=153+14;
 

  COPIA_USART(40,95);
  SIM5320.println(F("AT+CIPOPEN=0,\"TCP\",\"taxiup.herokuapp.com\",80"));  //taxiup.herokuapp.com
  while (TCP<66) {
  TCP++;                                               
  delay(600);                                                                                                       

                                                            if (strcasestr(BUFFER_USART2,"+CIPOPEN: 0,0")) { 
                                                            break;  
                                                                                                              }//IF

                                                            if (strcasestr(BUFFER_USART2,"+CIPOPEN: 0,11")) { 
                                                            SIM5320_ON();  
                                                            delay(1500);  
                                                              asm volatile ( "jmp 0");  
                                                                                                               }//IF                                                     
                                                                                                             
                                                                      
                                                                      
                                                                      } //WHILE*/
                                                            


  
   
   
   LONG_JSON+=GPS_JSON.length();
   SEND+=GPS_JSON.length();                              //CALCULA LA LONGITUD DE LO QUE VA ENVIAR DESPUES DEL PROMPT(SEND)
   
   SIM5320.print("AT+CIPSEND=0,");  //
   delay(50);
   SIM5320.println(SEND);  //
   delay(600);
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////  
   SIM5320.println(F("POST /tracks/ HTTP/1.1"));
   delay(10);
   SIM5320.println(F("Content-Type: application/json"));
   delay(10);
   SIM5320.println(F("Host: taxiup.herokuapp.com"));
   delay(10);
   SIM5320.println(F("Connection: keep-alive"));
   delay(10);
   SIM5320.print(F("Content-Length:"));   
   delay(10);
   SIM5320.println(LONG_JSON);   
   delay(10);
   SIM5320.println("");
///////////////////////////////////////////////////////////////   
   SIM5320.print(F("{\"Imei\":\""));
   delay(10);
   SIM5320.print(IMEI);
   delay(10);
   SIM5320.print(F("\",\"nmea\":\""));
   delay(10);
   SIM5320.print(GPS_JSON);
   delay(10);
   SIM5320.println(F("\"}"));
   delay(10);
   SIM5320.write(0x1A);  
   RESPONSE_WEB();
   //delay(5000);   
   //{"Imei":"012813006323523","nmea":""}                         //36 base json
   while (RESPONSE_WHILE<33) {
   RESPONSE_WHILE++;                                               
   delay(500);                                                                                                       

                                                            if (strcasestr(RESPONSE_BUFFER,"403: Forbidden")) { 
                                                            break;  
                                                                                                              }//IF


                                                            if (strcasestr(RESPONSE_BUFFER,"200: OK")) { 
                                                            break;  
                                                                                                              }//IF
                                                  
                                                            if (RESPONSE_WHILE == 65) { 
                                                            RESPONSE=0;
                                                                                                              }//IF                                                                                                  
                                                                      
                                                                      
                                                                                                                        } //WHILE*/





         
//////////////////////////////////////////////////////////////  
   COPIA_USART(1,40);
   SIM5320.println(F("AT+CIPCLOSE=0"));                                          
   while (CLOSE<50) {
   CLOSE++;                                               
   delay(500);                                                                                                       

                                                            if (strcasestr(BUFFER_USART2,"+CIPCLOSE: 0,0")) { 
                                                            break;  
                                                                                                              }//IF

                                                                                                              
                                                            if (strcasestr(BUFFER_USART2,"+CIPCLOSE: 0,4")) { 
                                                            break;  
                                                                                                              }//IF                                                                                                     
                                                                      
                                                                      
                                                                                                                        } //WHILE*/
         
 } 

void SIM5320_ON()                                  //  
{
  digitalWrite(5, HIGH);
  delay(2000);//                                     //ENCIENDE EL SIM5320
  digitalWrite(5, LOW);
  delay(2000);
 } 
////////////////////
void GPS() {
 
   COPIA_GPS();
   SIM5320.println(F("AT+CGPSINFOCFG=1,1"));
    delay(800);
      GPS_JSON=GPS_BUFFER;
            SIM5320.println(F("AT+CGPSINFOCFG=0,1"));
               delay(500);

      
}
////////////////////////////////////////////////////////////
void COPIA_USART(const int P,const int W)                       // FUNCION QUE LIMPIA TODO EL STRING QUE GUARDA LAS VARIABLES DE RX
{

  COPY=1;                                 
      X=0;
      Z=-1;
      
      for (int i=0; i<99;i++)
        {BUFFER_USART2[i]=NULL;}                  //CADA POSICION DEL STRING LE PONE CARACTER NULL 00
          
          I=P; 
            D=W;

}

void COPIA_GPS()                              // FUNCION QUE LIMPIA TODO EL STRING QUE GUARDA LAS VARIABLES DE RX
{
 
  GPS_USART=1;
             for (int i=0; i<79;i++){
                 GPS_BUFFER[i]=NULL;}                  //CADA POSICION DEL STRING LE PONE CARACTER NULL 00

              }

void RESPONSE_WEB()                              // FUNCION QUE LIMPIA TODO EL STRING QUE GUARDA LAS VARIABLES DE RX
{
 
  RESPONSE=1;
             for (int i=0; i<79;i++){
                 RESPONSE_BUFFER[i]=NULL;}                  //CADA POSICION DEL STRING LE PONE CARACTER NULL 00

              }              
/////////////////////////////////////////////////////////////////////////////
void SETUP_SIM5320()                       // FUNCION QUE LIMPIA TODO EL STRING QUE GUARDA LAS VARIABLES DE RX
{
  int NETOPEN=0;
   
  SIM5320.println(F("AT+CFUN=1"));    //
  delay(250);

  SIM5320.println(F("AT+CVAUXV=61"));    //GPS APGADO PARA NO GASTAR PILA
  delay(250);  
  
  SIM5320.println(F("AT+CGPSCOLD"));    //GPS 
  delay(500);
 
  SIM5320.println(F("AT+CNBP=0x0000000004000000"));  //3G EN 850MHZ EN TELCEL
  delay(500);
  
  SIM5320.println(F("AT+CNMP=2"));  //
  delay(250);
  
  //SIM5320.println(F("AT+IPREX=9600"));  //
  //delay(4000);

  SIM5320.println(F("AT+CIPTIMEOUT=30000,30000,30000"));  //PREGUNTO LA CALIDAD DE LA SEÑAL 
  delay(500);

  delay(6600);
  SIM5320.println(F("AT+CSQ"));  //PREGUNTO LA CALIDAD DE LA SEÑAL 
  delay(250);

  SIM5320.println(F("AT+CREG?"));  //REGISTRO DE RED, ME TIENE QUE DAR UN 1
  delay(250);
  
  SIM5320.println(F("AT+COPS?"));  //SELECCION DE OPERADOR
  delay(250);
  //////////////////////////////////

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

  COPIA_USART(20,60);
  SIM5320.println(F("AT+NETOPEN")); //ACTIVE PDP CONTEXT PROFILE NUMBER
  while (NETOPEN<50) { 
  NETOPEN++;                                               
  delay(500);                                                                                                       

                                                            if (strcasestr(BUFFER_USART2,"+NETOPEN: 0")) { 
                                                            break;  
                                                                                                              }//IF
                                                                                                   
                                                                      
                                                                      
                                                                                                                        } //WHILE*/

  
  
  
  SIM5320.println(F("AT+IPADDR"));  //CONSULTA LA DIRECCION IP LOCAL
  delay(500);


   COPIA_USART(11,25);
   SIM5320.println(F("AT+CGSN"));  //IMEI
   delay(500);
   IMEI=BUFFER_USART2;

}


#include <SoftwareSerial.h>
 

SoftwareSerial SIM800(4,15); // RX, TX
//SoftwareSerial NMEA(14, 6);  // RX, TX

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
 ///////////////////////////////                  
 int     FLAG_GPS;
 int     GPS_ZZ=-1;  
 int     GPS_USART;
 char    GPS_BUFFER[80];
 ///////////////////////////////
///////////////////////////////
int     FLAG_RESPONSE;
int     RESPONSE_ZZ = -1;
int     RESPONSE;
char    RESPONSE_BUFFER[80];
/////////////////////////////////

void setup() {
  ////////////////////////////////////////////////////////////////////////////  
  SREG    =0x80;                                    //INT GLOBAL SOLO ACTIVA LA DEL UART
  UCSR0B  =0x98;                                    //ACTIVA TX Y RX A SI COM LA INT DEL RX
  UCSR0C  =0x06;                                    //CONFIGURAMOS A 8BITS DE DATOS SERIAL
  UBRR0H  =0x00;                                    //VALOR QUE SE LE CARGA AL REGISTRO HIGH 0
  UBRR0L  =0x67;                                    //VALOR QUE SE LE CARGA AL REGISTRO LOW 0x67 QUE ES IGUAL A 103 PARA 9600 08 PARA 115200
//////////////////////////////////////////////////////////////////////////////  
  //pinMode(RX1, INPUT);
  //pinMode(14, INPUT);
  
  pinMode(2,  OUTPUT);                               //
  digitalWrite(2, LOW);
  delay(2000);
  digitalWrite(2, HIGH);
  delay(6000);
  
 // Serial.begin(9600);
   
    NMEA.begin(9600);
    SIM800.begin(9600);

  SIM800.println("AT");                             //
  delay(10000);

  GPS_USART = 1;
  SETUP_800();
  
}

void loop() {












      NMEA.listen();
      delay(10);    
     
  for (int i = 0; i <20000; i++) {     
      while(NMEA.available () > 0){
      BYTE_LEIDO_BUFFER_SERIAL (NMEA.read ());

                                                              }


                                                               }

     SIM800.listen();
         //delay(500); 
         TCP();


}
void TCP() {
  int CLOSE = 0;
  int RESPONSE_WHILE = 0;
  int TCP = 0;

  int LONG_JSON = 36;
  //int SEND = 153 + 14;

 
  COPIA_USART(40, 95);
  SIM800.println(F("AT+CIPSTART=\"tcp\",\"taxiup.herokuapp.com\",80"));                     //
  while (TCP <66) {
  TCP++;
  delay(600);

                                                       if (strcasestr(BUFFER_USART2, "CONNECT OK")) {
                                                       break;
                                                                                 }//IF

                                                       if (strcasestr(BUFFER_USART2, "CONNECT FAIL")) {

                                                        delay(1000);
                                                        asm volatile ( "jmp 0");
                                                        }//IF


                                                       if (strcasestr(BUFFER_USART2, "ERROR")) {

                                                        delay(1000);
                                                        asm volatile ( "jmp 0");
                                                        }//IF



                                                                                                     } //WHILE*/


   LONG_JSON += GPS_JSON.length();
  //SEND+=GPS_JSON.length();                            //CALCULA LA LONGITUD DE LO QUE VA ENVIAR DESPUES DEL PROMPT(SEND)

  COPIA_USART(1, 70);
  SIM800.println("AT+CIPSEND");
  delay(500);

                                                   if (strcasestr(BUFFER_USART2, "ERROR")) {           //BUSCA ERROR EN EL ARRAY CHAR DEL SERIAL SI LO ENCUENTRA APAGA EL SIM900
                                                   asm volatile ( "jmp 0");
                                                                                                  }
   SIM800.println(F("POST /tracks/ HTTP/1.1"));
   delay(10);
   SIM800.println(F("Content-Type: application/json"));
   delay(10);
   SIM800.println(F("Host: taxiup.herokuapp.com"));
   delay(10);
   SIM800.println(F("Connection: keep-alive"));
   delay(10);
   SIM800.print(F("Content-Length:"));   
   delay(10);
   SIM800.println(LONG_JSON);   
   delay(10);
   SIM800.println("");

   ///////////////////////////////////////////////////////////////   
   SIM800.print(F("{\"Imei\":\"012813008526503"));
   delay(10);
   //SIM800.print(IMEI);
   delay(10);
   SIM800.print(F("\",\"nmea\":\""));
   delay(10);
   SIM800.print(GPS_JSON);
   delay(10);
   SIM800.println(F("\"}"));
   delay(10);
   SIM800.write(0x1A); 
   RESPONSE_WEB();  
   while (RESPONSE_WHILE<33) {
   RESPONSE_WHILE++;                                               
   delay(500);                                                                                                       

                                                            if (strcasestr(RESPONSE_BUFFER,"403: Forbidden")) { 
                                                            break;  
                                                                                                              }//IF


                                                            if (strcasestr(RESPONSE_BUFFER,"200: OK")) { 
                                                            break;  
                                                                                                              }//IF
                                                  
                                                            if (RESPONSE_WHILE == 30) { 
                                                            RESPONSE=0;
                                                                                                               }//IF                                                                                                  
                                                                      
                                                                      
                                                                                                                        } //WHILE*/
   
   
   
   COPIA_USART(1,40);
   SIM800.println(F("AT+CIPCLOSE"));                                                
   while (CLOSE<15) {
   CLOSE++;                                               
   delay(500);                                                                                                       

                                                            if (strcasestr(BUFFER_USART2,"CLOSE OK")) { 
                                                            break;  
                                                                                                              }//IF

                                                            if (strcasestr(BUFFER_USART2,"ERROR")) { 
                                                            break;  
                                                                                                              }//IF                                                   

                                                            //if (CLOSE== 14) { 
                                                            //COPY=0;
                                                                                                              // }//IF                                                    

                                                                                                                          } //WHILE*/

} 

////////////////////////////////////////////////////////
void SETUP_800() {

  SIM800.println("AT+CSQ");                                            //
  delay(500);

  SIM800.println("AT+COPS?");                                          //
  delay(500);

    //SIM800.println("AT+CBAND=PCS_MODE");                                         //
  //delay(2000);

  SIM800.println("AT+CBAND?");                                         //
  delay(500);

  SIM800.println(F("AT+CIPMUX=0"));                //
  delay(500);

  SIM800.println(F("AT+CSTT=\"internet.itelcel.com\",\"webgprs\",\"webgprs2003\""));                 //
  delay(500);

  SIM800.println(F("AT+CIICR"));                      //
  delay(6000);

  SIM800.println(F("AT+CIFSR"));                     //
  delay(1000); //

  COPIA_USART(10,24);
  SIM800.println(F("AT+GSN"));                     //
  delay(1000); //

  IMEI=BUFFER_USART2;

}
///////////////////////////////////////////////////////
void BYTE_LEIDO_BUFFER_SERIAL (const byte BYTE_SERIAL){

  const unsigned int LIMITE_BUFFER = 90;
  static char BUFFER_WIFI_INDEX [90];
  static unsigned int POSICION_ARRAY =0;

                                         
                                          
                                          switch (BYTE_SERIAL){

                                          
                                          case '\n':                                   //VERIFICO SI ES NUEVA LINEA
                                          BUFFER_WIFI_INDEX [POSICION_ARRAY] = 0;      //PONE A CERO LA ULTIMA POS QUE CORRESPONDE AL NEW LINEA
                                          //IMPRIME_DATO (GPS_BUFFER);
                                         // Serial.println (GPS_JSON);
                                          POSICION_ARRAY = 0;                          //RESETEA EL CONTADOR PARA EL COPIADO AL ARRAY
                                              // if (strcasestr(BUFFER_WIFI_INDEX,"$GPGGA")) {   
                                                   //         GPS_JSON=BUFFER_WIFI_INDEX;
                                                                                            // }
                                          //Serial.println (GPS_JSON);
                                          //GPS_JSON="";
                                                                                for (int i = 0; i < 79; i++) {
                                                                                 BUFFER_WIFI_INDEX[i] = NULL;
                                                                                                             } //CADA POSICION DEL STRING LE PONE CARACTER NULL 00
                                          ////////////////////////////////////////////////////////////////////////////////  
                                          break;

                                          
                                          case '\r':   
                                          break;

                                          
                                          
                                          
                                          default:       //SI ES CUALQUIER CARACTER LO ESCRIBER EN EL BUFFER INDEXY EN LA POSICION INDICADA
                                          if (POSICION_ARRAY < (LIMITE_BUFFER - 1)){
                                          
                                                    if(GPS_USART == 1&&BYTE_SERIAL == '$'){
                                                                          FLAG_GPS=1;
                                                                                                    }

                                                                                              if(FLAG_GPS == 1){
                                                                                                   GPS_ZZ++;
                                                                                                    GPS_BUFFER[GPS_ZZ]= BYTE_SERIAL;  
                                                                                                               if(GPS_BUFFER[GPS_ZZ] == '*'){
                                            
                                                                                                                    FLAG_GPS=0;
                                                                                                                             GPS_ZZ=-1;
                                                                                                                              //GPS_USART=0; 
                                                                                                                             
                                                                 if (strcasestr(GPS_BUFFER,"$GPGGA")) {   
                                                                         GPS_JSON=GPS_BUFFER;  
                                                                                                                  }
                              
                                                                                                                                      for (int i = 0; i < 79; i++) {
                                                                                                                                           GPS_BUFFER[i] = NULL;
                                                                                                                                                                         } //CADA POSICION DEL STRING LE PONE CARACTER NULL 00               
                                                                                                                                                        }
                                                                               

                                                                                                                                                         }//*/
                                          
                                          
                                       //  BUFFER_WIFI_INDEX [POSICION_ARRAY++] = BYTE_SERIAL;
                                          break;
                                          
                                                                                          }//IF                                                
                                                                                       

                                                                                                                       }//SWITCH  
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////                                                                                            
}// */





///////////////////////////////////////////////////////////////////////////
ISR (USART_RX_vect)
{
  X++;
  BUFFER_USART = UDR0;                              // LEE EL BUFFER RX

                  if (COPY == 1 && X >= I && X <= D) {
                            Z++;
                                BUFFER_USART2[Z] = BUFFER_USART;

                                             if (X == D) {
                                                 COPY = 0;
                                                        X = 0;
                                                             Z = -1;
                                                                      I = 0;
                                                                            D = 0;
                                                                                        }



                                                                                                          }
//////////////////////////////////////////////////////////////////////////////////////////
  if (RESPONSE == 1 && BUFFER_USART == '{') {
                 FLAG_RESPONSE = 1;
                                                  }

                                                            if (FLAG_RESPONSE == 1) {
                                                                 RESPONSE_ZZ++;
                                                                    RESPONSE_BUFFER[RESPONSE_ZZ] = BUFFER_USART;
                                                                        if (RESPONSE_BUFFER[RESPONSE_ZZ] == '}') {

                                                                                     FLAG_RESPONSE = 0;
                                                                                          RESPONSE_ZZ = -1;
                                                                                                    RESPONSE = 0;
                                                                                                              }


  }

  //UDR0 = BUFFER_USART;                            // LEE EL BUFFER RX

}//
//////////////////////////////////////////////////////////////////////////////////////////
void COPIA_USART(const int P, const int W)                      // FUNCION QUE LIMPIA TODO EL STRING QUE GUARDA LAS VARIABLES DE RX
{

  COPY = 1;
  X = 0;
  Z = -1;

  for (int i = 0; i < 99; i++)
  {
    BUFFER_USART2[i] = NULL; //CADA POSICION DEL STRING LE PONE CARACTER NULL 00
  }

  I = P;
  D = W;

}

void RESPONSE_WEB()                              // FUNCION QUE LIMPIA TODO EL STRING QUE GUARDA LAS VARIABLES DE RX
{

  RESPONSE = 1;
  for (int i = 0; i < 79; i++) {
    RESPONSE_BUFFER[i] = NULL;
  }                  //CADA POSICION DEL STRING LE PONE CARACTER NULL 00

}
/////////////////////////////////////////////////////////////////////////////
/*void BYTE_LEIDO_BUFFER_SERIAL (const byte BYTE_SERIAL){

  int x=15;
  char FIN=' ';
  int i=-1;
  static char BUFFER_WIFI_INDEX [80];
  static unsigned int POSICION_ARRAY =0;

                                         
                                          
                                          switch (BYTE_SERIAL){

                                          case '\n':                                  //VERIFICO SI ES NUEVA LINEA
                               
                                          INDEX=BUFFER_WIFI_INDEX;
                                          BUFFER_WIFI_INDEX [POSICION_ARRAY] = 0;    //PONE A CERO LA ULTIMA POS QUE CORRESPONDE AL NEW LINEA
                                          IMPRIME_DATO (BUFFER_WIFI_INDEX);
                                          POSICION_ARRAY = 0;                        //RESETEA EL CONTADOR PARA EL COPIADO AL ARRAY
                                          ////////////////////////////////////////////////////////////////////////////////  
                                          break;

                                          
                                          case '\r':   
                                          break;

                                          
                                          
                                          
                                          default:       //SI ES CUALQUIER CARACTER LO ESCRIBER EN EL BUFFER INDEXY EN LA POSICION INDICADA
                                          if (POSICION_ARRAY < (LIMITE_BUFFER - 1)){
                                          BUFFER_WIFI_INDEX [POSICION_ARRAY++] = BYTE_SERIAL;
                                          break;
                                          
                                                                                          }//IF                                                
                                                                                       

                                                                                                                       }//SWITCH  
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////                                                                                            
} */


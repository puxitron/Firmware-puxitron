//#include <avr/wdt.h>
#include <SoftwareSerial.h>

 

SoftwareSerial SIM900(6,7);          //(RX,TX)

int puxitron=0;


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


char   COMPANIA [10];                                //GUARDA EL NOMBRE DE LA COMPAÑIA A LA QUE PERTNECE EL CHIP
String CHIP;                                        //GUARDA EL NOMBRE DE LA COMPAÑIA 


char   FECHA_[25];
String FECHA_SIM;


String RESPUESTA_SIM;

char   FTP_NUMEROS1[136];
char   FTP_NUMEROS2[136];
char   FTP_NUMEROS3[140];
char   MENSAJE[60];
char   CELULAR[21];
char   BODY[21];

unsigned long int ADC_SIM900; 
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 int     X=0;                                                                        
 int     I;
 int     D;
 int     P;                                          //REGISTROS DEL SERIAL
 int     Z;
 int     COPY;
 char    BUFFER_USART;
 char    BUFFER_USART2[380];
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
float V;                                            //VOLTAJE BATERIA
float VOLTAJE_BATERIA_DIVISOR;
float SUMA;

float POSTE;  
float V_POSTE;
float SUMA_POSTE;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 void setup() {
///////////////////////////////////////////////////////////////////////////////////////////////////////  
 // Timer1.initialize(5000000);            //5MS
 // Timer1.attachInterrupt(puxis);         //NOMBRE DE LA INT
  

//////////////////////////////////////////////////////////////////
 ///////////////////////////////////////////////////////////////////////////////////////////   
  SREG    =0x80;                                    //INT GLOBAL SOLO ACTIVA LA DEL UART
  UCSR0B  =0x98;                                    //ACTIVA TX Y RX A SI COM LA INT DEL RX
  UCSR0C  =0x06;                                    //CONFIGURAMOS A 8BITS DE DATOS SERIAL
  UBRR0H  =0x00;                                    //VALOR QUE SE LE CARGA AL REGISTRO HIGH 0
  UBRR0L  =0x67;                                    //VALOR QUE SE LE CARGA AL REGISTRO LOW 0x67 QUE ES IGUAL A 103 PARA 9600
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////  
  SIM900.begin(9600); 
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
  pinMode(A5, INPUT);                               //ADC VOLTAJE BATERIA SYTEM 
  pinMode(A4, INPUT);                               //ADC VOLTAJE POSTE 
  pinMode(17, INPUT);                               //ADC 3
  //pinMode(16, INPUT);                             
  pinMode(14, INPUT);                               //ADC 0
  

  pinMode(10,OUTPUT);    
  pinMode(9, OUTPUT);                              //
  pinMode(7, OUTPUT);                              //TX
  pinMode(6, INPUT);                               //RX
  pinMode(5, OUTPUT);                              //ACTIVA EL SIM900 
  
  pinMode(4, OUTPUT);                              //FTP ONLINE
  pinMode(3, OUTPUT);                              //ACTIVA CORNETA    
  pinMode(2, OUTPUT);                              //ACTIVA LUZ 
 /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////      
  analogReference(DEFAULT);                       //VOLTAJE DE REFERENCIA DEL ADC        
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
 digitalWrite(3, HIGH);          //DESACTIVA RELE
 digitalWrite(2, HIGH);          //DESACTIVA LUZ
  SIM900_ON(); 

  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
  delay(8000);
  SETUP_SIM900();
  VPN();
  //APN();
  FTP();
                                                 }  
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////                                                 

 void loop() {
   //interrupts();                          //ACTIVO LA INT DEL TIMER1
   COPIA_BUFFER(1,70);


   delay(1000);  
   LIMPIA_BUFFER();

                           if (strcasestr(BUFFER_USART2, "+CMT:")) {

                             for (int i=9;i<19;i++) {
                                     CELULAR[i-9]=BUFFER_USART2[i];
                                                                      }  
                                                                      
                                  for (int i=47;i<60;i++) {
                                     BODY[i-47]=BUFFER_USART2[i];
                                                                      }  
                                     
                                     
                                    /*SIM900.print("AT+CIPMUX=");                            
                                     delay(1000);
                                      SIM900.println(FTP_NUMEROS1);                            
                                     delay(4000);

                                                                          
                                    SIM900.print("AT+CIPMUX=");                            
                                     delay(1000);
                                      SIM900.println(FTP_NUMEROS2);                            
                                     delay(4000);

                                    SIM900.print("AT+CIPMUX=");                            
                                     delay(1000);
                                      SIM900.println(FTP_NUMEROS3);                            
                                     delay(4000);*/
                                                                                          
                                                   if (strcasestr(FTP_NUMEROS1, CELULAR)) {
                                                          
                                                      if (strcasestr(BODY, "On")) {
                                                          digitalWrite(3, LOW);          //ACTIVA RELE
                                                           digitalWrite(2, LOW);          //ACTIVA LUZ
                                                            //SIM900.println("ATD2299536830;"); 
                                                              TIO();         
                                                                                           
                                                                                      }   
                                                      if (strcasestr(BODY, "Off")) {
                                                          digitalWrite(3, HIGH);          //ACTIVA RELE
                                                           digitalWrite(2, HIGH);          //ACTIVA LUZ
                                                           //SIM900.println("ATD2299536830;");                                                   
                                                              //delay(1000);               
                                                                                           
                                                                                      }  
                                                                                                    
                                                                                                    }
                                                    
                                                    if (strcasestr(FTP_NUMEROS2, CELULAR)) {
                                                          
                                                      if (strcasestr(BODY, "On")) {
                                                          digitalWrite(3, LOW);          //ACTIVA RELE
                                                           digitalWrite(2, LOW);          //ACTIVA LUZ
                                                            //SIM900.println("ATD2299536830;");                                                   
                                                            TIO();              
                                                                                           
                                                                                      }   
                                                      if (strcasestr(BODY, "Off")) {
                                                          digitalWrite(3, HIGH);          //ACTIVA RELE
                                                           digitalWrite(2, HIGH);          //ACTIVA LUZ
                                                           //SIM900.println("ATD2299536830;");                                                   
                                                              //delay(1000);               
                                                                                           
                                                                                      }  
                                                                                                    
                                                                                                    }      

                                                      if (strcasestr(FTP_NUMEROS3, CELULAR)) {
                                                          
                                                      if (strcasestr(BODY, "On")) {
                                                          digitalWrite(3, LOW);          //ACTIVA RELE
                                                           digitalWrite(2, LOW);          //ACTIVA LUZ
                                                            //SIM900.println("ATD2299536830;");                                                   
                                                                TIO();              
                                                                                           
                                                                                      }   
                                                      if (strcasestr(BODY, "Off")) {
                                                          digitalWrite(3, HIGH);          //ACTIVA RELE
                                                           digitalWrite(2, HIGH);          //ACTIVA LUZ
                                                           //SIM900.println("ATD2299536830;");                                                   
                                                              delay(1000);               
                                                                                           
                                                                                      }  
                                                                                                    
                                                                                                    } 
                                                                                                                                             
                             
                              
                              
                              
                              
                              
                              
                              
                              
                              
                              if (strcasestr(BODY, "puxitron")) {      
                                      SIM900_ON();  
                                        delay(6000);
                                           asm volatile ( "jmp 0");  
                                                                  
                                                                
                                                                          
                                                                        
                                                                                           }                
                                                                             
                                                                              
                                                                              
                                                                              }
   
  // if (puxitron > 6) {  //10SEGUNDOS ENTRA A REFRESCAR LOS REGISTROS
     //       FTP();
      //          
       //                                                  }
   //FTP(); 
   //ENVIO_WEB();  
   //delay(10000);
   //SIM900_ON();
   //SETUP_SIM900(); 
                                             
                                              
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
    UDR0 = BUFFER_USART;                // ESCRIBE EN EL BUFFER TX  

} 
////////////////////////////////////////////////////////////////////////



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
      for (int i=0; i<199;i++)
        {BUFFER_USART2[i]=0x00;}                  //CADA POSICION DEL STRING LE PONE CARACTER NULL 00
          if(P<2){
            P=-3;
                      }         
             I=P; 
               D=W;
 //return K;
}

void COPIA_CMT(int P,int W)                       // FUNCION QUE LIMPIA TODO EL STRING QUE GUARDA LAS VARIABLES DE RX
{
  //K=0;
  COPY=1;                                 
    X=-1;
      Z=-1;
 
          if(P == 1){
            P=-3;
                      }         
             I=P; 
               D=W;
 //return K;
}

void LIMPIA_BUFFER()                       // FUNCION QUE LIMPIA TODO EL STRING QUE GUARDA LAS VARIABLES DE RX
{
  COPY=0;
    X=-1;
      Z=-1;
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
/*void TEMPERATURA()                       // FUNCION QUE 
{


  sensors.requestTemperatures(); // Send the command to get temperatures
  data=sensors.getTempCByIndex(0);
  //SIM900.println(data,1);

}*/


void SIM900_ON()                       // FUNCION QUE LIMPIA TODO EL STRING QUE GUARDA LAS VARIABLES DE RX
{
  digitalWrite(9, HIGH);
  delay(2000);//                                     //ENCIENDE EL SIM900
  digitalWrite(9, LOW);
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
 // delay(1000);
   
  delay(10000);                                                    //BUSCANDO SEÑAL 
  
  COPIA_BUFFER(20,31);
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
  delay(1000);                                                      //AL SERVIDOR WEB,1=ENVIA PURO HEXADECIMAL AL SERVIDOR WEB)
    
  SIM900.println(F("AT+CMGF=1"));                                  //MODO 0 ES NORMAL,MODO 1 ES TRANSPARENTE (0=ENVIA PURO ASCII 
  delay(1000);

       SIM900.println(F("AT+CNMI=2,2"));                                  //MODO 0 ES NORMAL,MODO 1 ES TRANSPARENTE (0=ENVIA PURO ASCII 
  delay(1000);   

 
       SIM900.println(F("AT+CLIP=1"));                                  //MODO 0 ES NORMAL,MODO 1 ES TRANSPARENTE (0=ENVIA PURO ASCII 
  delay(1000);   
 
 // SIM900.println("AT+CBAND=\"ALL_BAND\"");
 // delay(1000);
  
  //SIM900.println("AT+CBAND=\"GSM850_PCS_MODE\"");
  //delay(1000);
 
  //SIM900.println("AT+CBAND=\"GSM850_MODE\"");
  //delay(1000);  
  
   //SIM900.println("AT+CBAND=\"ALL_BAND\"");
   //delay(1000);
}
void FTP()                                                         // FUNCION QUE 
{
        
  SIM900.println(F("AT+FTPTYPE=\"A\""));                                    //MUESTRA LA IP 
  delay(1000);   

  SIM900.println(F("AT+FTPCID=1"));                                    //MUESTRA LA IP 
  delay(1000);  

  SIM900.println(F("AT+FTPSERV=\"ftp.webcindario.com\""));                                    //MUESTRA LA IP 
  delay(2000);  

   SIM900.println(F("AT+FTPPORT=21"));                                    //MUESTRA LA IP 
  delay(1000);  

   SIM900.println(F("AT+FTPUN=\"puxitron\""));                                    //MUESTRA LA IP 
  delay(1000);

  SIM900.println(F("AT+FTPPW=\"amdtools\""));                                    //MUESTRA LA IP 
  delay(1000);  
    
  SIM900.println(F("AT+FTPGETPATH=\"/\""));                                    //MUESTRA LA IP 
  delay(1000);  

  SIM900.println(F("AT+FTPGETNAME=\"6.txt\""));                                    //MUESTRA LA IP 
  delay(1000);

  COPIA_BUFFER(1,30);
  SIM900.println(F("AT+FTPGET=1"));                                    //MUESTRA LA IP 
  delay(15000);  
                              
                              if (!strcasestr(BUFFER_USART2, "+FTPGET:1,1")) {           //BUSCA ERROR EN EL ARRAY CHAR DEL SERIAL SI LO ENCUENTRA APAGA EL SIM900 
                              SIM900_ON();  
                              delay(60000);
                              asm volatile ( "jmp 0");  
                                                                  
                                                                
                                                                          
                                                                        
                                                                                     } 
 
  digitalWrite(4, HIGH);          //INDICA REGISTROS FTP OK
  LIMPIA_BUFFER();

  

  COPIA_BUFFER(32,364);
  SIM900.println(F("AT+FTPGET=2,1024"));                                    //MUESTRA LA IP 
  delay(1000); 
    for (int i=0; i<107;i++){
             FTP_NUMEROS1[i]=BUFFER_USART2[i];
                                               }

     for (int i=106; i<216;i++){
            FTP_NUMEROS2[i-106]=BUFFER_USART2[i];
                                               }  

     for (int i=216;i<324;i++){
            FTP_NUMEROS3[i-216]=BUFFER_USART2[i];
                                               }                                             
                                                                                
  
  
   LIMPIA_BUFFER();   

 

  //delay(15000);  
}

void VPN()                                                         // FUNCION QUE 
{
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
  SIM900.println(F("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\""));                                    
  delay(4000); 

  
  if(CHIP == "AT&T EDGE"){
      //SIM900.println(APN_IUSACELL_1);                                //CONFIGURACION APN DE TELCEL
      //delay(1000); 
      //SIM900.println(APN_IUSACELL_2);                                //CONFIGURACION APN DE TELCEL
      //delay(1000); 
      //SIM900.println(APN_IUSACELL_3);                                //CONFIGURACION APN DE TELCEL
      //delay(1000); 
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
  
  //SIM900.println(F("AT+SAPBR=0,1"));                                     
  //delay(4000); 
}
void APN()                                                         // FUNCION QUE 
{
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////                                                    
  
  SIM900.println(F("AT+CIPMUX=0"));
  delay(2000); 
  
  if(CHIP == "AT&T EDGE"){
      //SIM900.println(VPN_IUSACELL);                               //CONFIGURACION VPN DE TELCEL
        //delay(1000);   
                                  }
          else{
//              SIM900.println(VPN_TELCEL);                          //CONFIGURACION VPN DE IUSACELL
                    delay(1000);   
                                        }                              
  
  COPIA_BUFFER(1,70); 
  SIM900.println(F("AT+CIICR"));                                    //ENCIENDE EL RADIO 
    delay(4000); 
                  if (strcasestr(BUFFER_USART2, "ERROR")) {           //BUSCA ERROR EN EL ARRAY CHAR DEL SERIAL SI LO ENCUENTRA APAGA EL SIM900 
  
                               //SIM900_ON(); 
                               //soft_restart ();                         
                               
                               //MCUSR=0x00;              //LIMPIA TOADAS LAS BANDERAS DEL REGISTRO    
                               //wdt_enable(WDTO_15MS); 
                               //for(;;) { 
              
                                               // }
                                                                  }    
  LIMPIA_BUFFER();
  
  
  SIM900.println(F("AT+CIFSR"));                                    //MUESTRA LA IP 
  delay(4000);  


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
 
}
void TIO()
{
  //SIM900.write("AT+CMGF=1\r");           //set GSM to text mode
  //delay(1000);
  SIM900.print("AT+CMGS=\"+522292293635\"\r");   //phone number to which you want to send sms
  delay(1000);
  SIM900.print("Alarma activada por: ");           //SMS body 
  delay(50); 
  SIM900.print(CELULAR);           //SMS body 
  delay(500); 
  SIM900.write(0x1A);           // sends ctrl+z end of message 
  delay(1000);
}


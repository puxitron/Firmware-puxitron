#include <SoftwareSerial.h>

 

SoftwareSerial SIM900(2,3);          //(RX,TX)

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


 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 void setup() {

  SREG    =0x80;                                    //INT GLOBAL SOLO ACTIVA LA DEL UART
  UCSR0B  =0x98;                                    //ACTIVA TX Y RX A SI COM LA INT DEL RX
  UCSR0C  =0x06;                                    //CONFIGURAMOS A 8BITS DE DATOS SERIAL
  UBRR0H  =0x00;                                    //VALOR QUE SE LE CARGA AL REGISTRO HIGH 0
  UBRR0L  =0x67;                                    //VALOR QUE SE LE CARGA AL REGISTRO LOW 0x67 QUE ES IGUAL A 103 PARA 9600
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////  
  SIM900.begin(9600); 
 // Serial.begin(9600);
  //pinMode(6,OUTPUT);                              //RESET
  //pinMode(7,INPUT);                               //RX
  //pinMode(8,OUTPUT);                              //TX
  
  digitalWrite(6,HIGH);
  //digitalWrite(6,LOW);
  //delay(1000);  
  //ATdigitalWrite(6,HIGH);
  //SETUP_SIM900();

                                                 }  
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////                                                 

 void loop() {


                                                                          
  SIM900.println("AT");                            
  delay(1000);

  /*  while (SIM900.available() > 0) {
    char inByte = SIM900.read();
    Serial.write(inByte);
  }  */                                         
                                              
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

                                             
                                              
                                                  } 
                                                  
 UDR0 = BUFFER_USART;
 }   


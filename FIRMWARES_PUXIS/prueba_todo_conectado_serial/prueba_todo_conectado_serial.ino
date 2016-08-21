#include <SoftwareSerial.h> //
int long acumulador=0;
int long SUMA=0;
float    PROM=0;
float    INTER=0;
SoftwareSerial SIM5320(9,A1); // RX, TX

void setup() {
 SIM5320.begin(9600);
 Serial.begin(9600);

  pinMode(A5, INPUT);                              //
  pinMode(A4, INPUT);                              //
  pinMode(17, INPUT);                              //
  //pinMode(16, INPUT);                             
  //pinMode(15, INPUT); 
  pinMode(14, INPUT);                              //
  

  pinMode(10,OUTPUT);    
  pinMode(9, OUTPUT);                              //ACTIVA EL SIM900 
  pinMode(6, INPUT);                               //
  pinMode(5, OUTPUT);                              //
  pinMode(4, INPUT);                               //
  analogReference(DEFAULT);

  SIM900_ON();
  delay(20000);
 
  SIM5320.println(F("AT+CSQ"));
  delay(4000);
          
   // if(Serial.available() > 0 ){ 
     //      if (Serial.find("OK"))    // Busca el OK en la respuesta del wifi
       //          SIM900_ON();     
 
                                                           //}

 // SIM5320.println(F("AT+CREG?"));
 // delay(4000);
  
  SIM5320.println(F("AT+CGREG?"));
  delay(4000);

  SIM5320.println(F("AT+COPS?"));
  delay(4000);
  //////////////////////////////////
  SIM5320.println(F("AT+CGSOCKCONT=1,\"IP\",\"internet.itelcel.com\""));
  delay(4000);

  SIM5320.println(F("AT+CSOCKSETPN=1"));
  delay(4000);

  SIM5320.println(F("AT+CIPMODE=0"));
  delay(4000);

  SIM5320.println(F("AT+NETOPEN"));
  delay(4000);

  SIM5320.println(F("AT+IPADDR"));
  delay(4000);


  //Serial.println(F("AT+CGPSSWITCH=1"));
  //delay(4000);


}




void loop() {
  
WEB();
 delay(30000);
//GPS();
// delay(2000);

}




void WEB()                                  //  
{
  Serial.println(F("AT+CSQ"));
  delay(4000);
  
  
  Serial.println(F("AT+CIPOPEN=0,\"TCP\",\"vtechph.herokuapp.com\",80"));
  delay(10000);

  Serial.println("AT+CIPSEND=0,285");
  delay(10000);

 // Serial.println("289");
 // delay(4000);
///////////////////////////////////////////////////////////
    Serial.println("POST /vtechinfos/ HTTP/1.1");
  delay(100);

    Serial.println("Content-Type: application/json");
  delay(100);
  
   Serial.println("Host: versetech.herokuapp.com");
  delay(100);
  
   Serial.println("Connection: keep-alive");
  delay(100);
  
   Serial.println("Content-Length:38");
  delay(100);
  
   Serial.println("");
  delay(100);

///////////////////////////////////////////////////////////////   
    Serial.println(F("{\"phvalue\":\"4.0\",\"batterystate\":\"3.9\"}"));
   delay(100);



//////////////////////////////////////////////////////////////    
    
    Serial.write(0x1A);
  delay(10000);

    Serial.println("AT+CIPCLOSE=0");                                          
   delay(1000); 



 } 

void SIM900_ON()                                  //  
{
  digitalWrite(2, HIGH);
  delay(2000);//                                     //ENCIENDE EL SIM5320
  digitalWrite(2, LOW);
  delay(2000);
 } 

/*
////////////////////////////////////////////////
void BYTE_LEIDO_BUFFER_SERIAL (const byte BYTE_SERIAL){
////////////////////////////////////////////////
  int x=15;
  char FIN=' ';
  int i=-1;
  static char BUFFER_WIFI_INDEX [LIMITE_BUFFER];
  static unsigned int POSICION_ARRAY =0;

                                         
                                          
                                          switch (BYTE_SERIAL){
                                          ////////////////////////////////////////////////////////////////////////////////  
                                          case '\n':                                  //VERIFICO SI ES NUEVA LINEA
                               
                                          INDEX=BUFFER_WIFI_INDEX;
                                          BUFFER_WIFI_INDEX [POSICION_ARRAY] = 0;    //PONE A CERO LA ULTIMA POS QUE CORRESPONDE AL NEW LINEA
                                          IMPRIME_DATO (BUFFER_WIFI_INDEX);
                                          POSICION_ARRAY = 0;                        //RESETEA EL CONTADOR PARA EL COPIADO AL ARRAY  
                                          break;
                                          //////////////////////////////////////////////////////////////////////////////// 

                                          
                                          case '\r':   
                                          break;

                                          
                                          
                                          //////////////////////////////////////////////////////////////////////////////// 
                                          default:                                     //SI ES CUALQUIER CARACTER LO ESCRIBER EN EL BUFFER INDEXY EN LA POSICION INDICADA
                                          if (POSICION_ARRAY < (LIMITE_BUFFER - 1)){
                                          BUFFER_WIFI_INDEX [POSICION_ARRAY++] = BYTE_SERIAL;
                                          break;
                                          
                                                                                         }//IF                                                
                                                                                       

                                                                                            }//SWITCH  
///////////////////////////////////                                                                                       
} 


void IMPRIME_DATO (const char * data){

  Serial.println (data);
//////////////////////////
}  */

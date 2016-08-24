#include <EEPROM.h>
#include <SoftwareSerial.h>

SoftwareSerial SIM800(4,15); // RX, TX

///////////////////////
#define REGISTROS_MAXIMOS 10

 
int    ADRR_EEPROM_OCUPADO=REGISTROS_MAXIMOS; 
byte   INDEX_EEPROM;
int    y_clip;
int    y_eeprom;                                               //EEPROM
int    y;
char   MENSAJE[60];
char   CELULAR[21];
char   BODY[25];
char   CELULAR_ADMIN[25];
char   CELULAR_ALTA[25];
char   LLAMADA_RING[11];
char   EEPROM_BUFFER[11];
char   NUMERO_A_BORRAR[11];


  
boolean ERROR_LENGTH_MSJ=false;
boolean CARACTER_INVALIDO_MSJ= false;
String  CELULAR_ALTA_STRING;
String  NUMERO_EN_PELIGRO;
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int     X;
int     I;
int     D;
int     P;                                          //REGISTROS DEL SERIAL
int     Z;
int     COPY;
char    BUFFER_USART2[90];
char    BUFFER_USART;
///////////////////////////////

///////////////////////////////
int     FLAG_RESPONSE;
int     RESPONSE_ZZ = -1;
int     RESPONSE;
char    RESPONSE_BUFFER[80];
/////////////////////////////////

void setup() {
  
  ////////////////////////////////////////////////////////////////////////////
  SREG    = 0x80;                                   //INT GLOBAL SOLO ACTIVA LA DEL UART
  UCSR0B  = 0x98;                                   //ACTIVA TX Y RX A SI COM LA INT DEL RX
  UCSR0C  = 0x06;                                   //CONFIGURAMOS A 8BITS DE DATOS SERIAL
  UBRR0H  = 0x00;                                   //VALOR QUE SE LE CARGA AL REGISTRO HIGH 0
  UBRR0L  = 0x67;                                   //VALOR QUE SE LE CARGA AL REGISTRO LOW 0x67 QUE ES IGUAL A 103 PARA 9600 08 PARA 115200
  //////////////////////////////////////////////////////////////////////////////
  pinMode(4,  OUTPUT);                              //chicharron
  pinMode(3,  OUTPUT);                              //estrobo
  pinMode(2,  OUTPUT);                              //
 
  
  digitalWrite(4, HIGH);
  digitalWrite(3, HIGH);
  
  digitalWrite(2, LOW);
  delay(2000);
  digitalWrite(2, HIGH);
  delay(6000);


  SIM800.begin(9600);

  SIM800.println("AT");                             //
  delay(10000);

  SIM800.println("AT+IPR=9600");                                         //
  delay(2000);
  
  SIM800.println("AT+CBAND=PCS_MODE");                                         //
  delay(4000);
  

  SIM800.println("AT+CMGD=1");          //BORRA LA MEMORIA DONDE ESTAN LOS MENSAJES
  delay(1000);

  SIM800.println("AT+CMGF=1");              //MENSAJE MODO TEXTO
  delay(1000);

  SIM800.println("AT+CNMI=2,2");          //MUESTRA EL MENSAJE COMPLETO TXT
  delay(1000);


  //SIM800.println("AT+CLIP=1");          //MUESTRA EL MENSAJE COMPLETO TXT
  //delay(2000);

    //SIM800.println("AT+CHLD=?");          //MUESTRA EL MENSAJE COMPLETO TXT
  //delay(2000);

  
  //SIM800.println("AT+CCWA=1,1");          //MUESTRA EL MENSAJE COMPLETO TXT
  //delay(2000);

                                                                             for(int x=1;x<=REGISTROS_MAXIMOS;x++){             //SI EL SISTEMA SE APAGA EL FOR DETERMINA EL VALOR DE CUANTAS POSICIONES DE LA EEPROM ESTAN OCUPADAS (INDEX)
                                                                                              
                                                                                                                                    
                                                                                                                      if(EEPROM.read(x) != 0){
                                                                                                                          ADRR_EEPROM_OCUPADO--; 
                                                                                                                                                                     
                                                                                                                                                                                                              }

  
  
                                                                                                                       }

  SETUP_800();

}

void loop() {

  COPIA_USART(1, 80);

  delay(2000);

while(BUFFER_USART2[0] > 0) { 
  

if (strcasestr(BUFFER_USART2, "+CLIP:")) {

                               for (int i = 18; i <=27; i++) {
                                           if (BUFFER_USART2[i] == 0x0D) {
                                                      BUFFER_USART2[i]= 0x00;   
                                                                             }
                                      
                                      LLAMADA_RING[i - 18] = BUFFER_USART2[i];               //SACA EL NUMERO DEL IDENTIFICADOR DEL SIM (LLAMADA ENTRANTE)
                                                                             }

                                                                             //SIM800.print("AT+CMGD="); 
                                                                               // delay(100);
                                                                                   //  SIM800.print(LLAMADA_RING);    
                                         
                                              for(int x=1;x<=REGISTROS_MAXIMOS;x++){             //NUMERO DE RESGISTROS (NUMEROS DE REGISTROS)              
                                                     y_clip+=x; 
                                                      
                                                                           for(int k=0;k<10;k++){ 
                                                                                  EEPROM_BUFFER[k]=EEPROM.read(y_clip);
                                                                                        y_clip+=101;                    
                                                                                                  }//for
                                        
                                        
                                                                                                                                   if (strcasestr(EEPROM_BUFFER,LLAMADA_RING)) {
                                                                                                                                      //SIM800.println("AT+CMGD=PUTO");          //BORRA LA MEMORIA DONDE ESTAN LOS MENSAJES
                                                                                                                                          NUMERO_EN_PELIGRO=EEPROM_BUFFER;
                                                                                                                                            delay(1000); 
                                                                                                                                                SIM800.println("ATH");             //CUELGA LA LLAMDA :)
                                                                                                                                                  delay(300);  
                                                                                                                                                       
                                                                                                                                                       digitalWrite(4, !digitalRead(4)); 
                                                                                                                                                         digitalWrite(3, !digitalRead(3)); 
                                                                                                                                                       
                                                                                                                                                            if(digitalRead(4) == 0){
                                                                                                                                                                     ALARMA_ACTIVADA();
                                                                                                                                                                                                                 }
                                                                                                                                                       
                                                                                                                                                            else{
                                                                                                                                                                      ALARMA_DESACTIVADA();
                                                                                                                                                                                            }
                                                                                                                                                          
                                                                                                                                                           
                                                                                                                                                        
                                                                                                                                                      
                                                                                                                                    break;                         
                                                                                
                                                                                                                                                                                          }
                                                y_clip=0;                                                                                                                                           

                                                             }//*/
                                             

                                       
                                               
                                              
                                              }//LLAMADA



if (strcasestr(BUFFER_USART2, "+CMT:")) {//MENSAJE 
                         
                          for (int i = 9; i < 19; i++) {                                                  //////////////////////////////OBTIENE EL NUMERO DEL MENSAJE
                                      CELULAR[i - 9] = BUFFER_USART2[i];
                                                                             }

                            for (int i = 47; i < 71; i++) {                                              //////////////////////////////////OBTIENE EL CUERPO DEL MENSAJE
                                        BODY[i - 47] = BUFFER_USART2[i];
                                                                  }

                                                            ///////////////////////////////////////////////////////////////
                                                            if (strcasestr(BODY, "Borrar@:")) {         ///////////////////////////////BORRA UN NUMERO ESPECIFICO DE LA EEPROM
                                                                  
                                                               for (int i = 9; i <=19; i++) {

                                                                      if (BODY[i] == 0x0D) {
                                                                                    BODY[i]= 0x00;   
                                                                                                      }
                                                                             NUMERO_A_BORRAR[i - 9] = BODY[i];
                                                                                                    
                                                                                                       }//FOR 
                                                                  BUSCAR_NUMERO_BORRAR();
                                                                    // SIM800.print("AT+CMGD="); 
                                                                      //          delay(100);
                                                                        //             SIM800.print(NUMERO_A_BORRAR);       
                                                                          
                                                                          for (int i = 0; i < 26; i++) {
                                                                                   BODY[i] = NULL;
                                                                                                                  }//FOR LIMPIA BODY BUFFER
                                                                                                                      }
 
                                                            /////////////////////////////////////////////////////////////////////////////////////////////////
                                                            if (strcasestr(BODY, "limpiar memoria")) {     ///////////////////////////////LIMPIA LA EEPROM
                                                                     LIMPIA_EEPROM();
                                                                            for (int i = 0; i < 26; i++) {
                                                                                   BODY[i] = NULL;
                                                                                                                  }//FOR LIMPIA BODY BUFFER
                                                                                                                      }

                                                            ///////////////////////////////////////////////////////////////////////////////////////////////////
                                                            if (strcasestr(BODY, "Admin@:")) {          //////////////////////////////////ALTA DEL ADMIN
                                                                                    
                                                                  for (int i = 8; i <=18; i++) {

                                                                      if (BODY[i] == 0x0D) {
                                                                                    BODY[i]= 0x00;   
                                                                                                      }
                                                                             CELULAR_ADMIN[i - 8] = BODY[i];
                                                                                                    
                                                                                                       }//FOR
                                                                                
                                                                                                                
                                                                                                              
                                                                                                           y=0;
                                                                                                           for(int k=0;k<10;k++){

                                                                                                            EEPROM.write(y,CELULAR_ADMIN[k]);
                                                                                                                y+=101;                     
                                                                                                                                                                  
                                                                                                                                                                  
                                                                                                                                       }//for2  
                                                                                                                
                                                                 y=0;
                                                                 ALTA_ADMIN();  
                                                                         
                                                                          
                                                                                                        for (int i = 0; i < 26; i++) {
                                                                                                                  BODY[i] = NULL;
                                                                                                                                            }//FOR LIMPIA BODY BUFFER
                                                                                                                       
                                                                                                     
                                                                                                                
                                                                                                                }//IF ADMIN
                                                            
                                                            if (strcasestr(BODY, "Alta@:")) {      ////////////////////////////////MECANISMO DE ALTA DE CUALQUIER NUMERO INCLUYE VALIDACION 10DIGITOS(ONLY NUMBERS)



                                                            for (int i = 7; i < 20; i++) {

                                                                  if (BODY[i] == 0x0D) {
                                                                                BODY[i]= 0x00;   
                                                                                                      }
                                                                       CELULAR_ALTA[i - 7] = BODY[i];
                                                                       CELULAR_ALTA_STRING=CELULAR_ALTA;                               
                                                                                                       }



                                                                                                                      if(CELULAR_ALTA_STRING.length() < 10 || CELULAR_ALTA_STRING.length() > 10) { 
                                                                                                                          //ALTA_INCORRECTA();
                                                                                                                             ERROR_LENGTH_MSJ=true;
                                                                                                                                                                                                          }                                         

                                                                                                                       
                                                                                                                     
                                                                                                                      
                                                                                                                      
                                                                                                                                                        
                                                                                                                     for (int i = 0; i < 10; i++) {
                                                                                                                                if (CELULAR_ALTA[i] < 48 || CELULAR_ALTA[i] > 57) {  //// ASCII 0123...9
                                                                                                                                              CARACTER_INVALIDO_MSJ=true;
                                                                                                                                                                                                         }
                          
                                                                                                                                                                                                                      
                                                                                                                                                                                            }//FOR

                                                                                                                    
                                                                                                                     
                                                                                                                     
                                                                                                                     
                                                                                                                     
                                                                                                                     if (CARACTER_INVALIDO_MSJ == false && ERROR_LENGTH_MSJ == false) { 

 
                                                                                                                                    
                                                                                                                                  for(int x=1;x<=REGISTROS_MAXIMOS;x++){             //NUMERO DE RESGISTROS (NUMEROS DE REGISTROS)
                                                                                                                                      y_eeprom=0; 
                                                                                                                                    
                                                                                                                                                      if(EEPROM.read(x) == 0 && ADRR_EEPROM_OCUPADO > 0){
                                                                                                                                                            ADRR_EEPROM_OCUPADO--; 
                                                                                                                                                                 //ESCRIBIR EN LA EEPROM TAREA PENDIENTE                                  
                                                                                                                                                                                              
                                                                                                                                                                                              
                                                                                                                                                                                              
                                                                                                                                                                                              y_eeprom+=x;
                                                                                                                                                                                                  for(int k=0;k<10;k++){
                                                                                                                                                                                     
                                                                                                                                                                                                     EEPROM.write(y_eeprom,CELULAR_ALTA[k]);
                                                                                                                                                                                                          y_eeprom+=101;                     
                                                                                                                                                                  
                                                                                                                                                                  
                                                                                                                                                                                                                             }//for2  
                                                                                                                                                                                                                                                                                                                                                
                                                                                                                                                                               
                                                                                                                                                                                        
                                                                                                                                                                                       
                                                                                                                                                                                        ALTA_CORRECTA();
                                                                                                                                                                                        break;                     
                                                                                                                                                                     
                                                                                                                                                       
                                                                                                                                                       
                                                                                                                                                       
                                                                                                                                                       
                                                                                                                                                       
                                                                                                                                                      
                                                                                                                                                                                                     
  
                                                                                                                                                                                                                                        }
                                                                                                                                                     
                                                                                                                                                      
                                                                                                                                                      else {
                                                                                                                                                       
                                                                                                                                                         if(ADRR_EEPROM_OCUPADO == 0 || EEPROM.read(1023) == '!' ){
                                                                                                                                                            EEPROM.write(1023,33);
                                                                                                                                                            SIN_MEMORIA();        
                                                                                                                                                              break;   
                                                                                                                                                                      
                                                                                                                                                                                       }
                                                                                                                                                                      }
 
                                                                                                                                                        }//                             
                                                                                                                                                                                              
                                                                                                                                                                                              }//

                                                                                                                                                             
                                                                                                                                                
                                                                                                                        
                                                                                                                        else { 

                                                                                                                                                                               
                                                                                                                            CARACTER_INVALIDO_MSJ=false;                               
                                                                                                                               ERROR_LENGTH_MSJ=false;                               
                                                                                                                                   ALTA_INCORRECTA();                               
                                                                                                                                                                 }
 
                                                                                                                   
                                                           
                                                                                                                                 
                                                                                                   }



    
    
    if (strcasestr(BODY, "Reset")) {
      asm volatile ( "jmp 0");

                                            }



            }




  break;

}



}
////////////////////////////////////////////////////////
void SETUP_800() {

  SIM800.println("AT+CSQ");                                            //
  delay(500);

  SIM800.println("AT+COPS?");                                          //
  delay(500);

  SIM800.println("AT+CBAND?");                                         //
  delay(500);

 // SIM800.println(F("AT+CIPMUX=0"));                //
 // delay(500);

 // SIM800.println(F("AT+CSTT=\"internet.itelcel.com\",\"webgprs\",\"webgprs2003\""));                 //
 // delay(500);

  //SIM800.println(F("AT+CIICR"));                      //
 // delay(6000);

  //SIM800.println(F("AT+CIFSR"));                     //
  //delay(1000); //

  //COPIA_USART(10,24);
  //SIM800.println(F("AT+GSN"));                     //
  //delay(1000); //

  //MEI=BUFFER_USART2;

}
///////////////////////////////////////////////////////



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
                                                                  Z= -1;
                                                                      I = 0;
                                                                          D = 0;
                                                                                         }



  }
  //////////////////////////////////////////////////////////////////////////////////////////


  //UDR0 = BUFFER_USART;                            // LEE EL BUFFER RX

}//
//////////////////////////////////////////////////////////////////////////////////////////
void COPIA_USART(const int P, const int W)                      // FUNCION QUE LIMPIA TODO EL STRING QUE GUARDA LAS VARIABLES DE RX
{

  COPY = 1;
  X = 0;
  Z = -1;

  for (int i = 0; i < 90; i++)
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
void ALTA_CORRECTA()
{
  //SIM900.write("AT+CMGF=1\r");           //set GSM to text mode
  //delay(1000);
  SIM800.print("AT+CMGS=\"+52");   //
  delay(100);
  SIM800.print(CELULAR);   //  "\"\r"
  delay(100);
  SIM800.print("\"\r");   //
  delay(1000);
  SIM800.print("Alta correcta numero:");           //SMS body
  delay(50);
  SIM800.print(CELULAR_ALTA_STRING +"                \r"); //SMS body
  delay(500);
  SIM800.print("Numeros disponibles:");           //SMS body
  delay(50);
  SIM800.print(ADRR_EEPROM_OCUPADO);           //SMS body
  delay(50);
  SIM800.write(0x1A);           // sends ctrl+z end of message                          
  delay(6000);
}

void ALTA_INCORRECTA()
{
  //SIM900.write("AT+CMGF=1\r");           //set GSM to text mode
  //delay(1000);
  SIM800.print("AT+CMGS=\"+52");   //
  delay(100);
  SIM800.print(CELULAR);   //  "\"\r"
  delay(100);
  SIM800.print("\"\r");   //
  delay(1000);
  SIM800.print("Numero invalido");           //SMS body
  delay(50);
  SIM800.write(0x1A);           // sends ctrl+z end of message
  delay(4000);
}

void LIMPIA_EEPROM()
{
  ADRR_EEPROM_OCUPADO=REGISTROS_MAXIMOS; 
  for (int i = 0 ; i < EEPROM.length() ; i++) {
    EEPROM.write(i, 0);
                                                              }

  SIM800.print("AT+CMGS=\"+52");   //
  delay(100);
  SIM800.print(CELULAR);   //  "\"\r"
  delay(100);
  SIM800.print("\"\r");   //
  delay(1000);
  SIM800.print("Limpieza correcta");           //SMS body
  delay(50);
  SIM800.write(0x1A);           // sends ctrl+z end of message
  delay(4000);
}

void SIN_MEMORIA()
{
   
  SIM800.print("AT+CMGS=\"+52");   //
  delay(100);
  SIM800.print(CELULAR);   //  "\"\r"
  delay(100);
  SIM800.print("\"\r");   //
  delay(1000);
  SIM800.print("No hay suficiente espacio de almacenamiento");           //SMS body
  delay(50);
  SIM800.write(0x1A);           // sends ctrl+z end of message
  delay(4000);
}

void ALARMA_ACTIVADA()
{
 int z;  
  SIM800.print("AT+CMGS=\"+52");   //
  delay(100);
                                       z=0;                           //POSICION DEL ADMIN LA 0
                                         for(int k=0;k<10;k++){ 
                                          EEPROM_BUFFER[k]=EEPROM.read(z);
                                                  z+=101;                    
                                                        }//for
  
  
  
  SIM800.print(EEPROM_BUFFER);   //  "\"\r"
  delay(100);
  SIM800.print("\"\r");   //
  delay(1000);
  SIM800.print("Alarma activada por el numero:");           //SMS body
  delay(50);
  SIM800.print(NUMERO_EN_PELIGRO);           //SMS body
  delay(50);
  SIM800.write(0x1A);           // sends ctrl+z end of message
  delay(4000);
}

void ALARMA_DESACTIVADA()
{
 int z;  
  SIM800.print("AT+CMGS=\"+52");   //
  delay(100);
                                       z=0;                           //POSICION DEL ADMIN LA 0
                                         for(int k=0;k<10;k++){ 
                                          EEPROM_BUFFER[k]=EEPROM.read(z);
                                                  z+=101;                    
                                                        }//for
  
  
  
  SIM800.print(EEPROM_BUFFER);   //  "\"\r"
  delay(100);
  SIM800.print("\"\r");   //
  delay(1000);
  SIM800.print("Alarma desactivada por el numero:");           //SMS body
  delay(50);
  SIM800.print(NUMERO_EN_PELIGRO);           //SMS body
  delay(50);
  SIM800.write(0x1A);           // sends ctrl+z end of message
  delay(4000);
}

void ALTA_ADMIN()
{
int x;   
  SIM800.print("AT+CMGS=\"+52");   //
  delay(100);
  SIM800.print(CELULAR_ADMIN);   //  "\"\r"
  delay(100);
  SIM800.print("\"\r");   //
  delay(1000);
  SIM800.print("Admin activado:\r");           //SMS body
  delay(50);
  
                                     x=0;                           //POSICION DEL ADMIN LA 0
                                         for(int p=0;p<10;p++){ 
                                          EEPROM_BUFFER[p]=EEPROM.read(x);
                                                  x+=101;                    
                                                        }//for
  
  
  SIM800.print(EEPROM_BUFFER);           //SMS body
  
  SIM800.write(0x1A);           // sends ctrl+z end of message
  delay(4000);
}

void BUSCAR_NUMERO_BORRAR()
{
 int S;
 y_eeprom=0; 
 
                                              for(int x=1;x<=REGISTROS_MAXIMOS;x++){             //NUMERO DE RESGISTROS (NUMEROS DE REGISTROS)              
                                                     y_clip+=x; 
                                                       S++;     
                                                                           for(int k=0;k<10;k++){ 
                                                                                  EEPROM_BUFFER[k]=EEPROM.read(y_clip);
                                                                                        y_clip+=101;                    
                                                                                                  }//for
                                        
                                        
                                                                                                                                   if (strcasestr(EEPROM_BUFFER,NUMERO_A_BORRAR)) {
                                                                                                                                      //SIM800.println("AT+CMGD=PUTO");          
                                                                                                                                        //delay(50);
                                                                                                                                                                      y_eeprom+=x;
                                                                                                                                                                             for(int w=0;w<10;w++){//BORRA EL NUMERO EN LA EEPROM
                                                                                                                                                                                     
                                                                                                                                                                               EEPROM.write(y_eeprom,0);
                                                                                                                                                                                   y_eeprom+=101;                     
                                                                                                                                                                  
                                                                                                                                                                  
                                                                                                                                                                                                           }//for2   
                                                                                                                                    NUMERO_ENCONTRADO();                
                                                                                                                                    ADRR_EEPROM_OCUPADO++;                        
                                                                                                                                    EEPROM.write(1023,0);                       
                                                                                                                                    //UPDATE_EEPROM();                 
                                                                                                                                    break;                         
                                                                                
                                                                                                                                                                                          }

                                                                                                                                  
                                                                                                                                   if (S == REGISTROS_MAXIMOS) {
                                                                                                                                      S=0;
                                                                                                                                      //SIM800.println("AT+CMGD=PUTO");          //BORRA LA MEMORIA DONDE ESTAN LOS MENSAJES
                                                                                                                                        //delay(50);
                                                                                                                                          NUMERO_NO_ENCONTRADO();               
                                                                                                                                                           
                                                                                                                                                        
                                                                                                                                                      
                                                                                                                                                            
                                                                                
                                                                                                                                                                                          }                                                           
                                              
                                               y_clip=0; 
                                                                                                                                                                                         

                                                             }//*/ 
                                                         
//

}
void NUMERO_ENCONTRADO()
{
  int x;  
  
  SIM800.print("AT+CMGS=\"+52");   //
  delay(100);
                                     x=0;                           //POSICION DEL ADMIN LA 0
                                         for(int p=0;p<10;p++){ 
                                          EEPROM_BUFFER[p]=EEPROM.read(x);
                                                  x+=101;                    
                                                                    }//for
  
  SIM800.print(EEPROM_BUFFER);   //  "\"\r"
  delay(100);
  SIM800.print("\"\r");   //
  delay(1000);
  SIM800.print("Numero borrado:");           //SMS body
  delay(50);
  SIM800.print(NUMERO_A_BORRAR);   //  "\"\r"
  delay(100);
  SIM800.write(0x1A);           // sends ctrl+z end of message
  delay(4000); 
}
void NUMERO_NO_ENCONTRADO()
{
  int x;  
  
  SIM800.print("AT+CMGS=\"+52");   //
  delay(100);
                                     x=0;                           //POSICION DEL ADMIN LA 0
                                         for(int p=0;p<10;p++){ 
                                          EEPROM_BUFFER[p]=EEPROM.read(x);
                                                  x+=101;                    
                                                                    }//for
  
  SIM800.print(EEPROM_BUFFER);   //  "\"\r"
  delay(100);
  SIM800.print("\"\r");   //
  delay(1000);
  SIM800.print("El numero no se encuentra en la memoria favor de verficar :)");           //SMS body
  delay(50);
  delay(100);
  SIM800.write(0x1A);           // sends ctrl+z end of message
  delay(4000); 
}

void UPDATE_EEPROM()
{
 
  for(int x=1;x<=REGISTROS_MAXIMOS;x++){             //SI EL SISTEMA SE APAGA EL FOR DETERMINA EL VALOR DE CUANTAS POSICIONES DE LA EEPROM ESTAN OCUPADAS (INDEX)
                                                                                              
    if(EEPROM.read(x) == 0){
         ADRR_EEPROM_OCUPADO++; 
                                                                                                                                                                     
                                 }
                                      }
  EEPROM.write(1023,0);    
}

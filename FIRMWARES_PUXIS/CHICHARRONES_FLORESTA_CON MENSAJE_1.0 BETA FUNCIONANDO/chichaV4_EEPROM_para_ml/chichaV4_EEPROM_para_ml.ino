#include <SoftwareSerial.h>


SoftwareSerial SIM800(4, 15); // RX, TX

char   MENSAJE[60];
char   CELULAR[21];
char   BODY[25];
char   CELULAR_ALTA[25];

//String IMEI;
String CELULAR_ALTA_STRING;
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

  pinMode(2,  OUTPUT);                               //
  digitalWrite(2, LOW);
  delay(2000);
  digitalWrite(2, HIGH);
  delay(6000);


  SIM800.begin(9600);

  SIM800.println("AT");                             //
  delay(10000);


  SIM800.println("AT+CMGD=1");          //BORRA LA MEMORIA DONDE ESTAN LOS MENSAJES
  delay(4000);


  SIM800.println("AT+CMGF=1");              //MENSAJE MODO TEXTO
  delay(2000);

  SIM800.println("AT+CNMI=2,2");          //MUESTRA EL MENSAJE COMPLETO TXT
  delay(2000);


  SETUP_800();

}

void loop() {

  COPIA_USART(1, 90);


  delay(500);


  if (strcasestr(BUFFER_USART2, "+CMT:")) {
                         
                          for (int i = 9; i < 19; i++) {
                                      CELULAR[i - 9] = BUFFER_USART2[i];
                                                                             }

                                                          for (int i = 47; i < 68; i++) {
                                                                   BODY[i - 47] = BUFFER_USART2[i];
                                                                                                           }


                                                            if (strcasestr(BODY, "Alta@:")) {



                                                            for (int i = 7; i < 20; i++) {

                                                                  if (BODY[i] == 0x0D) {
                                                                                BODY[i]= 0x00;   
                                                                                                      }
                                                                       CELULAR_ALTA[i - 7] = BODY[i];
                                                                       CELULAR_ALTA_STRING=CELULAR_ALTA;                               
                                                                                                       }



                                                                                                                       if(CELULAR_ALTA_STRING.length() < 10 || CELULAR_ALTA_STRING.length() > 10) { 
                                                                                                                       ALTA_INCORRECTA();
                                                                                                                                                                      }                                         

                                                                                                                       else{
                                                                                                                       ALTA_ALARMA();
                                                                                                                       asm volatile ( "jmp 0");
                                                                                                                                         }                                            
                                                                                                                                 
                                                                                                                                 
                                                                                                   }



    if (strcasestr(BODY, "Reset")) {
   
      asm volatile ( "jmp 0");




                                                  }



  }











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

  //COPIA_USART(10,24);
  SIM800.println(F("AT+GSN"));                     //
  delay(1000); //

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
void ALTA_ALARMA()
{
  //SIM900.write("AT+CMGF=1\r");           //set GSM to text mode
  //delay(1000);
  SIM800.print("AT+CMGS=\"+522299536830\"\r");   //phone number to which you want to send sms
  delay(1000);
  SIM800.print("Alta correcta del numero:");           //SMS body
  delay(50);
  SIM800.print(CELULAR_ALTA);           //SMS body
  delay(500);
  SIM800.write(0x1A);           // sends ctrl+z end of message
  delay(3000);
}

void ALTA_INCORRECTA()
{
  //SIM900.write("AT+CMGF=1\r");           //set GSM to text mode
  //delay(1000);
  SIM800.print("AT+CMGS=\"+522299536830\"\r");   //phone number to which you want to send sms
  delay(1000);
  SIM800.print("Numero invalido");           //SMS body
  delay(50);
  SIM800.write(0x1A);           // sends ctrl+z end of message
  delay(3000);
}


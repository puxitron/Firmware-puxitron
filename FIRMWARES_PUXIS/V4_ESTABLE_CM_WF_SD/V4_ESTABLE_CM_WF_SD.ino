#include <SoftwareSerial.h>
#include <SD.h>
#include <SPI.h>
#include <MemoryFree.h>

String CONTEO_STRING;
char   CONTEO_BUFFER  [11];
int    x=0;
long int   CONTEO_IMAGEN ;


String ARCHIVO;

SoftwareSerial COMANDOS_CAMARA(5, 6);  
SoftwareSerial WIFI(8,7); // RX, TX 

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
volatile int contador = 0;   // Somos de lo mas obedientes
int n = contador ;
long T0 = 0 ;  // Variable global para tiempo
int OCUPADO=0;
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const unsigned int LIMITE_BUFFER = 37;
char JPG[12];
char JPG_SD[12];
char JPG_23[]="35.JPG";
String INDEX; 
String JPG_STRING; 
int ENVIO_INDEX_NAVEGADOR;
int ENVIO_IMAGEN_NAVEGADOR; 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
long int p=0x0000,j=0,k=0,count=0,i=0;
uint8_t MH,ML;
boolean EndFlag=0;
byte ZERO = 0x00;                                          //ERROR EN AMBIGUEDAD EN EL SERIAL// SE DECLARA UN BYTE EN CERO 0X00  
byte incomingbyte;                                         //BUFFER PARA GUARDAR EL EXADECIMAL QUE ARROJA LA CAMARA EN EL ERIAL RX TX 
String  FORMATO_IMAGEN[] = ".jpg";
char    BUFFER_CONTADOR[11];
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void setup(){ 

            
      Serial.begin(115200);
      WIFI.begin(115200);                       //INICIA EL SERIAL WIFI A 115200
      COMANDOS_CAMARA.begin(38400);
      SETUP_WIFI();                             //COMANDOS PARA EL WIFI ACTIVA EL SERVIDOR WEB
//////////////////////////////////////////////////////////////////////////////////////////////////// //////////////////////////////////////////////////////////     
      Serial.println(F("Ini..MicroSD..."));
      pinMode(10, OUTPUT);                                     //PIN SE DEJA COMO SALIDA 
      delay(1000);
      if (!SD.begin(4)) {            //CHECK PARA VER SI ESTA LA MEMORIA INSERTADA RETORNA UN VALOR LA FUN SD.BEGIN
                          Serial.println(F("Error-MicroSD"));
                          return;
                                 }
      Serial.println(F("MicroSD-OK......"));
 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      attachInterrupt(digitalPinToInterrupt(2), ServicioBoton, RISING);  

}

void loop(){

  COMANDOS_CAMARA.listen();
      
    if(OCUPADO !=0){
      delay(1000); 
        CREDENCIAL();
          OCUPADO=0;
                       }//*/

 
    if(OCUPADO !=1){                 
      WIFI.listen(); 
      if(Serial.available () > 0){
       BYTE_LEIDO_BUFFER_SERIAL (Serial.read ());
                                    }

                        }            

    
    //Serial.print(F("Free Memory = "));
    //Serial.println(getFreeMemory());

 


  
}





////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CREDENCIAL()
{

File  CAM;  

 detachInterrupt(0);  
/////////////////////
       if (!SD.exists("X.txt")) {        
        CAM = SD.open("X.txt", FILE_WRITE);             //ABRE LA SD CREA Y ESCRIBE SOBRE EL ARCHIVO    
        CAM.seek(0);
          CAM.print("0");
            CAM.close();
             Serial.println(F("Archivo creado"));    
               CAM.close();         
                                      }


          else{
           Serial.println(F("Txt en root"));  
                          }                               


       if (CAM= SD.open("X.txt")) {          
        while (CAM.available()) {
          //Serial.println(CAM.available()); 
           CONTEO_BUFFER[x]=CAM.read();
            x++;
                                     }
                x=0;
                  CAM.close();                      
                                                   }
   
   
       else{
           CAM.close(); 
                        }                          

   CONTEO_STRING=CONTEO_BUFFER;                  //STRING IGUAL AL ARRAY CHAR
   CONTEO_IMAGEN=CONTEO_STRING.toInt();          //CONVERCION DE STRING A ENTERO
//////////////////// 



//  String str;
//  str=String(CONTEO_IMAGEN);
  CONTEO_STRING+=".jpg";
  CONTEO_STRING.toCharArray(BUFFER_CONTADOR,11);
   
  byte a[32];
  int ii;
  //RESET();
  //delay(2000);  
  TOMA_FOTO();                              //COMANDO PARA INICIAR LA FOTOGRAFIA
  delay(20);                                //TIEMPO DE ESPERA DESPUES DE ENVIAR UN COMANDO A LA CAMARA
      
      while(COMANDOS_CAMARA.available()>0){
       incomingbyte=COMANDOS_CAMARA.read();
                                            }
   //CAM= SD.open(BUFFER_CONTADOR, FILE_WRITE);   //ABRE LA SD CREA Y ESCRIBE SOBRE EL ARCHIVO
   CAM= SD.open(BUFFER_CONTADOR, FILE_WRITE);   //ABRE LA SD CREA Y ESCRIBE SOBRE EL ARCHIVO
 
    while(!EndFlag){
    j=0;
    k=0;
    count=0;
    SendReadDataCmd(); 
    delay(20); 
 
          while(COMANDOS_CAMARA.available()>0){
           incomingbyte=COMANDOS_CAMARA.read();
            k++;
            delayMicroseconds(300);
            
            if((k>5)&&(j<32)&&(!EndFlag)){
            a[j]=incomingbyte;

                              if((a[j-1]==0xFF)&&(a[j]==0xD9)){     
                              EndFlag=1;
                                                                    }
            j++;
            count++;
                                                      } //IF K5 
                                                              }//WHILE
 
      for(j=0;j<count;j++){
      if(a[j]<0x10)
      Serial.print("0");
      Serial.print(a[j],HEX);                // IMPRIME EN EL SERIAL LA FOTO MEDIANTE VALORES HEXADECIMALES 
      Serial.print("");
                               }
 
      for(ii=0; ii<count; ii++)
      CAM.write(a[ii]); 
      Serial.println();
      //i++;                                  //UNA FRANJA       

 
                                          }//WHITE FLAG

    
    StopTakePhotoCmd();
    delay(300); 
    CAM.close();
    //Serial.println(""); 
    //Serial.println(F("OK ")); 
    //Serial.println(BUFFER_CONTADOR);
    //delay(1000);
   /////////////////////////////////////////////////////////////////////////////////////////////////////
    EndFlag=0;
    j=0;
    k=0;
    count=0;
    p=0x0000;
    ii=0;
    
    /////////////////////////////////////////////////////////////////////////////////////////////////////
              //  }//IF

    
   CONTEO_IMAGEN++;                             //AUMENTA EL NUMERO CONFORME TOMA FOTO
   
   Serial.println(CONTEO_IMAGEN);                           


      if (CAM = SD.open("X.txt", FILE_WRITE)) { //ESCRIBE EN LA SD EL NUMERO DE LA IMAGEN EN LA QUE VA             
        CAM.seek(0);
          CAM.print(CONTEO_IMAGEN);
          CAM.close();
                                                 }

      else{
            CAM.close(); 
                          }//*/
    
    
    
    
    attachInterrupt(digitalPinToInterrupt(2), ServicioBoton, RISING);       
}//CAMRA*/

///////////////////////////////////////////////////////////////////////////////////////////////////////////
void ServicioBoton()
   {
       if ( millis() > T0  + 250)
          {   OCUPADO=1;
              T0 = millis();
          }
    }
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void BYTE_LEIDO_BUFFER_SERIAL (const byte BYTE_SERIAL){
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
  int x=15;
  char FIN=' ';
  int i=-1;
  static char BUFFER_WIFI_INDEX [LIMITE_BUFFER];
  static unsigned int POSICION_ARRAY =0;

                                         
                                          
                                          switch (BYTE_SERIAL){

                                          case '\n':                                  //VERIFICO SI ES NUEVA LINEA
                               
                                          INDEX=BUFFER_WIFI_INDEX;
                                          BUFFER_WIFI_INDEX [POSICION_ARRAY] = 0;    //PONE A CERO LA ULTIMA POS QUE CORRESPONDE AL NEW LINEA
                                          IMPRIME_DATO (BUFFER_WIFI_INDEX);
                                          POSICION_ARRAY = 0;                        //RESETEA EL CONTADOR PARA EL COPIADO AL ARRAY
                                          
                                                                
                                         //////////////////////////////////////////////////////////////////////////////// 
                                        if (INDEX.substring(0,6) == "+IPD,0") {     //VERIFICA SI SE SOLICITO LA INDEX PARA MANDARLA AL NAVEGADOR
                                 
                                                   if (INDEX.substring(10) == ":GET /index HTTP/1.1") { 
                                                      //Serial.println("Envio index");
                                                        INDEX_WEB();
                                                        //ENVIO_INDEX_NAVEGADOR=1;
                                                        INDEX=" ";                                               //LIMPIO INDEX STRING                  
                                                                                                            }//IF
                                                 
                                                                                                   
                                                   
                                                   else{
                                                       while(true){
                                                        JPG[i++] = BUFFER_WIFI_INDEX[x++];
                                                                                                 if(BUFFER_WIFI_INDEX[x] == ' '){       
                                                                                                 break;           
                                                                                                                                    }
                                                                   }//FOR COPIADO JPG
                                                           
                                                           for(int x=0;x<sizeof(JPG)-1;x++){
                                                           JPG_SD[x] =JPG[x]; // BUFFER JPG
                                                                                                                                           
                                                                                              }
                                                           
                                                           //Serial.print("Buscando imagen:");
                                                           //Serial.println(JPG);
                                                           INDEX=" ";         //LIMPIO INDEX STRING   
                                                           //ENVIO_IMAGEN_NAVEGADOR =1; 
                                                           OPEN_ARCHIVO();
                                                           for(int x=0;x<sizeof(JPG)-1;x++){
                                                           JPG[x] =0x00; //LIMPIO EL BUFFER JPG
                                                                                                                                           
                                                                                                 }//LIMPIO EL BUFFER JPG
                                                         }//GET RAIZ                                                 
                                                                         
                                                        
                                                                                            }//IF DEL GET PRINCIPAL   
                                                                                        
                                           for(int x=0;x<LIMITE_BUFFER;x++){
                                           BUFFER_WIFI_INDEX [x] =0x00; //LIMPIO EL BUFFER BUFFER_WIFI_INDEX
                                                                                                                                      
                                                                                       }//FOR
                                          ////////////////////////////////////////////////////////////////////////////////  
                                          break;

                                          
                                          case '\r':   
                                          break;

                                          
                                          
                                          
                                          default:                                     //SI ES CUALQUIER CARACTER LO ESCRIBER EN EL BUFFER INDEXY EN LA POSICION INDICADA
                                          if (POSICION_ARRAY < (LIMITE_BUFFER - 1)){
                                          BUFFER_WIFI_INDEX [POSICION_ARRAY++] = BYTE_SERIAL;
                                          break;
                                          
                                                                                         }//IF                                                
                                                                                       

                                                                                            }//SWITCH  
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////                                                                                            
} 


void IMPRIME_DATO (const char * data){

  Serial.println (data);
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////  
}  

////////////////////////////////////////////////////////////////////////////////////////////////////////////
void INDEX_WEB() {    
File root; 

    WIFI.println(F("AT+CIPSEND=0,15"));                       // AT+CIPSEND=0, num
    delay(20);
    
            WIFI.println(F("<!DOCTYPE html>"));                     //Aqui va el string hacia el WIFI 
                    while ( Serial.available() > 0 ){ 
                           if (  Serial.find("SEND OK") )    // Busca el OK en la respuesta del wifi
                                        break; 
                                                           }
                                                           
    WIFI.println(F("AT+CIPSEND=0,6"));                       // AT+CIPSEND=0, num
    delay(20);
    
            WIFI.println(F("<html>"));                     //Aqui va el string hacia el WIFI 
                    while ( Serial.available() > 0 ){ 
                           if (  Serial.find("SEND OK") )    // Busca el OK en la respuesta del wifi
                                        break; 
                                                           }

    WIFI.println(F("AT+CIPSEND=0,42"));                       // AT+CIPSEND=0, num
    delay(20);
    
            WIFI.println(F("<body style=\"background-color:DimGray;\">"));                     //Aqui va el string hacia el WIFI 
                    while ( Serial.available() > 0 ){ 
                           if (  Serial.find("SEND OK") )    // Busca el OK en la respuesta del wifi
                                        break; 
                                                           }

    /*WIFI.println(F("AT+CIPSEND=0,87"));                       // AT+CIPSEND=0, num
    delay(20);
    
            WIFI.println(F("<button type=\"button\" onclick=\"alert('Borrando Memoria SD')\">Formatiar memoria</button>"));                     //Aqui va el string hacia el WIFI 
                    while ( Serial.available() > 0 ){ 
                           if (  Serial.find("SEND OK") )    // Busca el OK en la respuesta del wifi
                                        break; 
                                                           }*/

     /*WIFI.println(F("AT+CIPSEND=0,99"));                       // AT+CIPSEND=0, num
    delay(20);
    
            WIFI.println(F("<form action=\"192.168.4.1/SD_F\" method=\"post\"><div><button type=\"submit\">Post</button></div></form>"));               //Aqui va el string hacia el WIFI 
                    while ( Serial.available() > 0 ){ 
                           if (  Serial.find("SEND OK") )    // Busca el OK en la respuesta del wifi
                                        break; 
                                                           }*/
                                                          

    
     
     // http("<!DOCTYPE html>");
     //http("<html>");
     //http("<body style=\"background-color:DimGray;\">");    
     //http("<center><p style=\"font-size:160%;\"><a href=\"dinero.png\"style=\"color:black\">12313.jpg</a></p></center>");  //PARA PRUEBAS
     root = SD.open("/"); //INDICA QUE ABRE LA MEMORIA SD   
     root.rewindDirectory();
     while(1) {
     File entry =  root.openNextFile();
                                               if (! entry) {
                                               //root.rewindDirectory();
                                               break;
                                                                   }
     ARCHIVO=entry.name();
     http("<center><p style=\"font-size:160%;\"><a href=\""+ARCHIVO+"\"style=\"color:black\">"+ARCHIVO+"</a></p></center>");  //PARA PRUEBAS
     entry.close(); 
                    }

     http("</body>");
     http("</html>");
     delay(20);
     WIFI.println("AT+CIPCLOSE=0"); 
 
              while ( Serial.available() > 0 ){ 
                      if (Serial.find("0,CLOSED"))    // Busca el OK en la respuesta del wifi

                              break; 
                                                } 

root.close();                                                  

}//FUNCION


//////////////////////////////////////////////////////////////////////////////////////////////////////////
void OPEN_ARCHIVO(){
File  FOTO; 

  int ACUMULADOR=0;
  int SIZE_IMAGEN=0; 
  int BYTE_IMAGEN=0;     
  int RESTANTE_DE_LA_IMAGEN;

  Serial.println(JPG_SD);


if (JPG_SD[0] != ' ') {    
  
  FOTO= SD.open(JPG_SD);                     //ABRE EL ARCHIVO DESDE LA MEMORIA SD               
 
  if (FOTO) {                                //SI EL ARCHIVO ESTA EN LA RAIZ IMPRIME OK

  SIZE_IMAGEN=(FOTO.size());
  Serial.print(F("TAMANO "));
  Serial.println(SIZE_IMAGEN);
  WIFI.println(F("AT+CIPSEND=0,2000"));
  delay(250);  


                                                     
                                                    while (FOTO.available()) {                        //SI ESTA DISPONIBLE LO ABRE 
                                                    BYTE_IMAGEN++;
                                                    ACUMULADOR++;
                                                      WIFI.write(FOTO.read());
                                                      //delayMicroseconds(600); 
                                                      
                                                                                      
                                                                                      
                                                                                      
                                                                                      
                                                                                        if (BYTE_IMAGEN == 2000){  
                                                                                        RESTANTE_DE_LA_IMAGEN=FOTO.available();
                                                                                          Serial.print(F("RESTAN "));
                                                                                          Serial.println(RESTANTE_DE_LA_IMAGEN);      
                                                                                            BYTE_IMAGEN=0;   
                                                                                               WIFI.println();
                                                                                                                          delay(600);

                                                                                                      
                                                                                                                      
                                                                                                   
                                                                                                    if(RESTANTE_DE_LA_IMAGEN < 2049){                                       
                                                                                                     String SEND(RESTANTE_DE_LA_IMAGEN);                //CONVIERTE ENTERO A STRING       
                                                                                                      WIFI.print(F("AT+CIPSEND=0,"));
                                                                                                         WIFI.println(SEND);
                                                                                                         delay(600);
                                                                                                                                             } 
           
                                                                                                      
                                                                                                                              }//CHECK BYTE ENVIO   */
                                                                                                                              
                                                                                        if (ACUMULADOR == SIZE_IMAGEN){  
                                                                                         BYTE_IMAGEN=0;
                                                                                         ACUMULADOR=0;
                                                                                                WIFI.println();         
                                                                                                                        while ( Serial.available() > 0 ){ 
                                                                                                                             if (Serial.find("SEND OK"))    // Busca el OK en la respuesta del wifi
                                                                                                                                     break; 
                                                                                                                                                                 }                                   
                                                                                                                       }//CHECK BYTE ENVIO   */                    
                       
                                                                                              
                                                         
                                                                                                                                                
                                                                               }//WHILE ENVIO BYTES
                                                                                      
                                                                                            

   FOTO.close();
   delay(100);
   Serial.println(F(""));
   WIFI.println(F("AT+CIPCLOSE=0")); 
   delay(350);
   BYTE_IMAGEN=0;


                             

  
   }//IF PARA VER SI ESTA LA JPG EN LA MEMORIA SI NO PASA AL ELSE 

    else { 
          Serial.println(F(""));
          Serial.println(F("JPG ERROR"));
          FOTO.close();
              
                                    WIFI.println(F("AT+CIPCLOSE=0")); 
 
                                        while ( Serial.available() > 0 ){ 
                                             if (Serial.find("0,CLOSED"))    // Busca el OK en la respuesta del wifi
                                                       break; 
                                                                           }
           }



}//IF FOTO CHECK SI ESTA VACIO
FOTO.close();
}//FUNCION


//////////////////////////////////////////////////////////////////////////////////////////////////////////
void http(String output){
    WIFI.print(F("AT+CIPSEND=0,"));                       // AT+CIPSEND=0, num
    WIFI.println(output.length());
    delay(30);
  
    //if (Serial.find(">"))                           // Si recibimos la peticion del mensaje
      // {    
            //Serial.println(output);
            WIFI.println(output);                     //Aqui va el string hacia el WIFI 
            //delay(10);
                    while ( Serial.available() > 0 ){ 
                           if (  Serial.find("SEND OK") )    // Busca el OK en la respuesta del wifi
                                        break; 
                                                           }
       //}


}//FUNCION


void SETUP_WIFI(){ 
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      delay(3000);                              
        
      WIFI.println(F("AT+CWMODE=3"));                    
       delay(500);
        while(Serial.available()) {
          Serial.write(Serial.read());
                                        }

      WIFI.println(F("AT+CWSAP=\"nk6\",\"00000000\",1,0"));                 //NOMBRE DE LA RED,CANAL Y WPA2 PARA LA CONTRASEÑA                    
       delay(500);
        while(Serial.available()) {
          Serial.write(Serial.read());
                                        }

      WIFI.println(F("AT+CIPMUX=1"));                    
       delay(500);
        while(Serial.available()) {
          Serial.write(Serial.read());
                                        } 
      WIFI.println(F("AT+CIPSERVER=1,80"));                               //INICIA EL MODO SERVIDOR HTTP         
       delay(500);
        while(Serial.available()) {
          Serial.write(Serial.read());
                                        }

      WIFI.println(F("AT+CIPSTO=2"));                                   //CIERRA EL SOCKET DESPUES DE 3SEG DE INACTIVIDAD          
       delay(500);
        while(Serial.available()) {
          Serial.write(Serial.read());
                                        }                                                                                                                                            
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////   
}


void RESET()
{
COMANDOS_CAMARA.write(0x56);
COMANDOS_CAMARA.write(ZERO);
COMANDOS_CAMARA.write(0x26);
COMANDOS_CAMARA.write(ZERO);
}
 
void RESOLUCION_JPEG_320()
{
COMANDOS_CAMARA.write(0x56);
COMANDOS_CAMARA.write(ZERO);
COMANDOS_CAMARA.write(0x31);
COMANDOS_CAMARA.write(0x05);
COMANDOS_CAMARA.write(0x04);
COMANDOS_CAMARA.write(0x01);
COMANDOS_CAMARA.write(ZERO);
COMANDOS_CAMARA.write(0x19);
COMANDOS_CAMARA.write(0x22);
}
 
void VELOCIDAD_CAM_38400()
{
COMANDOS_CAMARA.write(0x56);
COMANDOS_CAMARA.write(ZERO);
COMANDOS_CAMARA.write(0x24);
COMANDOS_CAMARA.write(0x03);
COMANDOS_CAMARA.write(0x01);
COMANDOS_CAMARA.write(0x2A);
COMANDOS_CAMARA.write(0xF2);
 
}
void VELOCIDAD_CAM_57600()
{
COMANDOS_CAMARA.write(0x56);
COMANDOS_CAMARA.write(ZERO);
COMANDOS_CAMARA.write(0x24);
COMANDOS_CAMARA.write(0x03);
COMANDOS_CAMARA.write(0x01);
COMANDOS_CAMARA.write(0x1C);
COMANDOS_CAMARA.write(0x4C);
 
}

void VELOCIDAD_CAM_115200()
{
COMANDOS_CAMARA.write(0x56);
COMANDOS_CAMARA.write(ZERO);
COMANDOS_CAMARA.write(0x24);
COMANDOS_CAMARA.write(0x03);
COMANDOS_CAMARA.write(0x01);
COMANDOS_CAMARA.write(0x0D);
COMANDOS_CAMARA.write(0xA6);
 
}
 
void TOMA_FOTO()
{
COMANDOS_CAMARA.write(0x56);
COMANDOS_CAMARA.write(ZERO);
COMANDOS_CAMARA.write(0x36);
COMANDOS_CAMARA.write(0x01);
COMANDOS_CAMARA.write(ZERO);
}
 
void SendReadDataCmd()
{
MH=p/0x100;
ML=p%0x100;
COMANDOS_CAMARA.write(0x56);
COMANDOS_CAMARA.write(ZERO);
COMANDOS_CAMARA.write(0x32);
COMANDOS_CAMARA.write(0x0c);
COMANDOS_CAMARA.write(ZERO);
COMANDOS_CAMARA.write(0x0a);
COMANDOS_CAMARA.write(ZERO);
COMANDOS_CAMARA.write(ZERO);
COMANDOS_CAMARA.write(MH);
COMANDOS_CAMARA.write(ML);
COMANDOS_CAMARA.write(ZERO);
COMANDOS_CAMARA.write(ZERO);
COMANDOS_CAMARA.write(ZERO);
COMANDOS_CAMARA.write(0x20);
COMANDOS_CAMARA.write(ZERO);
COMANDOS_CAMARA.write(0x0a);
p+=0x20;
}

void SendReadDataCmd2()
{

COMANDOS_CAMARA.write(0x56);
COMANDOS_CAMARA.write(ZERO);
COMANDOS_CAMARA.write(0x32);
COMANDOS_CAMARA.write(0x0C);
COMANDOS_CAMARA.write(ZERO);
COMANDOS_CAMARA.write(0x0A);
COMANDOS_CAMARA.write(ZERO);
COMANDOS_CAMARA.write(ZERO);
COMANDOS_CAMARA.write(ZERO);
COMANDOS_CAMARA.write(ZERO);
COMANDOS_CAMARA.write(ZERO);
COMANDOS_CAMARA.write(ZERO);
COMANDOS_CAMARA.write(0x0E);
COMANDOS_CAMARA.write(0x0C);
COMANDOS_CAMARA.write(ZERO);
COMANDOS_CAMARA.write(0x01);

}
 
void StopTakePhotoCmd()
{
COMANDOS_CAMARA.write(0x56);
COMANDOS_CAMARA.write(ZERO);
COMANDOS_CAMARA.write(0x36);
COMANDOS_CAMARA.write(0x01);
COMANDOS_CAMARA.write(0x03);
}

void COMPRIME_36()
{
COMANDOS_CAMARA.write(0x56);
COMANDOS_CAMARA.write(ZERO);
COMANDOS_CAMARA.write(0x31);
COMANDOS_CAMARA.write(0x05);
COMANDOS_CAMARA.write(0x01);
COMANDOS_CAMARA.write(0x01);
COMANDOS_CAMARA.write(0x12);
COMANDOS_CAMARA.write(0x04);
COMANDOS_CAMARA.write(0x24);
}

      

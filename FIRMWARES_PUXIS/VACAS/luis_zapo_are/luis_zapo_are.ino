
#include <SPI.h>
#include <Mirf.h>   
#include <nRF24L01.h>
#include <MirfHardwareSpiDriver.h>
#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>
#include <avr/io.h>
#include <TimerOne.h>
#include <OneWire.h> 
#include <DallasTemperature.h>

  int puxitron;
  int NORDIC=0;
  int n;
  int IRQ=2;
  unsigned int x;
 
  byte  config_reg;
  byte  SETUP_RETR_REG;
  byte  status_reg;
  byte  power;
  char  serial;
  float data[16];
  
OneWire ourWire(A2);                                   //ESTABLECE A1 COMO EL DE COMUNICACION
DallasTemperature arete(&ourWire);                    //Se instancia la librería DallasTemperature


void setup() {
  Serial.begin(9600);
  pinMode(A0,INPUT);           //TESTEO DE LA BATERIA
  pinMode(10,OUTPUT);           //
  pinMode(9,OUTPUT);           //SENSOR DE LA TEMP
  pinMode(17,OUTPUT);           //

  
  analogReference(INTERNAL);  //SELECIONAMOS LA REF INTERNA 1.1V 
//  attachInterrupt(0,IRQ_NORDIC,FALLING);           //INTERRUPCION 0 QUE ES EL PIN2 ATIENDE A LA FUNCION IRQ_NORDIC CADA VEZ QUE PASA DE ALTO A BAJO
//  pinMode(IRQ,INPUT);                              //DEFINIMOS EL PIN2 COMO IRQ DEL NRF PARA QUE TRABAJE COMO ENTRADA
/////////////////////////////////////////////////////////////////////////////////////////////////////////// 
  Mirf.spi = &MirfHardwareSpi;
  Mirf.cePin  = 17;
  Mirf.csnPin = 9;
  Mirf.init();
  Mirf.channel = 6;
  Mirf.payload = 32;
  Mirf.config();  
  Mirf.setTADDR((byte *)"00001");
  Mirf.setRADDR((byte *)"MSTER");
  Mirf.configRegister(RF_SETUP, 0x26);
  Mirf.configRegister(SETUP_RETR,0x20);

////////////////////////////////////////////////////////////////////////////////////////////////////////

  MCUSR &= ~(1<<WDRF);
  WDTCSR |= (1<<WDCE) | (1<<WDE);
  WDTCSR = 1<<WDP0 | 1<<WDP3; /* 8.0 se */
  
  WDTCSR &= ~(_BV(WDIE));
///////////////////////////////////////////////////////////////////////////////////////////////////////  
  Timer1.initialize(5000000);            //5MS
  Timer1.attachInterrupt(puxis);         //NOMBRE DE LA INT
  interrupts();                          //ACTIVO LA INT DEL TIMER1

///////////////////////////////////////////////////////////////////////////////////////////////////////
  arete.begin();                         //ACTIVA LOS SENSORES
  arete.setResolution(TEMP_10_BIT);      //SETEO 10BITS DE RESOLUCION             
//////////////////////////////////////////////////////////////////////////////////////////////////////
 }

void loop() {
    

    if (!Mirf.isSending() && Mirf.dataReady()) {
    Mirf.getData((byte *) &data);
    if (data[0] == 1.00) {                                         //NOMBRE ARETE
    arete.requestTemperatures();                                   //PREPARA EL SENSOR PARA REALIZAR LECTURAS
    data[1]=arete.getTempCByIndex(0);                              //SOLICITA LA TEMPERATURA Y LA GUARDA EN data[1] 
    //data[3] =4.9*(analogRead(A0) * 0.001075);                             //BATERIA ADC

    Mirf.send((byte *) &data); 
   /* Mirf.readRegister(CONFIG,&config_reg,8);
    config_reg=bitClear(config_reg,1);
    digitalWrite(17, LOW);
 
    ME_DUERMO();              
    
    config_reg=bitSet(config_reg,1);
    Mirf.configRegister(CONFIG,config_reg);
    digitalWrite(17, HIGH);  */
  
                                                        }

                                                   
                                                        }//if  
                                                     
                                                                 // } 
    

    
    Mirf.readRegister(CONFIG,&config_reg,8);                                         
    Serial.println(config_reg, BIN);                                     //IMPRIME EL REG NRF
    power=bitRead(config_reg,1);
    Serial.println(power); 
    Mirf.readRegister(STATUS,&status_reg,8);    //REGISTRO DEL NRF,BUFFER,TAMÁÑO
    Serial.println(status_reg, BIN); 
    Serial.println(); 
  
   if(Serial.available()){                                                 //RECOG EL VALOR DE LA TERMINAL SERIAL
   serial=Serial.read(); 
                        if(serial==49){                                    //RECOG EL VALOR DE LA TERMINAL SERIAL
                        config_reg=bitSet(config_reg,1);
                                                 }
                        if(serial==48){                                    //RECOG EL VALOR DE LA TERMINAL SERIAL
                       config_reg=bitClear(config_reg,1);
                                                   }                       
   Serial.println(serial);
   Serial.println(config_reg,BIN);
   Serial.println();    
                            }


   Mirf.configRegister(CONFIG,config_reg);                                //REGISTRO DEL NRF,BUFFER,TAMÁÑO                        


                   }// void loop



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ME_DUERMO(void)                       //FUNCION PARA QUE EL ATMEGA ENTRE EN SLEEP Y AHORRE MUCHOS mA
{
for(int c=1;c<2;c++){                       //470 =1hora   2700=6 horas
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);     //TIPO DE AHORRO DE ENERGIA (SON 5 TIPOS )
  sleep_enable();                          //ACTIVO EL MODO SLEEP
  WDTCSR |= _BV(WDIE);                     //ACTIVA LA INT DEL PERRO GUARDIAN       (FORMA NATIVA DEL ATMEGA DE HACER UN BITSET)
  sleep_mode();                            //SE VA A DORMIR EL VATO POR 8SEGUNDOS
                        
  sleep_disable();                         //REGRESA A ESTE PUNTO DESPUES DE DORMIR PUESTO QUE EL WD LO DESPIERTA 
  power_all_enable();  }                     //ACTIVA TODOS LOS PERIFERICOS ON

} 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ISR(WDT_vect)                              //FUNCION DEL WD
{

  WDTCSR &= ~(_BV(WDIE));                  //DESACTIVA LAS INT DEL WD   (FORMA NATIVA DEL ATMEGA DE HACER UN BITCLEAR)

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void puxis(void)
{
puxitron++;
     
}


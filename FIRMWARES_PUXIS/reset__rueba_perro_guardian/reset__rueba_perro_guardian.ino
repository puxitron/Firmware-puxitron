#include <avr/wdt.h>
#include <avr/sleep.h>
#include <avr/power.h>


void setup ()
{
 /* bitSet(MCUSR,3);
  //bitClear(WDTCSR,3);                           //WDE RESET DEL SISTEMA
  bitSet(WDTCSR,4);                          // Watchdog Change Enable
  WDTCSR = 1<<WDP3 | 1<<WDP0;                //CONFIGURADO PARA 8 SEGUNDOS
  bitClear(WDTCSR,6);*/                        

  bitSet(MCUSR,3);
  WDTCSR |= (1<<WDCE) | (1<<WDE);
  WDTCSR = 1<<WDP0 | 1<<WDP3; /* 8.0 se */
  bitClear(WDTCSR,6);           //WDIE INT DESACTIVADA PERROGARDIAN

  delay (500);
  Serial.begin (115200);
  Serial.println ();
  Serial.println ("Entering setup ...");
  pinMode (2, OUTPUT);
} 

void loop ()
{
  Serial.println ();
  Serial.println ("Entering loop ...");
  Serial.println ("HOLA MUNDO1 XD ...");
  digitalWrite (2, HIGH);
  delay (500);
  digitalWrite (2, HIGH);
  delay (500);
  ATMEGA328_DUERME();
  Serial.println ("Entering loop ...");
  Serial.println ("HOLA MUNDO2 XD ...");
  software_reset ();  // oops, want to reset the system
} 


void software_reset()
{
  
  wdt_reset(); 
  wdt_enable(WDTO_2S);
  while  (1);{
                }
}  
void ATMEGA328_DUERME(void)                 //FUNCION PARA QUE EL ATMEGA ENTRE EN SLEEP Y AHORRE MUCHOS mA
{
for(int x=0;x<1;x++){
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);     //TIPO DE AHORRO DE ENERGIA (SON 5 TIPOS )
  sleep_enable();                          //ACTIVO EL MODO SLEEP
  bitSet(WDTCSR,6);                        //ACTIVA LA INT DEL PERRO GUARDIAN       
  sleep_mode();                            //SE VA A DORMIR                        
  sleep_disable();                         //REGRESA A ESTE PUNTO DESPUES DE DORMIR PUESTO QUE EL WD LO DESPIERTA 
                          }                //ACTIVA TODOS LOS PERIFERICOS ON
  power_all_enable();
} 
////////////////////////////
ISR(WDT_vect)                              //FUNCION DEL WD
{

 bitClear(WDTCSR,6);                       //DESACTIVA LAS INT DEL WD   

}


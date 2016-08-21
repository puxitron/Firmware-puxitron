#include <UTFT.h>
#include <avr/pgmspace.h>

UTFT myGLCD(ITDB32S,38,39,40,41);
extern uint8_t SmallFont[];
extern uint8_t BigFont[];
extern uint8_t SevenSegNumFont[];
//extern unsigned int VERSE[0x7530];
extern unsigned int atras[0x400];
extern unsigned int ok[0x400];
extern unsigned int herramientas[0x400];

void setup()
{
  myGLCD.InitLCD(PORTRAIT);
       
       

       myGLCD.fillScr(0,0,0);
       
 
       myGLCD.setColor(0,0,0);
       myGLCD.fillRect(202,2,237,317);//x,y
       myGLCD.drawBitmap (206,230, 32, 32, herramientas,360,0,0);
       myGLCD.drawBitmap (206,315, 32, 32, atras,268,0,0);
 
}

void loop()
{
  myGLCD.setColor(0, 255, 0);
  myGLCD.setBackColor(0, 0, 0);
  myGLCD.setFont(SevenSegNumFont);
  myGLCD.print("7",40,300,270);

 // delay(6000);


 
   //delay(6000);
}


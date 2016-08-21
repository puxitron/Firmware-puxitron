#include <EEPROM.h>


int k=-1;
void setup() {
  
Serial.begin(9600);

   
    for(int x=0;x<1024;x++){

      k++;
        Serial.print(EEPROM.read(x));
      
                                         if(k == 100){
                                            Serial.println("");
                                             k=-1;                     
                                                                  }
     
      }
    


}

void loop() {



  //Serial.println("LECTURA EEPROM OK");


  

}

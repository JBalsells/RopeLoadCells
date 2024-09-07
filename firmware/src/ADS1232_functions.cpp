#include <definitions.h>
#include "ILI9341_functions.h"

long readADS1232() {
  while (digitalRead(pinData1));  // Espera a que DOUT esté en bajo
  
  long result = 0;
  for (int i = 0; i < 24; i++) {  // Lee 24 bits de datos
    digitalWrite(pinSCLK1, HIGH);
    result = result << 1;
    digitalWrite(pinSCLK1, LOW);
    if (digitalRead(pinData1)) {
      result++;
    }
  }
  
  // Convierte a entero de 32 bits
  if (result & 0x800000) {  // Si el valor es negativo
    result |= 0xFF000000;
  }
  
  // Pulso extra para terminar la lectura
  digitalWrite(pinSCLK1, HIGH);
  digitalWrite(pinSCLK1, LOW);
  
  return result;
}

long auto_calibrate(int samples){
  int i;
  long suma_datos = 0;

  for (i = 0; i < samples; i++) {
    suma_datos += readADS1232();
    }

  return suma_datos/samples;
}

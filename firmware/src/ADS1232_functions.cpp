#include <definitions.h>
#include "ILI9341_functions.h"

long readADS1232(int PinData, int PinSCLK, int PinPOMN) {
  while (digitalRead(PinData));  // Espera a que DOUT esté en bajo
  
  long result = 0;
  for (int i = 0; i < 24; i++) {  // Lee 24 bits de datos
    digitalWrite(PinSCLK, HIGH);
    result = result << 1;
    digitalWrite(PinSCLK, LOW);
    if (digitalRead(PinData)) {
      result++;
    }
  }
  
  // Convierte a entero de 32 bits
  if (result & 0x800000) {  // Si el valor es negativo
    result |= 0xFF000000;
  }
  
  // Pulso extra para terminar la lectura
  digitalWrite(PinSCLK, HIGH);
  digitalWrite(PinSCLK, LOW);
  
  return result;
}

long auto_calibrate(int samples, int PinData, int PinSCLK, int PinPOMN){
  int i = 0;
  long suma_datos = 0;

  for (i = 0; i < samples; i++) {
    suma_datos += readADS1232(PinData, PinSCLK, PinPOMN);
    }

  return suma_datos/samples;
}

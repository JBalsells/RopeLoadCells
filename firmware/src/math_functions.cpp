#include <definitions.h>

double random_normal_generatos(double media, double desviacion) {
    static bool generar = true;
    static float valor1, valor2;

    if (generar) {
        float u1 = random(0, 10000) / 10000.0;
        float u2 = random(0, 10000) / 10000.0;

        float z1 = sqrt(-2.0 * log(u1)) * cos(2.0 * PI * u2);
        valor1 = z1;

        float z2 = sqrt(-2.0 * log(u1)) * sin(2.0 * PI * u2);
        valor2 = z2;

        generar = false;
        return media + z1 * desviacion;
    } else {
        generar = true;
        return media + valor2 * desviacion;
    }
}

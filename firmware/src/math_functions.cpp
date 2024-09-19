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

double interpolation(const std::vector<double>& vec){
    double sum = 0;
    for(int i=0; i<vec.size();i++){
        sum += vec[i];
    }
    
    return (double)sum/(double)vec.size();
}

double load_scale(long adc_value = 0) {
    const float proportionality_constant = 1.2;
    const double gravity = 9.81;
    const int LOAD_CELL_SCALE = 500;
    const long ADC_SCALE = (1L << 23);

    double scale = gravity*proportionality_constant*((double)adc_value / (double)ADC_SCALE) * LOAD_CELL_SCALE;

    return round(scale * 10000.0) / 10000.0;
}

std::vector<double> normalize_vector(const std::vector<double>& vec, int new_min = -50, int new_max = 50) {
    std::vector<double> vector_normalizado;

    if (vec.empty()) return vector_normalizado; // Retornar un vector vacío si el vector original está vacío

    // Encuentra el valor mínimo y máximo en el vector
    int viejo_min = *std::min_element(vec.begin(), vec.end());
    int viejo_max = *std::max_element(vec.begin(), vec.end());

    // Evitar la división por cero si todos los valores son iguales
    if (viejo_min == viejo_max) {
        vector_normalizado.resize(vec.size(), new_min); // Todos los valores serán igual al nuevo mínimo
        return vector_normalizado;
    }

    // Normaliza los valores y los almacena en el nuevo vector
    vector_normalizado.reserve(vec.size());
    for (int valor : vec) {
        // Normaliza el valor
        int valor_normalizado = new_min + (valor - viejo_min) * (new_max - new_min) / (viejo_max - viejo_min);

        // Limitar el valor a los rangos -50 a 50
        if (valor_normalizado > new_max) {
            valor_normalizado = new_max;
        } else if (valor_normalizado < new_min) {
            valor_normalizado = new_min;
        }

        vector_normalizado.push_back(valor_normalizado);
    }

    return vector_normalizado;
}

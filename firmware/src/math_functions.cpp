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

std::vector<int> normalize_vector(const std::vector<int>& vec, int new_min = -50, int new_max = 50) {
    std::vector<int> vector_normalizado;

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
        int valor_normalizado = new_min + (valor - viejo_min) * (new_max - new_min) / (viejo_max - viejo_min);
        vector_normalizado.push_back(valor_normalizado);
    }

    return vector_normalizado;
}

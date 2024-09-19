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

double calculateMean(const std::vector<double>& data) {
    if (data.empty()) return 0.0;
    
    double sum = 0.0;
    for (double value : data) {
        sum += value;
    }
    return sum / data.size();
}

std::pair<double, double> calculateStandardDeviation(const std::vector<double>& data) {
    if (data.empty()) return {0.0, 0.0};

    double mean = calculateMean(data);
    double sumSquaredDiffs = 0.0;

    for (double value : data) {
        double diff = value - mean;
        sumSquaredDiffs += diff * diff;
    }
    double variance = sumSquaredDiffs / data.size();
    double stdDev = std::sqrt(variance);
    return {mean, stdDev};
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

double clamp(double valor, double min_val, double max_val) {
    if (valor < min_val) return min_val;
    if (valor > max_val) return max_val;
    return valor;
}

std::tuple<std::vector<double>, std::vector<double>, int> normalize_vectors(
    const std::vector<double>& vec1, const std::vector<double>& vec2, 
    int new_min = -50, int new_max = 50) {

    std::vector<double> vector_normalizado1;
    std::vector<double> vector_normalizado2;

    if (vec1.empty() && vec2.empty()) return {vector_normalizado1, vector_normalizado2, 0.0};

    double viejo_min = std::min(*std::min_element(vec1.begin(), vec1.end()), *std::min_element(vec2.begin(), vec2.end()));
    double viejo_max = std::max(*std::max_element(vec1.begin(), vec1.end()), *std::max_element(vec2.begin(), vec2.end()));

    // Si viejo_min es igual a viejo_max, entonces todos los valores son iguales
    if (viejo_min == viejo_max) {
        vector_normalizado1.resize(vec1.size(), new_min);
        vector_normalizado2.resize(vec2.size(), new_min);
        return {vector_normalizado1, vector_normalizado2, new_min};
    }

    // Calcular el valor normalizado correspondiente a cero
    double valor_normalizado_cero = new_min + (0 - viejo_min) * (new_max - new_min) / (viejo_max - viejo_min);
    valor_normalizado_cero = clamp(valor_normalizado_cero, static_cast<double>(new_min), static_cast<double>(new_max));

    // Normaliza los valores del primer vector
    vector_normalizado1.reserve(vec1.size());
    for (double valor : vec1) {
        double valor_normalizado = new_min + (valor - viejo_min) * (new_max - new_min) / (viejo_max - viejo_min);
        valor_normalizado = clamp(valor_normalizado, static_cast<double>(new_min), static_cast<double>(new_max));
        vector_normalizado1.push_back(valor_normalizado);
    }

    // Normaliza los valores del segundo vector
    vector_normalizado2.reserve(vec2.size());
    for (double valor : vec2) {
        double valor_normalizado = new_min + (valor - viejo_min) * (new_max - new_min) / (viejo_max - viejo_min);
        valor_normalizado = clamp(valor_normalizado, static_cast<double>(new_min), static_cast<double>(new_max));
        vector_normalizado2.push_back(valor_normalizado);
    }

    return {vector_normalizado1, vector_normalizado2, valor_normalizado_cero};
}

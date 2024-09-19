#ifndef MATH_FUNCTIONS
#define MATH_FUNCTIONS

    double random_normal_generator(double media, double desviacion);
    double load_scale(long adc_value=0);
    double interpolation(const std::vector<double>& vec);
    std::pair<std::vector<double>, std::vector<double>> normalize_vectors(
        const std::vector<double>& vec1, const std::vector<double>& vec2, 
        int new_min = -60, int new_max = 60);
#endif

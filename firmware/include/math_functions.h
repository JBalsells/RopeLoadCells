#ifndef MATH_FUNCTIONS
#define MATH_FUNCTIONS

    double random_normal_generator(double media, double desviacion);
    std::vector<double> normalize_vector(const std::vector<double>& vec, int new_min = -60, int new_max = 60);
    double load_scale(long adc_value=0);
    double interpolation(const std::vector<double>& vec);

#endif

#include "LineInterpolation.h"

LineInterpolation::LineInterpolation(const std::vector <double> array_param, const std::vector <double> array_time, const double dt) {
    this->array_param = array_param;
    this->array_time = array_time;
    this->dt = dt;
}

/// @brief line_interpolation - метод линейной интерполяции
/// @param array_param - синтетический ряд изменения параметра
/// @param array_time - временной ряд изменения параметра
/// @return interpolation_param - интерполированный параметр
double LineInterpolation::line_interpolation()
{
    /// @param interpolation_param - интерполированный параметр
    double interpolation_param{};
    /// @param size_array_param - размер синтетического временного ряда параметра
    int size_array_param = array_time.size();
    // Проверка наличия элементов в синтетическом ряде при интерполяции
   
    // Выбор прямой, на которой интерполируется расход
    for (int i = 1; i < size_array_param; i++) {
            if (dt >= array_time[i - 1] && dt <= array_time[i]) {
                interpolation_param = (dt - array_time[i - 1]) / (array_time[i] - array_time[i - 1]) * (array_param[i] - array_param[i - 1]) + array_param[i - 1];
            }
     }
    return interpolation_param;
}
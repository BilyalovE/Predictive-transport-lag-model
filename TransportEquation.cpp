#include "TransportEquation.h"

TransportEquation::TransportEquation(const Pipeline_parameters& pipe, const std::vector <double> volumeFlow, const std::vector <double>)
{
    /// @param volumeFlow - объемный расход
    this->volumeFlow = volumeFlow;
    ///@param n - количество точек расчетной сетки;
    this->n = pipe.n;
    /// @param pipeline_characteristics - параметры трубопровода
    this->pipe = pipe;
}

void TransportEquation::methodCharacteristic(vector<double>& current_layer, vector<double>& previous_layer, double left_condition_sulfar, int j)
{
  /*  vector<double> current_layer = buffer.current();
    vector<double> previous_layer = buffer.previous();*/
    // Получение ссылок на текущий и предыдущий слои буфера
    this->j = j;
    for (size_t i = 1; i < n; i++)
    {
        current_layer[i] = previous_layer[i - 1];
    }
    // Слой current_layer на следующем шаге должен стать предыдущим. 
    // Для этого сместим индекс текущего слоя в буфере на единицу
    current_layer[0] = left_condition_sulfar;
}

/// @brief get_speed - метод расчета скорости по расходу (расход может быть интерполирован)
double TransportEquation::get_speed() {
    double square = pipe.get_inner_square();
    double speed{};
    if (j == 0) {
        speed = (volumeFlow)[0] / square;
    }
    else {
        LineInterpolation flow(volumeFlow, )
        double interpolation_Q = line_interpolation(synthetic_time.volumetric_flow, synthetic_time.time, dt);
        speed = interpolation_Q / square;
    }
    return speed;
}




/// @brief get_dt - метод получения шага времени dt
double TransportEquation::get_dt()
{
    double speed = get_speed();
    dt = pipe.get_dx() / speed;
    return dt;
}
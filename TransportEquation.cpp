#include "TransportEquation.h"

TransportEquation::TransportEquation(const Pipeline_parameters& pipe, const std::vector <double> volumeFlow, const std::vector <double> discreteTime)
{
    this->volumeFlow = volumeFlow;
    this->discreteTime = discreteTime;
    ///@param n - количество точек расчетной сетки;
    this->n = pipe.n;
    /// @param pipeline_characteristics - параметры трубопровода
    this->pipe = pipe;
}

void TransportEquation::methodCharacteristic(vector<double>& current_layer, vector<double>& previous_layer, double left_condition_sulfar)
{
    // Получение ссылок на текущий и предыдущий слои буфера
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
    double speed{0};
    if (dt == 0) {
        speed = (volumeFlow)[0] / square;
    }
    else {

        LineInterpolation flow(volumeFlow, discreteTime, dt);
        double interpolation_Q = flow.line_interpolation();
        speed = interpolation_Q / square;
    }
    return speed;
}

/// @brief get_dt - метод получения шага времени dt
double TransportEquation::get_dt()
{
    double speed = get_speed();
    this->dt += pipe.get_dx() / speed;
    return dt;
}

/// @brief transportDelay - транспортное запаздывание
/// @return timeDelayPredict - прогнозируемое транспорное запаздывание
double TransportEquation::transportDelay() {
    double speed = get_speed();
    /// @param pathWayOnLastLayers - пройденный путь на пред
    double pathWayOnLastLayers { 0 };
    double timeDelayPredict = (pipe.L - pathWayOnLastLayers - speed * dt )/ speed;
    pathWayOnLastLayers += speed * dt;
    return timeDelayPredict;
}
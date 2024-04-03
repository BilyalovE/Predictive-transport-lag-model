#include "TransportEquation.h"

TransportEquation::TransportEquation(Pipeline_parameters& pipeline_characteristics, double volumeFlow, int j)
{
    /// @param volumeFlow - объемный расход
    this->volumeFlow = volumeFlow;
    ///@param n - количество точек расчетной сетки;
    /*this->n = pipeline_characteristics.n;*/
    /// @param j - счетчик слоя
    this->j = j;
    /*/// @param pipeline_characteristics - параметры трубопровода
    this->pipeline_characteristics = pipeline_characteristics;
    */
}

void TransportEquation::methodCharacteristic(ring_buffer_t <vector<double>> buffer, double left_condition_sulfar)
{
    vector<double> current_layer = buffer.current();
    vector<double> previous_layer = buffer.previous();
    // Получение ссылок на текущий и предыдущий слои буфера
    for (size_t i = 1; i < n; i++)
    {
        current_layer[i] = previous_layer[i - 1];
    }
    // Слой current_layer на следующем шаге должен стать предыдущим. 
    // Для этого сместим индекс текущего слоя в буфере на единицу
    current_layer[0] = left_condition_sulfar;
}

/// @brief get_speed - метод расчета скорости по объемному расходу 
double TransportEquation::get_speed() {
    return volumeFlow / pipeline_characteristics.get_inner_square();
}

/// @brief get_dt - метод получения шага времени dt
double TransportEquation::get_dt()
{
    double speed = get_speed();
    dt = pipeline_characteristics .get_dx() / speed;
    return dt;
}
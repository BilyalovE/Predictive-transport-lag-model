#include "TransportEquation.h"

TransportEquation::TransportEquation(Pipeline_parameters& pipeline_characteristics, int j)
{
    /// @param n - ���������� ����� ��������� �����;
    this->n = pipeline_characteristics.n;
    /// @param j - ������� ����
    this->j = j;
    /// @param pipeline_characteristics - ��������� ������������
    this->pipeline_characteristics = pipeline_characteristics;
    /// @param dx - �������� ���� ����� ������ ��������� �����, �;
    this->dx = pipeline_characteristics.L / (n - 1);
}

void TransportEquation::methodCharacteristic(ring_buffer_t <vector<double>> buffer, double left_condition) 
{
    vector<double> current_layer = buffer.current();
    vector<double> previous_layer = buffer.previous();
    // ��������� ������ �� ������� � ���������� ���� ������
    for (size_t i = 1; i < n; i++)
    {
        current_layer[i] = previous_layer[i - 1];
    }
    // ���� current_layer �� ��������� ���� ������ ����� ����������. 
    // ��� ����� ������� ������ �������� ���� � ������ �� �������
    current_layer[0] = left_condition;
}

/// @brief get_speed - ����� ������� �������� �� ��������� ������� 
double TransportEquation::get_speed() {
  
   
}

/// @brief get_dt - ����� ��������� ���� ������� dt
double TransportEquation::get_dt()
{
    double speed = get_speed();
    dt = dx / speed;
    return dt;
}
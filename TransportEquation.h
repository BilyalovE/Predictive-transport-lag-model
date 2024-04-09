#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <locale.h>
#include <fstream>
#include <iomanip>
#include <fixed/fixed.h>
#include <pde_solvers/pde_solvers.h>
#include "Pipe.h"

class TransportEquation
{
    std::vector <double> volumeFlow;
    /// @param n - количество точек расчетной сетки;
    int n;
    /// @param dt - шаг во времени из условия Куранта;
    double dt;
    /// @param speed - скорость движения нефти
    double speed;
    /// @param pipe - параметры трубопровода
    Pipeline_parameters pipe;
    /// @brief j - счетчик слоя
    int j;
public:
    /// @brief Конструктор класса Block_1
    /// @param pipeline_characteristics - Структура исходных параметров трубопровода
    /// @param n - количество точек расчетной сетки;
    /// @param j - счетчик текущего слоя
    TransportEquation(const Pipeline_parameters& pipeline_characteristics, const std::vector <double> volumeFlow);

    // @brief methodCharacteristic - метод характеристик, рассчитывающий слои
    /// @param buffer - буфер, который для расчёта хранит 2 слоя (текущий и прядущий);
    /// @param left_condition - граничное условие для параметра нефти.
    void methodCharacteristic(vector<double>& current_layer, vector<double>& previous_layer, double left_condition_sulfar, int j) ;

    /// @brief get_speed - метод расчета скорости по расходу (расход может быть интерполирован)
    double get_speed();

    /// @brief get_dt - метод получения шага времени dt
    double get_dt();
};


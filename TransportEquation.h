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
    // ѕол€ класса
    /// @param n - количество точек расчетной сетки;
    int n;
    /// @param j - счетчик сло€
    int j;
    /// @param dx - величина шага между узлами расчетной сетки, м;
    double dx;
    /// @param dt - шаг во времени из услови€  уранта;
    double dt;
    /// @param speed - скорость движени€ нефти
    double speed;
    /// @param pipeline_characteristics - параметры трубопровода
    Pipeline_parameters pipeline_characteristics;

    /// @brief  онструктор класса Block_1
    /// @param pipeline_characteristics - —труктура исходных параметров трубопровода
    /// @param n - количество точек расчетной сетки;
    /// @param j - счетчик текущего сло€
    TransportEquation(Pipeline_parameters& pipeline_characteristics, int j);

    // @brief methodCharacteristic - метод характеристик, рассчитывающий слои
    /// @param buffer - буфер, который дл€ расчЄта хранит 2 сло€ (текущий и пр€дущий);
    /// @param left_condition - граничное условие дл€ параметра нефти.
    void methodCharacteristic(ring_buffer_t <vector<double>> buffer, double left_condition) ;

    /// @brief get_speed - метод расчета скорости по расходу (расход может быть интерполирован)
    double get_speed();

    /// @brief get_dt - метод получени€ шага времени dt
    double get_dt();
};


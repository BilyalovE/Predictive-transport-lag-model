#pragma once
#include <iostream>
#include <vector>
#include <pde_solvers/pde_solvers.h>
/// @brief Pipeline_parameters - структура исходных параметров трубопровода
struct Pipeline_parameters
{
    /// @param L - длина трубопровода, м;
    double L = 1500;
    /// @param D - внешний диаметр трубы, м;
    double D = 0.219;
    /// @param d - толщина стенки трубы, м;
    double d = 0.008;
    /// @brief get_inner_diameter - функция, рассчитывающая внутренний диаметр трубы, [м]
    double get_inner_diameter() const {
        return D - 2 * d;
    };
    /// @brief get_inner_square - функция, рассчитывающая площадь сечения трубы, [м2]
    double get_inner_square() const {
        double inner_diameter = get_inner_diameter();
        double S = M_PI * inner_diameter * inner_diameter / 4;
        return S;
    };
    /// @brief get_dx - функция, рассчитывающая шаг по координате, [м]
    double get_dx() const {
        return L / (n - 1);
    }

    /// @param T - время моделирования, с (24 часа * 3600)
    int T = 86400;
    /// @param - количество точек расчетной сетки;
    int n = 1000;
};
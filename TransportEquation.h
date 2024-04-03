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
    // ���� ������
    /// @param n - ���������� ����� ��������� �����;
    int n;
    /// @param j - ������� ����
    int j;
    /// @param dx - �������� ���� ����� ������ ��������� �����, �;
    double dx;
    /// @param dt - ��� �� ������� �� ������� �������;
    double dt;
    /// @param speed - �������� �������� �����
    double speed;
    /// @param pipeline_characteristics - ��������� ������������
    Pipeline_parameters pipeline_characteristics;

    /// @brief ����������� ������ Block_1
    /// @param pipeline_characteristics - ��������� �������� ���������� ������������
    /// @param n - ���������� ����� ��������� �����;
    /// @param j - ������� �������� ����
    TransportEquation(Pipeline_parameters& pipeline_characteristics, int j);

    // @brief methodCharacteristic - ����� �������������, �������������� ����
    /// @param buffer - �����, ������� ��� ������� ������ 2 ���� (������� � ��������);
    /// @param left_condition - ��������� ������� ��� ��������� �����.
    void methodCharacteristic(ring_buffer_t <vector<double>> buffer, double left_condition) ;

    /// @brief get_speed - ����� ������� �������� �� ������� (������ ����� ���� ��������������)
    double get_speed();

    /// @brief get_dt - ����� ��������� ���� ������� dt
    double get_dt();
};


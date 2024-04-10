#pragma once
#include <pde_solvers/pde_solvers.h>
#include <vector>
#include <locale>
#include <iostream>
#include <iomanip>
#include <pde_solvers/pde_solvers.h>
#include "File.h"
#include "Pipe.h"

class OutPutData
{
	Pipeline_parameters pipe;

	std::vector<double> current_layer;
	double sum_dt;
	std::string output_name;

public:
	OutPutData(std::string output_name, std::vector <double>& current_layer, double sum_dt);

	void output_data();
};


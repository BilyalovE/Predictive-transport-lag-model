#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <locale.h>
#include <fstream>
#include <iomanip>
#include <fixed/fixed.h>
#include <pde_solvers/pde_solvers.h>
#include "File.h"
#include "Pipe.h"



class OutPutData
{
	Pipeline_parameters pipe;

	std::vector<double> previous_layer;
	double sum_dt;
	std::string output_name;
	
	std::string setNormalTimeFormat();

public:
	OutPutData(const std::string output_name, const std::vector <double>& previous_layer, double sum_dt);
	
	
	
	void output_data();
};


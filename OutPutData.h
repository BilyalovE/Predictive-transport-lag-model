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
	double sulfur;
	double time;
	std::vector<double> interpolationVolumeFlow;
	
	std::string setNormalTimeFormat(const double time);
	
	std::vector<double> param;
	std::vector<double> timeLine;

public:
	OutPutData(const std::string output_name, const std::vector <double>& previous_layer, double sum_dt);
	
	OutPutData(const std::string output_name, const double sulfur, const double time);

	OutPutData(const std::string output_name, const std::vector<double>& interpolationVolumeFlow);

	OutPutData(const std::string output_name, const std::vector <double>& param, const std::vector <double>& timeLine);

	void outputParamWithTime();

	void outputTransportDelay();

	void outputModelingFlowRawMaterials();

	void outputPredictSulfar();

	void outputInerpolationVolumeFlow();
};


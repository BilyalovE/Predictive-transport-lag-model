#pragma once
#include <vector>

class LineInterpolation
{
	std::vector <double> array_param;
	std::vector <double> array_time;
	double dt;

public:
	LineInterpolation(const std::vector <double> array_param, const std::vector <double> array_time, const double dt);

	double line_interpolation();
};


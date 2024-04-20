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
#include "LineInterpolation.h"
#include "Pipe.h"

class TransportDelay
{
    double dt;
    double speed;
    double pathWayOnLastLayers;
    double timeDelayPredict{ 1 };
    Pipeline_parameters pipe;

public:
    TransportDelay(const double dt, const double speed);
    double transportDelay();
};


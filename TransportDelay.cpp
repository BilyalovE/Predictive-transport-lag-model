#include "TransportDelay.h"

TransportDelay::TransportDelay(const double dt, const double speed)
{
	this->dt = dt;
	this->speed = speed;
}

double TransportDelay::transportDelay()
{
    if (dt == 0) {
        pathWayOnLastLayers = 0;
    }
    else {
        pathWayOnLastLayers += speed * dt;
    }

    if (pathWayOnLastLayers <= pipe.L) {
        timeDelayPredict = (pipe.L - pathWayOnLastLayers - speed * dt) / speed;
    }
    else {
        timeDelayPredict = 0;
    }
    return timeDelayPredict;
}


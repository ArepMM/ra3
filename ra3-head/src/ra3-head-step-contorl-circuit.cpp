#include    "ra3-head.h"

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void RA3HeadMotor::stepControlCircuit(double t, double dt)
{
    Ucc = tumbler[TUMBLER_CABINE_CHANGE].getState() * bat110->getVoltage();

    bat110->step(t, dt);

    bat24->step(t, dt);
}

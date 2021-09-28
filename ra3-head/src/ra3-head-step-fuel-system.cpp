#include    "ra3-head.h"

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void RA3HeadMotor::stepFuelSystem(double t, double dt)
{
    double common_fuel_level = 0.0;

    for (FuelTank *fuel_tank : fuel_tank)
    {
        fuel_tank->setFuelConsumption(0.0);
        fuel_tank->step(t, dt);
        common_fuel_level += fuel_tank->getFuelLevel();
    }

    // Топливный насос включается непосредственно, или с ведущей секции
    bool is_fuel_pump = mpsu->getOutputData().is_fuel_pump1_ON ||
            static_cast<bool>(forward_inputs[SME_BWD_FUEL_PUMP]);

    fuel_pump->setVoltage(Ucc * static_cast<double>(is_fuel_pump));
    fuel_pump->setFuelLevel(common_fuel_level / 2.0);
    fuel_pump->step(t, dt);
}

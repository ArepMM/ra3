#include    "ra3-head.h"

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void RA3HeadMotor::stepRegistrator(double t, double dt)
{
    if (!is_Registrator_on)
        return;

    QString msg = QString("%1 %2 %3 %4")
            .arg(railway_coord, 10, 'f', 2)
            .arg(blok->getVelocityKmh(), 4, 'f', 2);

    reg->print(msg, t, dt);
}

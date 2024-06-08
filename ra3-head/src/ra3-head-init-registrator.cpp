#include    "ra3-head.h"

#include    <QDir>

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void RA3HeadMotor::initRegistrator()
{
    if (!is_Registrator_on)
        return;

    reg = new Registrator();
    reg->setFileName("ra3-" + QString("%1").arg(num));
    reg->init();
}

#include    "ra3-middle.h"

#include    <QDir>

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void RA3Middle::initControlCircuit(const QString &modules_dir, const QString &custom_cfg_dir)
{
    (void) modules_dir;

    bat110 = new Battery();
    bat110->read_config("bat110", custom_cfg_dir);

    KM_power = new Relay(3);
    KM_power->read_config("mk", custom_cfg_dir);
    KM_power->setInitContactState(0, false);
    KM_power->setInitContactState(1, false);
    KM_power->setInitContactState(2, false);
}

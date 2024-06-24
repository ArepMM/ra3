#include    "ra3-head.h"

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void RA3HeadMotor::initControlCircuit(const QString &modules_dir, const QString &custom_cfg_dir)
{
    (void) modules_dir;

    bat110 = new Battery();
    bat110->read_config("bat110", custom_cfg_dir);

    bat24 = new Battery();
    bat24->read_config("bat24", custom_cfg_dir);

    KM_power = new Relay(3);
    KM_power->read_config("mk", custom_cfg_dir);
    KM_power->setInitContactState(0, false);
    KM_power->setInitContactState(1, false);
    KM_power->setInitContactState(2, false);

    mpsu = new MPSU();
    mpsu->read_config("mpsu", custom_cfg_dir);

    generator = new Generator();
    generator->read_config("generator", custom_cfg_dir);

    aux_conv = new AuxiliaryConverter(custom_cfg_dir);
    aux_conv->read_config("aux-converter", custom_cfg_dir);
}

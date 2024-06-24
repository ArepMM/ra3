#include    "ra3-head.h"

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void RA3HeadMotor::initBrakesControl(const QString &modules_dir, const QString &custom_cfg_dir)
{
    (void) modules_dir;
    (void) custom_cfg_dir;

    // Кран резервного управления
    kru = new KRU091();
    kru->read_config("kru091", custom_cfg_dir);

    epk = new AutoTrainStopEPK151D();
    epk->read_config("epk", custom_cfg_dir);

    emerg_brake_valve = new EmergencyBrakeValve();
    emerg_brake_valve->read_config("emergency-brake-valve", custom_cfg_dir);

}

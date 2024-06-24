#include    "ra3-head.h"

#include    <QDir>

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void RA3HeadMotor::initTraction(const QString &modules_dir, const QString &custom_cfg_dir)
{
    (void) modules_dir;

    hydro_trans = new HydroTransmission();
    hydro_trans->setCustomConfigDir(config_dir);
    hydro_trans->read_config("hydro-transmission", custom_cfg_dir);
}

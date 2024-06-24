#include    "ra3-head.h"

#include    <QDir>

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void RA3HeadMotor::initRegistrator(const QString &modules_dir, const QString &custom_cfg_dir)
{
    (void) modules_dir;
    (void) custom_cfg_dir;

    if (!is_Registrator_on)
        return;

    reg = new Registrator();
    reg->setFileName("ra3-" + QString("%1").arg(num));
    reg->read_config("registrator", custom_cfg_dir);
    reg->init();
}

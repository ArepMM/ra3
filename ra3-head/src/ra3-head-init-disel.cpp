#include    "ra3-head.h"

#include    <QDir>

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void RA3HeadMotor::initDisel(const QString &modules_dir, const QString &custom_cfg_dir)
{
    (void) modules_dir;

    disel = new Disel();
    disel->read_config("disel", custom_cfg_dir);

    // Переключение звука дизеля в неактивных секциях отключено, поскольку все равно не используется
/*    if (active_cab_relay->getContactState(1))*/
        disel->setName("d1");
/*    else
        disel->setName("d2");*/

    starter = new Starter();
    starter->read_config("starter", custom_cfg_dir);

    starter_relay = new Relay(2);
    starter_relay->read_config("starter-relay", custom_cfg_dir);
    starter_relay->setInitContactState(0, false);
    starter_relay->setInitContactState(1, false);
}

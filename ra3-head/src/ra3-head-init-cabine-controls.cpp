#include    "ra3-head.h"

#include    <QDir>

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void RA3HeadMotor::initCabineControls(const QString &modules_dir, const QString &custom_cfg_dir)
{
    (void) modules_dir;

    initTumblers("tumblers-config", custom_cfg_dir);

    km = new TracController();
    km->read_config("trac-controller", custom_cfg_dir);
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void RA3HeadMotor::initTumblers(const QString &config_name, const QString &custom_cfg_dir)
{
    CfgReader cfg;

    if (!cfg.load(custom_cfg_dir + QDir::separator() + config_name + ".xml"))
        return;


    QDomNode node = cfg.getFirstSection("Tumbler");

    while (!node.isNull())
    {
        int id = 0;
        bool state = false;

        if (cfg.getInt(node, "ID", id) && cfg.getBool(node, "State", state))
        {
            if (state)
                tumbler[id].set();
            else
                tumbler[id].reset();
        }

        node = cfg.getNextSection();
    }
}

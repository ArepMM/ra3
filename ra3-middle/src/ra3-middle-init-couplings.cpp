#include    "ra3-middle.h"

#include    <QDir>

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
void RA3Middle::initCouplings(const QString &modules_dir, const QString &custom_cfg_dir)
{
    (void) custom_cfg_dir;

    // Сцепные устройства
    coupling_fwd = loadCoupling(modules_dir + QDir::separator() + coupling_fwd_module_name);
    coupling_fwd->read_config(coupling_fwd_config_name);
    forward_connectors.push_back(coupling_fwd);
    coupling_bwd = loadCoupling(modules_dir + QDir::separator() + coupling_bwd_module_name);
    coupling_bwd->read_config(coupling_bwd_config_name);
    backward_connectors.push_back(coupling_bwd);

    // Расцепные рычаги
/*    oper_rod_fwd = new OperatingRod(KEY_X);
    oper_rod_fwd->read_config("coupling-operating-rod");
    oper_rod_bwd = new OperatingRod(KEY_C);
    oper_rod_bwd->read_config("coupling-operating-rod");*/
}

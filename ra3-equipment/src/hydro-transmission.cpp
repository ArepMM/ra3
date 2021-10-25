#include    "hydro-transmission.h"

#include    <QDir>

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
HydroTransmission::HydroTransmission(QObject *parent) : Device(parent)
  , omega_in(0.0)
  , M_in(0.0)
  , M_out(0.0)
  , omega_out(0.0)
  , k(0.062)
  , T_gt(0.5)
  , T_gm(0.5)
  , T_gb(0.5)
  , u_gt(0.0)
  , u_gm(0.0)
  , u_gb(0.0)
{

}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
HydroTransmission::~HydroTransmission()
{

}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void HydroTransmission::preStep(state_vector_t &Y, double t)
{
    M_in = (Y[0] + Y[1]) * k *  pow(omega_in, 2);

    M_out = getHydroTranstCoeff(omega_in, omega_out) * M_in;
}

//------------------------------------------------------------------------------
//  Y[0] - уровень заполнения гидротрансформатора
//  Y[1] - уровень заполнения гидромуфты
//  Y[2] - уровень заполнения гидротормоза
//------------------------------------------------------------------------------
void HydroTransmission::ode_system(const state_vector_t &Y,
                                   state_vector_t &dYdt,
                                   double t)
{
    dYdt[0] = (u_gt - Y[0]) / T_gt;

    dYdt[1] = (u_gm - Y[1]) / T_gm;

    dYdt[2] = (u_gb - Y[2]) / T_gt;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void HydroTransmission::load_config(CfgReader &cfg)
{
    QString secName = "Device";

    cfg.getDouble(secName, "k", k);
    cfg.getDouble(secName, "T_gt", T_gt);
    cfg.getDouble(secName, "T_gm", T_gm);
    cfg.getDouble(secName, "T_gb", T_gb);

    QString path = custom_config_dir + QDir::separator() + "gdt.csv";
    gt_char.load(path.toStdString());
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
double HydroTransmission::getHydroTranstCoeff(double omega_in, double omega_out)
{
    if (qAbs(omega_in) < 0.001)
        return 0.0;

    return gt_char.getValue(qAbs(omega_out) / qAbs(omega_in));
}

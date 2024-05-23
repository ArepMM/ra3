#include    "ra3-middle.h"

#include    "filesystem.h"

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
RA3Middle::RA3Middle(QObject *parent) : Vehicle(parent)
  , coupling_fwd_module_name("sa3")
  , coupling_fwd_config_name("sa3")
  , coupling_bwd_module_name("sa3")
  , coupling_bwd_config_name("sa3")
  , coupling_fwd(nullptr)
  , coupling_bwd(nullptr)
//  , oper_rod_fwd(nullptr)
//  , oper_rod_bwd(nullptr)
  , num(4003)
  , main_res_leak(0.0)
  , door_R_state(1)
  , door_L_state(1)
  , U_bat_110(0.0)
  , Ucc_110(0.0)
  , bat110(Q_NULLPTR)
  , KM_power(Q_NULLPTR)
  , main_reservoir(Q_NULLPTR)
  , brake_module(Q_NULLPTR)
  , supply_reservoir(Q_NULLPTR)
  , pb_split(Q_NULLPTR)
{

}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
RA3Middle::~RA3Middle()
{

}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void RA3Middle::initialization()
{
    // Определяем путь к загружаемым модулям оборудования
    FileSystem &fs = FileSystem::getInstance();
    QString modules_dir = QString(fs.getModulesDir().c_str());

    // Инициализация сцепных устройств
    initCouplings(modules_dir);

    // Инициализация связей системы многих единиц (СМЕ)
    initSME();

    // Инициализация цепей управления
    initControlCircuit();

    // Инициализация системы обеспечения сжатым воздухом
    initPneumoSupply();

    // Инициализация тормозного оборудования
    initBrakesEquipment();
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void RA3Middle::step(double t, double dt)
{
    // Моделирование сцепных устройств
    stepCouplings(t, dt);

    // Моделирование сигналов СМЕ
    stepSME(t, dt);

    // Работа цепей управления
    stepControlCircuit(t, dt);

    // Работа системы обеспечения сжатым воздухом
    stepPneumoSupply(t, dt);

    // Работа тормозного оборудования
    stepBrakesEquipment(t, dt);

    // Вывод сигналов
    stepSignalsOutput(t, dt);

    // Отладочный вывод по F1
    debugOutput(t, dt);
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void RA3Middle::loadConfig(QString cfg_path)
{
    CfgReader cfg;

    if (cfg.load(cfg_path))
    {
        QString secName = "Vehicle";

        cfg.getDouble(secName, "MainResLeak", main_res_leak);
        cfg.getInt(secName, "Number", num);
    }
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
GET_VEHICLE(RA3Middle)

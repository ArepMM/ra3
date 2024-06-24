#ifndef     RA3_MIDDLE_H
#define     RA3_MIDDLE_H

#include    "vehicle-api.h"

#include    "ra3-middle-signals.h"

#include    "ra3-sme-connector.h"
#include    "battery.h"
#include    "ra3-brake-mech.h"
#include    "bto-092.h"

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
class RA3Middle : public Vehicle
{
public:

    RA3Middle(QObject *parent = Q_NULLPTR);

    ~RA3Middle();

    void initBrakeDevices(double p0, double pBP, double pFL) override;

private:

    /// Имя модуля сцепного устройства спереди
    QString coupling_fwd_module_name;
    /// Имя конфига сцепного устройства спереди
    QString coupling_fwd_config_name;

    /// Имя модуля сцепного устройства сзади
    QString coupling_bwd_module_name;
    /// Имя конфига сцепного устройства сзади
    QString coupling_bwd_config_name;

    /// Сцепка спереди
    Coupling *coupling_fwd;
    /// Сцепка сзади
    Coupling *coupling_bwd;

/*    /// Расцепной рычаг спереди
    OperatingRod *oper_rod_fwd;
    /// Расцепной рычаг сзади
    OperatingRod *oper_rod_bwd;*/

    /// Серийный номер вагона
    int num;

    /// Коэффициент утечки из ГР
    double main_res_leak;

    /// Состояние дверей справа
    int door_R_state;

    /// Состояние дверей слева
    int door_L_state;

    /// Напряжение от батареи 110 В
    double U_bat_110;

    /// Напряжение питания цепей управления 110 В
    double Ucc_110;

    /// Батарея 110 В
    Battery *bat110;

    /// Контактор включения батареи 110 В
    Relay   *KM_power;

    /// Соединения для работы по системе многих единиц (СМЕ) спереди
    RA3SME  *sme_fwd;

    /// Соединения для работы по системе многих единиц (СМЕ) сзади
    RA3SME  *sme_bwd;

    /// Главный резервуар
    Reservoir   *main_reservoir;

    /// Концевой кран питательной магистрали спереди
    PneumoAngleCock *anglecock_fl_fwd;

    /// Концевой кран питательной магистрали сзади
    PneumoAngleCock *anglecock_fl_bwd;

    /// Рукав питательной  магистрали спереди
    PneumoHose      *hose_fl_fwd;

    /// Рукав питательной  магистрали сзади
    PneumoHose      *hose_fl_bwd;

    /// Тормозная магистраль
    Reservoir   *brakepipe;

    /// Блок тормозного оборудования БТО-092
    BTO092  *brake_module;

    /// Запасный резервуар
    Reservoir   *supply_reservoir;

    /// Концевой кран тормозной магистрали спереди
    PneumoAngleCock *anglecock_bp_fwd;

    /// Концевой кран тормозной магистрали сзади
    PneumoAngleCock *anglecock_bp_bwd;

    /// Рукав тормозной магистрали спереди
    PneumoHose   *hose_bp_fwd;

    /// Рукав тормозной магистрали сзади
    PneumoHose   *hose_bp_bwd;

    /// Тройник на питание СТ
    PneumoSplitter *pb_split;

    enum
    {
        NUM_TROLLEYS = 2,
        NUM_AXIS_PER_TROLLEY = 2,
        TROLLEY_FWD = 0,
        TROLLEY_BWD = 1
    };

    /// Тормозные механизмы тележек
    std::array<RA3BrakeMech *, NUM_TROLLEYS> brake_mech;

    void initialization() override;

    /// Инициализация сцепных устройств
    void initCouplings(const QString &modules_dir, const QString &custom_cfg_dir);

    /// Инициализация связей системы многих единиц (СМЕ)
    void initSME(const QString &modules_dir, const QString &custom_cfg_dir);

    /// Инициализация цепей управления
    void initControlCircuit(const QString &modules_dir, const QString &custom_cfg_dir);

    /// Инициализация системы обеспечения сжатым воздухом
    void initPneumoSupply(const QString &modules_dir, const QString &custom_cfg_dir);

    /// Инициализация тормозного оборудования
    void initBrakesEquipment(const QString &modules_dir, const QString &custom_cfg_dir);

    /// Предварительные расчёты перед симуляцией
    void preStep(double t) override;

    /// Предварительный расчёт координат сцепных устройств
    void preStepCouplings(double t);

    /// Шаг моделирования
    void step(double t, double dt) override;

    /// Моделирование сцепных устройств
    void stepCouplings(double t, double dt);

    /// Моделирование сигналов СМЕ
    void stepSME(double t, double dt);

    /// Моделирование работы цепей управления
    void stepControlCircuit(double t, double dt);

    /// Работа системы обеспечения сжатым воздухом
    void stepPneumoSupply(double t, double dt);

    /// Работа тормозного оборудования
    void stepBrakesEquipment(double t, double dt);

    /// Вывод сигналов на анимации модели поезда
    void stepSignalsOutput(double t, double dt);

    /// Отладочный вывод
    void debugOutput(double t, double dt);

    void loadConfig(QString cfg_path) override;
};

#endif // RA3_MIDDLE_H

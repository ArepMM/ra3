#ifndef     MPSU_H
#define     MPSU_H

#include    "device.h"

#include    "mpsu-data.h"

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
class MPSU : public Device
{
public:

    MPSU(QObject *parent = Q_NULLPTR);

    ~MPSU();

    void step(double t, double dt) override;

    void setInputData(const mpsu_input_t &mpsu_input);

    mpsu_output_t getOutputData() const;

private:

    /// Признак сброса МПСУ
    bool is_reseted;

    /// Предыдущее состояние кнопки "СТАРТ"
    bool old_start_state;

    /// Входные сигналы
    mpsu_input_t    mpsu_input;

    /// Выходные сигналы
    mpsu_output_t   mpsu_output;

    enum
    {
        NUM_DISELS = 2,
        FWD_DISEL = 0,
        BWD_DISEL = 1
    };

    /// Тригер активации запуска дизеля
    std::array<Trigger, NUM_DISELS> trig_disel_start;

    void preStep(state_vector_t &Y, double t);

    void ode_system(const state_vector_t &Y, state_vector_t &dYdt, double t);

    void load_config(CfgReader &cfg);

    /// Сброс МПСУ
    void reset();

    /// Главный цикл управления
    void main_loop_step(double t, double dt);

    /// Обработка кнопки старт
    void start_button_process(bool is_start_button);
};

#endif // MPSU_H

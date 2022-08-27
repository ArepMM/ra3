#include "mfdu-main-disp.h"

#include <QPainter>

#include "ra3-head-signals.h"

#include "speedometer.h"
#include "vertical-scale-bar.h"



//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MfduMainDisp::MfduMainDisp(QLabel *parent)
    : QLabel(parent)
{
    speedometer_ = new Speedometer(QSize(274,264), parent);
    speedometer_->move(261, 147);
//    speedometer_->resize(360, 350);
//    speedometer_->setStyleSheet("border: 1px solid green");



    // блок иконок справа от спидометра
    setBlockIcons_rightSpeedometer_(parent);
    // блок иконок слева от спидометра
    setBlockIcons_leftSpeedometer_(parent);
    // вертикальная шкала тяги/торможения
    verticalScaleBar_ = new VerticalScaleBar(QSize(34, 352), parent);
    verticalScaleBar_->move(53,161);
    //verticalScaleBar_->setStyleSheet("border: 1px solid red");
    // блок нижних параметров
    setBlockDownParameters_(parent);
    // блок иконок сверху от спидометра
    setBlockIcon_topSpeedometer_(parent);

    // Сообщения об ошибках
    setErrosMsgBox_(parent);
}


//-----------------------------------------------------------------------------
// Обновление данных на экране: "Основной экран"
//-----------------------------------------------------------------------------
void MfduMainDisp::updateData(display_signals_t input_signals)
{
    // спидометр
    speedometer_->setSpeed(input_signals[MFDU_S_SPEED]);
    speedometer_->setSpeedLimit(input_signals[MFDU_S_SPEED_LIMIT]);
    speedometer_->setGreenDigit(input_signals[MFDU_S_GREEN_DIGIT]);
    speedometer_->setWhiteDigit(input_signals[MFDU_S_WHITE_DIGIT]);

    // блок иконок справа от спидометра
    // 11-15
    setNeededIcon_(lab110380_, input_signals[MFDU_ACTIVE_CHARGE]);
    setNeededIcon_(labPvu_, input_signals[MFDU_PVU]);
    setNeededIcon_(labEpk_, input_signals[MFDU_EPK]);
    setNeededIcon_(labFier1_, input_signals[MFDU_MANEUVER_LIGHTS1]);
    setNeededIcon_(labFier2_, input_signals[MFDU_MANEUVER_LIGHTS2]);
    // 21-25
    setNeededIcon_(labPtf_, input_signals[MFDU_PTF]);
    setNeededIcon_(labPzd_, input_signals[MFDU_PZD]);
    setNeededIcon_(labPump_, input_signals[MFDU_PUMP]);
    setNeededIcon_(labTpn_, input_signals[MFDU_TPN]);
    // 31-35
    setNeededIcon_(labFire_, input_signals[MFDU_FIRE]);
    setNeededIcon_(labOz_, input_signals[MFDU_OZ]);
    setNeededIcon_(labDoor_, input_signals[MFDU_DOOR]);
    setNeededIcon_(labGenerator_, input_signals[MFDU_GENERATOR]);
    setNeededIcon_(labVip_, input_signals[MFDU_VIP]);
    // 41-45
    setNeededIcon_(labIncOzMotor_, input_signals[MFDU_INC_OZ_MOTOR]);
    setNeededIcon_(labDecOzMotor_, input_signals[MFDU_DEC_OZ_MOTOR]);
    setNeededIcon_(labWc_, input_signals[MFDU_WC]);
    setNeededIcon_(labAntyYuz_, input_signals[MFDU_ANTY_YUZ]);
    // 51-55
    setNeededIcon_(labRevers_, input_signals[MFDU_REVERS]);
    setNeededIcon_(labTransmission_, input_signals[MFDU_TRANSMISSION]);
    setNeededIcon_(labOilMotor_, input_signals[MFDU_OIL_MOTOR]);
    setNeededIcon_(labPressureOilMotor_, input_signals[MFDU_PRESSURE_OIL_MOTOR]);
    setNeededIcon_(labMotor_, input_signals[MFDU_MOTOR]);

    // блок иконок слева от спидометра
    setNeededIcon_(labMkFrame_, input_signals[MFDU_COMPRESSOR]);
    setNeededIcon_(labMotorCompressor_, input_signals[MFDU_COMPRESSOR]);
    setNeededIcon_(labMotorCompressor1_, input_signals[MFDU_COMPRESSOR_1]);
    setNeededIcon_(labMotorCompressor2_, input_signals[MFDU_COMPRESSOR_2]);
    setNeededIcon_(labAttention_, input_signals[MFDU_ATTENTION]);
    setNeededIcon_(labStop_, input_signals[MFDU_STOP]);
    setNeededIcon_(labFwd_, input_signals[MFDU_REVERS_FWD]);
    setNeededIcon_(labBwd_, input_signals[MFDU_REVERS_BWD]);
    setNeededIcon_(labBwdFwd_, input_signals[MFDU_REVERS_NEUTRAL]);
    setNeededIcon_(labXren1_, input_signals[MFDU_XREN1]);
    setNeededIcon_(labXren2_, input_signals[MFDU_XREN2]);
    setNeededIcon_(labXren3_, input_signals[MFDU_XREN3]);

    // вертикальная шкала тяги/торможения
    verticalScaleBar_->setVal(input_signals[MFDU_TRACTION_BRAKING]);

    // блок нижних параметров
    labPpm_->setText(QString::number(input_signals[MFDU_PRESSURE_PM], 'f', 3));
    labPtm_->setText(QString::number(input_signals[MFDU_PRESSURE_TM], 'f', 3));
    labPtc_max_->setText(QString::number(input_signals[MFDU_PRESSURE_TC_MAX], 'f', 3));
    labPtc_min_->setText(QString::number(input_signals[MFDU_PRESSURE_TC_MIN], 'f', 3));
    labTkab_->setText(QString::number(input_signals[MFDU_TEMPERATURE_KAB], 'f', 1));
    labIakb24_->setText(QString::number(input_signals[MFDU_I_AKB_24], 'f', 1));
    labIakb110_->setText(QString::number(input_signals[MFDU_I_AKB_110], 'f', 1));

    // блок иконок сверху от спидометра
    labNoHead_->setText(QString("%1")
        .arg(static_cast<int>(input_signals[MFDU_TRAIN_UNIT_NUM]), 5, 10, QChar('0')));
    int size = static_cast<int>(input_signals[MFDU_TRAIN_SIZE]);
    if (size > MAX_TRAIN_SIZE)
        size = MAX_TRAIN_SIZE;
    int x = 402 - 66 * size;
    int y = 62;
    for (int i = 0; i < size; i++)
    {
        setNeededIcon_(labTrainUnit_[i], input_signals[MFDU_TRAIN_UNIT + i * MFDU_UNIT_SIGNALS_SIZE], x, y);
        int num = static_cast<int>(input_signals[MFDU_TRAIN_UNIT_NUM + i * MFDU_UNIT_SIGNALS_SIZE]);
        if (num > 100)
        {
            setNeededIcon_(labCAN_[i], -1);
            labNo1_[i]->setText(QString("%1").arg(num / 100, 3, 10, QChar('0')));
            labNo1_[i]->move(x + 15, y + 1);
            labNo2_[i]->setText(QString("%1").arg(num % 100, 2, 10, QChar('0')));
            labNo2_[i]->move(x + 15, y + 15);
        }
        else
        {
            setNeededIcon_(labCAN_[i], 0, x + 20, y + 2);
            labNo1_[i]->setText(QString(""));
            labNo2_[i]->setText(QString(""));
        }
        labT_[i]->setText(QString::number(input_signals[MFDU_TRAIN_UNIT_T + i * MFDU_UNIT_SIGNALS_SIZE], 'f', 1) + QString("°C"));
        labT_[i]->move(x + 60, y + 8);
        setNeededIcon_(labVagonEquipment_[i], input_signals[MFDU_TRAIN_UNIT_EQUIP + i * MFDU_UNIT_SIGNALS_SIZE], x + 12, y + 42);
        setNeededIcon_(labDiesel_[i], input_signals[MFDU_TRAIN_UNIT_DIESEL + i * MFDU_UNIT_SIGNALS_SIZE], x + 47, y + 42);
        setNeededIcon_(labBrakes_[i], input_signals[MFDU_TRAIN_UNIT_BRAKES + i * MFDU_UNIT_SIGNALS_SIZE], x + 82, y + 42);
        x += 132;
    }

    if (size < MAX_TRAIN_SIZE)
        for (int i = size; i < MAX_TRAIN_SIZE; i++)
        {
            setNeededIcon_(labTrainUnit_[i], -1);
            setNeededIcon_(labCAN_[i], -1);
            labNo1_[i]->setText(QString(""));
            labNo2_[i]->setText(QString(""));
            labT_[i]->setText(QString(""));
            setNeededIcon_(labVagonEquipment_[i], -1);
            setNeededIcon_(labDiesel_[i], -1);
            setNeededIcon_(labBrakes_[i], -1);
        }

    // сообщение об ошибке
    setNeededIcon_(labErros_, input_signals[MFDU_ERROR_CODE]);
}

void MfduMainDisp::setBlockIcons_rightSpeedometer_(QLabel *parent)
{
    int startX = speedometer_->x() + speedometer_->width() + 13;
    int startY = speedometer_->y() + 5;
    int fooX = 0;
    int fooY = 0;
    int fooDeltaX = 50;
    int fooDeltaY = 52;
    QPixmap pixmap;
    QLabel* labFoo = Q_NULLPTR;

    //
    fooX = startX;
    fooY = startY;

    // 11
    fooNoneAdd_(pixmap, parent, lab110380_, fooX, fooY);
    if (!pixmap.load(":/mfdu/main_110380_380")) { return; }
    labFoo = new QLabel(parent);
    labFoo->move(fooX,fooY);
    labFoo->setPixmap(pixmap);
    lab110380_.push_back(labFoo);
    if (!pixmap.load(":/mfdu/main_110380_110")) { return; }
    labFoo = new QLabel(parent);
    labFoo->move(fooX,fooY);
    labFoo->setPixmap(pixmap);
    lab110380_.push_back(labFoo);
    if (!pixmap.load(":/mfdu/main_110380_380110")) { return; }
    labFoo = new QLabel(parent);
    labFoo->move(fooX,fooY);
    labFoo->setPixmap(pixmap);
    lab110380_.push_back(labFoo);
    if (!pixmap.load(":/mfdu/main_110380_noActive")) { return; }
    labFoo = new QLabel(parent);
    labFoo->move(fooX,fooY);
    labFoo->setPixmap(pixmap);
    lab110380_.push_back(labFoo);


    fooX += fooDeltaX;
    // 12
    fooNoneAdd_(pixmap, parent, labPvu_, fooX, fooY);
    if (!pixmap.load(":/mfdu/main_pvu")) { return; }
    labFoo = new QLabel(parent);
    labFoo->move(fooX,fooY);
    labFoo->setPixmap(pixmap);
    labPvu_.push_back(labFoo);
    if (!pixmap.load(":/mfdu/main_pvu_noActive")) { return; }
    labFoo = new QLabel(parent);
    labFoo->move(fooX,fooY);
    labFoo->setPixmap(pixmap);
    labPvu_.push_back(labFoo);


    fooX += fooDeltaX;
    // 13
    fooNoneAdd_(pixmap, parent, labEpk_, fooX, fooY);
    if (!pixmap.load(":/mfdu/main_epk_on")) { return; }
    labFoo = new QLabel(parent);
    labFoo->move(fooX,fooY);
    labFoo->setPixmap(pixmap);
    labEpk_.push_back(labFoo);
    if (!pixmap.load(":/mfdu/main_epk_off")) { return; }
    labFoo = new QLabel(parent);
    labFoo->move(fooX,fooY);
    labFoo->setPixmap(pixmap);
    labEpk_.push_back(labFoo);


    fooX += fooDeltaX;
    // 14
    fooNoneAdd_(pixmap, parent, labFier1_, fooX, fooY);
    if (!pixmap.load(":/mfdu/main_fier1_noSignals")) { return; }
    labFoo = new QLabel(parent);
    labFoo->move(fooX,fooY);
    labFoo->setPixmap(pixmap);
    labFier1_.push_back(labFoo);
    if (!pixmap.load(":/mfdu/main_fier1_maneuver")) { return; }
    labFoo = new QLabel(parent);
    labFoo->move(fooX,fooY);
    labFoo->setPixmap(pixmap);
    labFier1_.push_back(labFoo);
    if (!pixmap.load(":/mfdu/main_fier1_redFiers")) { return; }
    labFoo = new QLabel(parent);
    labFoo->move(fooX,fooY);
    labFoo->setPixmap(pixmap);
    labFier1_.push_back(labFoo);
    if (!pixmap.load(":/mfdu/main_fier1_2signals")) { return; }
    labFoo = new QLabel(parent);
    labFoo->move(fooX,fooY);
    labFoo->setPixmap(pixmap);
    labFier1_.push_back(labFoo);


    fooX += fooDeltaX;
    // 15
    fooNoneAdd_(pixmap, parent, labFier2_, fooX, fooY);
    if (!pixmap.load(":/mfdu/main_fier2_noSignals")) { return; }
    labFoo = new QLabel(parent);
    labFoo->move(fooX,fooY);
    labFoo->setPixmap(pixmap);
    labFier2_.push_back(labFoo);
    if (!pixmap.load(":/mfdu/main_fier2_whiteFiers")) { return; }
    labFoo = new QLabel(parent);
    labFoo->move(fooX,fooY);
    labFoo->setPixmap(pixmap);
    labFier2_.push_back(labFoo);
    if (!pixmap.load(":/mfdu/main_fier2_whiteRedFiers")) { return; }
    labFoo = new QLabel(parent);
    labFoo->move(fooX,fooY);
    labFoo->setPixmap(pixmap);
    labFier2_.push_back(labFoo);
    if (!pixmap.load(":/mfdu/main_fier2_2signals")) { return; }
    labFoo = new QLabel(parent);
    labFoo->move(fooX,fooY);
    labFoo->setPixmap(pixmap);
    labFier2_.push_back(labFoo);


    fooX = startX + fooDeltaX;
    fooY += fooDeltaY;
    // 22
    fooNoneAdd_(pixmap, parent, labPtf_, fooX, fooY);
    if (!pixmap.load(":/mfdu/main_ptf")) { return; }
    labFoo = new QLabel(parent);
    labFoo->move(fooX,fooY);
    labFoo->setPixmap(pixmap);
    labPtf_.push_back(labFoo);
    if (!pixmap.load(":/mfdu/main_ptf_noActive")) { return; }
    labFoo = new QLabel(parent);
    labFoo->move(fooX,fooY);
    labFoo->setPixmap(pixmap);
    labPtf_.push_back(labFoo);


    fooX += fooDeltaX;
    // 23
    fooNoneAdd_(pixmap, parent, labPzd_, fooX, fooY);
    if (!pixmap.load(":/mfdu/main_pzd")) { return; }
    labFoo = new QLabel(parent);
    labFoo->move(fooX,fooY);
    labFoo->setPixmap(pixmap);
    labPzd_.push_back(labFoo);
    if (!pixmap.load(":/mfdu/main_pzd_burn")) { return; }
    labFoo = new QLabel(parent);
    labFoo->move(fooX,fooY);
    labFoo->setPixmap(pixmap);
    labPzd_.push_back(labFoo);
    if (!pixmap.load(":/mfdu/main_pzd_fail")) { return; }
    labFoo = new QLabel(parent);
    labFoo->move(fooX,fooY);
    labFoo->setPixmap(pixmap);
    labPzd_.push_back(labFoo);
    if (!pixmap.load(":/mfdu/main_pzd_noActive")) { return; }
    labFoo = new QLabel(parent);
    labFoo->move(fooX,fooY);
    labFoo->setPixmap(pixmap);
    labPzd_.push_back(labFoo);


    fooX += fooDeltaX;
    // 24
    fooNoneAdd_(pixmap, parent, labPump_, fooX, fooY);
    if (!pixmap.load(":/mfdu/main_pump")) { return; }
    labFoo = new QLabel(parent);
    labFoo->move(fooX,fooY);
    labFoo->setPixmap(pixmap);
    labPump_.push_back(labFoo);
    if (!pixmap.load(":/mfdu/main_pump_fail")) { return; }
    labFoo = new QLabel(parent);
    labFoo->move(fooX,fooY);
    labFoo->setPixmap(pixmap);
    labPump_.push_back(labFoo);
    if (!pixmap.load(":/mfdu/main_pump_noActive")) { return; }
    labFoo = new QLabel(parent);
    labFoo->move(fooX,fooY);
    labFoo->setPixmap(pixmap);
    labPump_.push_back(labFoo);


    fooX += fooDeltaX;
    // 25
    fooNoneAdd_(pixmap, parent, labTpn_, fooX, fooY);
    if (!pixmap.load(":/mfdu/main_tpn")) { return; }
    labFoo = new QLabel(parent);
    labFoo->move(fooX,fooY);
    labFoo->setPixmap(pixmap);
    labTpn_.push_back(labFoo);
    if (!pixmap.load(":/mfdu/main_tpn_noActive")) { return; }
    labFoo = new QLabel(parent);
    labFoo->move(fooX,fooY);
    labFoo->setPixmap(pixmap);
    labTpn_.push_back(labFoo);


    fooX = startX;
    fooY += fooDeltaY;
    // 31
    fooNoneAdd_(pixmap, parent, labFire_, fooX, fooY);
    if (!pixmap.load(":/mfdu/main_fire")) { return; }
    labFoo = new QLabel(parent);
    labFoo->move(fooX,fooY);
    labFoo->setPixmap(pixmap);
    labFire_.push_back(labFoo);
    if (!pixmap.load(":/mfdu/main_fire_noActive")) { return; }
    labFoo = new QLabel(parent);
    labFoo->move(fooX,fooY);
    labFoo->setPixmap(pixmap);
    labFire_.push_back(labFoo);


    fooX += fooDeltaX;
    // 32
    fooNoneAdd_(pixmap, parent, labOz_, fooX, fooY);
    if (!pixmap.load(":/mfdu/main_oz_1")) { return; }
    labFoo = new QLabel(parent);
    labFoo->move(fooX,fooY);
    labFoo->setPixmap(pixmap);
    labOz_.push_back(labFoo);
    if (!pixmap.load(":/mfdu/main_oz_2")) { return; }
    labFoo = new QLabel(parent);
    labFoo->move(fooX,fooY);
    labFoo->setPixmap(pixmap);
    labOz_.push_back(labFoo);
    if (!pixmap.load(":/mfdu/main_oz_noActive")) { return; }
    labFoo = new QLabel(parent);
    labFoo->move(fooX,fooY);
    labFoo->setPixmap(pixmap);
    labOz_.push_back(labFoo);


    fooX += fooDeltaX;
    // 33
    fooNoneAdd_(pixmap, parent, labDoor_, fooX, fooY);
    if (!pixmap.load(":/mfdu/main_door")) { return; }
    labFoo = new QLabel(parent);
    labFoo->move(fooX,fooY);
    labFoo->setPixmap(pixmap);
    labDoor_.push_back(labFoo);
    if (!pixmap.load(":/mfdu/main_door_noActive")) { return; }
    labFoo = new QLabel(parent);
    labFoo->move(fooX,fooY);
    labFoo->setPixmap(pixmap);
    labDoor_.push_back(labFoo);


    fooX += fooDeltaX;
    // 34
    fooNoneAdd_(pixmap, parent, labGenerator_, fooX, fooY);
    if (!pixmap.load(":/mfdu/main_generator")) { return; }
    labFoo = new QLabel(parent);
    labFoo->move(fooX,fooY);
    labFoo->setPixmap(pixmap);
    labGenerator_.push_back(labFoo);
    if (!pixmap.load(":/mfdu/main_generator_noActive")) { return; }
    labFoo = new QLabel(parent);
    labFoo->move(fooX,fooY);
    labFoo->setPixmap(pixmap);
    labGenerator_.push_back(labFoo);


    fooX += fooDeltaX;
    // 35
    fooNoneAdd_(pixmap, parent, labVip_, fooX, fooY);
    if (!pixmap.load(":/mfdu/main_vip")) { return; }
    labFoo = new QLabel(parent);
    labFoo->move(fooX,fooY);
    labFoo->setPixmap(pixmap);
    labVip_.push_back(labFoo);
    if (!pixmap.load(":/mfdu/main_vip_noActive")) { return; }
    labFoo = new QLabel(parent);
    labFoo->move(fooX,fooY);
    labFoo->setPixmap(pixmap);
    labVip_.push_back(labFoo);


    fooX = startX;
    fooY += fooDeltaY;
    // 41
    fooNoneAdd_(pixmap, parent, labIncOzMotor_, fooX, fooY);
    if (!pixmap.load(":/mfdu/main_incOzMotor_1")) { return; }
    labFoo = new QLabel(parent);
    labFoo->move(fooX,fooY);
    labFoo->setPixmap(pixmap);
    labIncOzMotor_.push_back(labFoo);
    if (!pixmap.load(":/mfdu/main_incOzMotor_2")) { return; }
    labFoo = new QLabel(parent);
    labFoo->move(fooX,fooY);
    labFoo->setPixmap(pixmap);
    labIncOzMotor_.push_back(labFoo);
    if (!pixmap.load(":/mfdu/main_incOzMotor_noActive")) { return; }
    labFoo = new QLabel(parent);
    labFoo->move(fooX,fooY);
    labFoo->setPixmap(pixmap);
    labIncOzMotor_.push_back(labFoo);


    fooX += fooDeltaX;
    // 42
    fooNoneAdd_(pixmap, parent, labDecOzMotor_, fooX, fooY);
    if (!pixmap.load(":/mfdu/main_decOzMotor_1")) { return; }
    labFoo = new QLabel(parent);
    labFoo->move(fooX,fooY);
    labFoo->setPixmap(pixmap);
    labDecOzMotor_.push_back(labFoo);
    if (!pixmap.load(":/mfdu/main_decOzMotor_2")) { return; }
    labFoo = new QLabel(parent);
    labFoo->move(fooX,fooY);
    labFoo->setPixmap(pixmap);
    labDecOzMotor_.push_back(labFoo);
    if (!pixmap.load(":/mfdu/main_decOzMotor_noActive")) { return; }
    labFoo = new QLabel(parent);
    labFoo->move(fooX,fooY);
    labFoo->setPixmap(pixmap);
    labDecOzMotor_.push_back(labFoo);


    fooX += fooDeltaX*2;
    // 44
    fooNoneAdd_(pixmap, parent, labWc_, fooX, fooY);
    if (!pixmap.load(":/mfdu/main_wc")) { return; }
    labFoo = new QLabel(parent);
    labFoo->move(fooX,fooY);
    labFoo->setPixmap(pixmap);
    labWc_.push_back(labFoo);
    if (!pixmap.load(":/mfdu/main_wc_noActive")) { return; }
    labFoo = new QLabel(parent);
    labFoo->move(fooX,fooY);
    labFoo->setPixmap(pixmap);
    labWc_.push_back(labFoo);


    fooX += fooDeltaX;
    // 45
    fooNoneAdd_(pixmap, parent, labAntyYuz_, fooX, fooY);
    if (!pixmap.load(":/mfdu/main_antyYuz_work")) { return; }
    labFoo = new QLabel(parent);
    labFoo->move(fooX,fooY);
    labFoo->setPixmap(pixmap);
    labAntyYuz_.push_back(labFoo);
    if (!pixmap.load(":/mfdu/main_antyYuz_fail")) { return; }
    labFoo = new QLabel(parent);
    labFoo->move(fooX,fooY);
    labFoo->setPixmap(pixmap);
    labAntyYuz_.push_back(labFoo);
    if (!pixmap.load(":/mfdu/main_antyYuz_noActive")) { return; }
    labFoo = new QLabel(parent);
    labFoo->move(fooX,fooY);
    labFoo->setPixmap(pixmap);
    labAntyYuz_.push_back(labFoo);


    fooX = startX;
    fooY += fooDeltaY;
    // 51
    fooNoneAdd_(pixmap, parent, labRevers_, fooX, fooY);
    if (!pixmap.load(":/mfdu/main_revers")) { return; }
    labFoo = new QLabel(parent);
    labFoo->move(fooX,fooY);
    labFoo->setPixmap(pixmap);
    labRevers_.push_back(labFoo);
    if (!pixmap.load(":/mfdu/main_revers_noActive")) { return; }
    labFoo = new QLabel(parent);
    labFoo->move(fooX,fooY);
    labFoo->setPixmap(pixmap);
    labRevers_.push_back(labFoo);


    fooX += fooDeltaX;
    // 52
    fooNoneAdd_(pixmap, parent, labTransmission_, fooX, fooY);
    if (!pixmap.load(":/mfdu/main_transmission")) { return; }
    labFoo = new QLabel(parent);
    labFoo->move(fooX,fooY);
    labFoo->setPixmap(pixmap);
    labTransmission_.push_back(labFoo);
    if (!pixmap.load(":/mfdu/main_transmission_noActive")) { return; }
    labFoo = new QLabel(parent);
    labFoo->move(fooX,fooY);
    labFoo->setPixmap(pixmap);
    labTransmission_.push_back(labFoo);


    fooX += fooDeltaX;
    // 53
    fooNoneAdd_(pixmap, parent, labOilMotor_, fooX, fooY);
    if (!pixmap.load(":/mfdu/main_oilMotor_up")) { return; }
    labFoo = new QLabel(parent);
    labFoo->move(fooX,fooY);
    labFoo->setPixmap(pixmap);
    labOilMotor_.push_back(labFoo);
    if (!pixmap.load(":/mfdu/main_oilMotor_down")) { return; }
    labFoo = new QLabel(parent);
    labFoo->move(fooX,fooY);
    labFoo->setPixmap(pixmap);
    labOilMotor_.push_back(labFoo);
    if (!pixmap.load(":/mfdu/main_oilMotor_noActive")) { return; }
    labFoo = new QLabel(parent);
    labFoo->move(fooX,fooY);
    labFoo->setPixmap(pixmap);
    labOilMotor_.push_back(labFoo);


    fooX += fooDeltaX;
    // 54
    fooNoneAdd_(pixmap, parent, labPressureOilMotor_, fooX, fooY);
    if (!pixmap.load(":/mfdu/main_pressureOilMotor_up")) { return; }
    labFoo = new QLabel(parent);
    labFoo->move(fooX,fooY);
    labFoo->setPixmap(pixmap);
    labPressureOilMotor_.push_back(labFoo);
    if (!pixmap.load(":/mfdu/main_pressureOilMotor_down")) { return; }
    labFoo = new QLabel(parent);
    labFoo->move(fooX,fooY);
    labFoo->setPixmap(pixmap);
    labPressureOilMotor_.push_back(labFoo);
    if (!pixmap.load(":/mfdu/main_pressureOilMotor_noActive")) { return; }
    labFoo = new QLabel(parent);
    labFoo->move(fooX,fooY);
    labFoo->setPixmap(pixmap);
    labPressureOilMotor_.push_back(labFoo);


    fooX += fooDeltaX;
    // 55
    fooNoneAdd_(pixmap, parent, labMotor_, fooX, fooY);
    if (!pixmap.load(":/mfdu/main_motor_work")) { return; }
    labFoo = new QLabel(parent);
    labFoo->move(fooX,fooY);
    labFoo->setPixmap(pixmap);
    labMotor_.push_back(labFoo);
    if (!pixmap.load(":/mfdu/main_motor_warn")) { return; }
    labFoo = new QLabel(parent);
    labFoo->move(fooX,fooY);
    labFoo->setPixmap(pixmap);
    labMotor_.push_back(labFoo);
    if (!pixmap.load(":/mfdu/main_motor_error")) { return; }
    labFoo = new QLabel(parent);
    labFoo->move(fooX,fooY);
    labFoo->setPixmap(pixmap);
    labMotor_.push_back(labFoo);

}



void MfduMainDisp::setBlockIcons_leftSpeedometer_(QLabel *parent)
{
    int startX = 92;
    int startY = 155;
    int fooX = 0;
    int fooY = 0;

    QPixmap pixmap;

    /*QLabel* labFoo = new QLabel(parent);
    labFoo->resize(151, 76);
    labFoo->move(startX, startY);
    labFoo->setStyleSheet("border: 1px solid white;"
                          "border-radius: 5px;");*/

    fooX = startX;
    fooY = startY;
    QLabel *labFoo = new QLabel(parent);
    if (!pixmap.load(":/mfdu/mk_frame_on")) { return; }
    labFoo->move(fooX, fooY);
    labFoo->setPixmap(pixmap);
    labMkFrame_.push_back(labFoo);

    labFoo = new QLabel(parent);
    if (!pixmap.load(":/mfdu/mk_frame_off")) { return; }
    labFoo->move(fooX, fooY);
    labFoo->setPixmap(pixmap);
    labMkFrame_.push_back(labFoo);

    fooX = startX + 15;
    fooY = startY + 17;
    if (!pixmap.load(":/mfdu/main_motor-compressor-on")) { return; }
    labFoo = new QLabel(parent);
    labFoo->move(fooX,fooY);
    labFoo->setPixmap(pixmap);
    labMotorCompressor_.push_back(labFoo);


    fooX = startX + 74;
    if (!pixmap.load(":/mfdu/main_motor_compressor_1_on")) { return; }
    labFoo = new QLabel(parent);
    labFoo->move(fooX, startY + 4);
    labFoo->setPixmap(pixmap);
    labMotorCompressor1_.push_back(labFoo);
    if (!pixmap.load(":/mfdu/main_motor_compressor_1_off")) { return; }
    labFoo = new QLabel(parent);
    labFoo->move(fooX, startY + 4);
    labFoo->setPixmap(pixmap);
    labMotorCompressor1_.push_back(labFoo);


    fooX += 38;
    if (!pixmap.load(":/mfdu/main_motor_compressor_2_on")) { return; }
    labFoo = new QLabel(parent);
    labFoo->move(fooX, startY + 4);
    labFoo->setPixmap(pixmap);
    labMotorCompressor2_.push_back(labFoo);
    if (!pixmap.load(":/mfdu/main_motor_compressor_2_off")) { return; }
    labFoo = new QLabel(parent);
    labFoo->move(fooX, startY + 4);
    labFoo->setPixmap(pixmap);
    labMotorCompressor2_.push_back(labFoo);


    if (!pixmap.load(":/mfdu/main_attention")) { return; }
    labFoo = new QLabel(parent);
    labFoo->move(startX + 2, startY + 104);
    labFoo->setPixmap(pixmap);
    labAttention_.push_back(labFoo);


    if (!pixmap.load(":/mfdu/main_stop")) { return; }
    labFoo = new QLabel(parent);
    labFoo->move(startX + 100, startY + 101);
    labFoo->setPixmap(pixmap);
    labStop_.push_back(labFoo);


    if (!pixmap.load(":/mfdu/main_fwd")) { return; }
    labFoo = new QLabel(parent);
    labFoo->move(startX + 60, startY + 82);
    labFoo->setPixmap(pixmap);
    labFwd_.push_back(labFoo);


    if (!pixmap.load(":/mfdu/main_bwd")) { return; }
    labFoo = new QLabel(parent);
    labFoo->move(startX + 60, startY + 142);
    labFoo->setPixmap(pixmap);
    labBwd_.push_back(labFoo);


    if (!pixmap.load(":/mfdu/main_bwdfwd")) { return; }
    labFoo = new QLabel(parent);
    labFoo->move(startX + 60, startY + 112);
    labFoo->setPixmap(pixmap);
    labBwdFwd_.push_back(labFoo);


    fooX = startX;
    if (!pixmap.load(":/mfdu/main_xren1")) { return; }
    labFoo = new QLabel(parent);
    labFoo->move(fooX, startY + 190);
    labFoo->setPixmap(pixmap);
    labXren1_.push_back(labFoo);


    fooX += 52;
    if (!pixmap.load(":/mfdu/main_xren2")) { return; }
    labFoo = new QLabel(parent);
    labFoo->move(fooX, startY + 190);
    labFoo->setPixmap(pixmap);
    labXren2_.push_back(labFoo);


    fooX += 52;
    if (!pixmap.load(":/mfdu/main_xren3")) { return; }
    labFoo = new QLabel(parent);
    labFoo->move(fooX, startY + 190);
    labFoo->setPixmap(pixmap);
    labXren3_.push_back(labFoo);

}



void MfduMainDisp::setBlockDownParameters_(QLabel *parent)
{
    int fooX = 90;
    int fooY = 438;

    //
    drawLabel_(parent, labPpm_, QPoint(fooX,fooY));
    drawLabel_(parent, labPtm_, QPoint(fooX + 66,fooY));
    drawLabel_(parent, labPtc_max_, QPoint(fooX,fooY + 42));
    drawLabel_(parent, labPtc_min_, QPoint(fooX + 66,fooY + 42));
    drawLabel_(parent, labTkab_, QPoint(fooX + 570,fooY + 10), "00ffff", Qt::AlignRight | Qt::AlignVCenter);
    drawLabel_(parent, labIakb24_, QPoint(fooX + 570,fooY + 46), "ffff00");
    drawLabel_(parent, labIakb110_, QPoint(fooX + 570 + 69,fooY + 46), "ffff00");

}



void MfduMainDisp::drawLabel_(QLabel *parent, TriggerLabel *&lab, QPoint pos, QString color, Qt::Alignment align)
{
    lab = new TriggerLabel("0", parent);
    lab->move(pos);
    lab->resize(66, 20);
    lab->setAlignment(Qt::AlignCenter);
    lab->setFont(QFont("Arial", 14, 63));
    lab->setStyleSheet("border: none;"
                       "color: #" + color);
    lab->setAlignment(align);
}



void MfduMainDisp::setBlockIcon_topSpeedometer_(QLabel *parent)
{
    labNoHead_ = new QLabel("00101", parent);
    labNoHead_->move(40, 32);
    labNoHead_->resize(80, 20);
    labNoHead_->setFont(QFont("Arial", 16, 63));
    labNoHead_->setStyleSheet("color: grey;");
    labNoHead_->setAlignment(Qt::AlignCenter | Qt::AlignVCenter);

    QPixmap pixmap;

    QLabel* labFoo = Q_NULLPTR;

    for (size_t i = 0; i < MAX_TRAIN_SIZE; i++)
    {
        if (!pixmap.load(":/mfdu/main_trainUnit_empty")) { return; }
        labFoo = new QLabel(parent);
        labFoo->setPixmap(pixmap);
        labTrainUnit_[i].push_back(labFoo);
        if (!pixmap.load(":/mfdu/main_trainUnit_head_fwd")) { return; }
        labFoo = new QLabel(parent);
        labFoo->setPixmap(pixmap);
        labTrainUnit_[i].push_back(labFoo);
        if (!pixmap.load(":/mfdu/main_trainUnit_head_bwd")) { return; }
        labFoo = new QLabel(parent);
        labFoo->setPixmap(pixmap);
        labTrainUnit_[i].push_back(labFoo);
        if (!pixmap.load(":/mfdu/main_trainUnit_middle")) { return; }
        labFoo = new QLabel(parent);
        labFoo->setPixmap(pixmap);
        labTrainUnit_[i].push_back(labFoo);

        labNo1_[i] = new QLabel("001", parent);
        labNo1_[i]->resize(40, 20);
        labNo1_[i]->setFont(QFont("Arial", 12, 63));
        labNo1_[i]->setStyleSheet("color: white;");
        labNo1_[i]->setAlignment(Qt::AlignCenter | Qt::AlignVCenter);

        labNo2_[i] = new QLabel("01", parent);
        labNo2_[i]->resize(40, 20);
        labNo2_[i]->setFont(QFont("Arial", 12, 63));
        labNo2_[i]->setStyleSheet("color: white;");
        labNo2_[i]->setAlignment(Qt::AlignCenter | Qt::AlignVCenter);

        labT_[i] = new QLabel("0.0°C", parent);
        labT_[i]->resize(60, 20);
        labT_[i]->setFont(QFont("Arial", 12, 63));
        labT_[i]->setStyleSheet("color: white;");
        labT_[i]->setAlignment(Qt::AlignCenter | Qt::AlignVCenter);

        if (!pixmap.load(":/mfdu/main_can")) { return; }
        labFoo = new QLabel(parent);
        labFoo->setPixmap(pixmap);
        labCAN_[i].push_back(labFoo);

        if (!pixmap.load(":/mfdu/main_vagonEquipment_no_active")) { return; }
        labFoo = new QLabel(parent);
        labFoo->setPixmap(pixmap);
        labVagonEquipment_[i].push_back(labFoo);
        if (!pixmap.load(":/mfdu/main_vagonEquipment_work")) { return; }
        labFoo = new QLabel(parent);
        labFoo->setPixmap(pixmap);
        labVagonEquipment_[i].push_back(labFoo);
        if (!pixmap.load(":/mfdu/main_vagonEquipment_warn")) { return; }
        labFoo = new QLabel(parent);
        labFoo->setPixmap(pixmap);
        labVagonEquipment_[i].push_back(labFoo);
        if (!pixmap.load(":/mfdu/main_vagonEquipment_error")) { return; }
        labFoo = new QLabel(parent);
        labFoo->setPixmap(pixmap);
        labVagonEquipment_[i].push_back(labFoo);

        if (!pixmap.load(":/mfdu/main_brakes_no_active")) { return; }
        labFoo = new QLabel(parent);
        labFoo->setPixmap(pixmap);
        labBrakes_[i].push_back(labFoo);
        if (!pixmap.load(":/mfdu/main_brakes_gidro")) { return; }
        labFoo = new QLabel(parent);
        labFoo->setPixmap(pixmap);
        labBrakes_[i].push_back(labFoo);
        if (!pixmap.load(":/mfdu/main_brakes_mix")) { return; }
        labFoo = new QLabel(parent);
        labFoo->setPixmap(pixmap);
        labBrakes_[i].push_back(labFoo);
        if (!pixmap.load(":/mfdu/main_brakes_ept")) { return; }
        labFoo = new QLabel(parent);
        labFoo->setPixmap(pixmap);
        labBrakes_[i].push_back(labFoo);
        if (!pixmap.load(":/mfdu/main_brakes_parking")) { return; }
        labFoo = new QLabel(parent);
        labFoo->setPixmap(pixmap);
        labBrakes_[i].push_back(labFoo);

        if (!pixmap.load(":/mfdu/main_diesel_none")) { return; }
        labFoo = new QLabel(parent);
        labFoo->setPixmap(pixmap);
        labDiesel_[i].push_back(labFoo);
        if (!pixmap.load(":/mfdu/main_diesel_work")) { return; }
        labFoo = new QLabel(parent);
        labFoo->setPixmap(pixmap);
        labDiesel_[i].push_back(labFoo);
        if (!pixmap.load(":/mfdu/main_diesel_warn")) { return; }
        labFoo = new QLabel(parent);
        labFoo->setPixmap(pixmap);
        labDiesel_[i].push_back(labFoo);
        if (!pixmap.load(":/mfdu/main_diesel_error")) { return; }
        labFoo = new QLabel(parent);
        labFoo->setPixmap(pixmap);
        labDiesel_[i].push_back(labFoo);

    }
}

void MfduMainDisp::setErrosMsgBox_(QLabel *parent)
{
    int fooX = 230;
    int fooY = 413;

    QPixmap pixmap;

    if (!pixmap.load(":/mfdu/error_none")) { return; }
    QLabel *labFoo = new QLabel(parent);
    labFoo->move(fooX,fooY);
    labFoo->setPixmap(pixmap);
    labErros_.push_back(labFoo);

    if (!pixmap.load(":/mfdu/error_ST1")) { return; }
    labFoo = new QLabel(parent);
    labFoo->move(fooX,fooY);
    labFoo->setPixmap(pixmap);
    labErros_.push_back(labFoo);

    if (!pixmap.load(":/mfdu/error_ST2")) { return; }
    labFoo = new QLabel(parent);
    labFoo->move(fooX,fooY);
    labFoo->setPixmap(pixmap);
    labErros_.push_back(labFoo);

    if (!pixmap.load(":/mfdu/error_ST3")) { return; }
    labFoo = new QLabel(parent);
    labFoo->move(fooX,fooY);
    labFoo->setPixmap(pixmap);
    labErros_.push_back(labFoo);

    if (!pixmap.load(":/mfdu/error_ST4")) { return; }
    labFoo = new QLabel(parent);
    labFoo->move(fooX,fooY);
    labFoo->setPixmap(pixmap);
    labErros_.push_back(labFoo);

    if (!pixmap.load(":/mfdu/error_ST5")) { return; }
    labFoo = new QLabel(parent);
    labFoo->move(fooX,fooY);
    labFoo->setPixmap(pixmap);
    labErros_.push_back(labFoo);

    if (!pixmap.load(":/mfdu/error_ST6")) { return; }
    labFoo = new QLabel(parent);
    labFoo->move(fooX,fooY);
    labFoo->setPixmap(pixmap);
    labErros_.push_back(labFoo);

    if (!pixmap.load(":/mfdu/error_revers_0")) { return; }
    labFoo = new QLabel(parent);
    labFoo->move(fooX,fooY);
    labFoo->setPixmap(pixmap);
    labErros_.push_back(labFoo);

    if (!pixmap.load(":/mfdu/error_epk_off")) { return; }
    labFoo = new QLabel(parent);
    labFoo->move(fooX,fooY);
    labFoo->setPixmap(pixmap);
    labErros_.push_back(labFoo);

    if (!pixmap.load(":/mfdu/error_hold_speed_trac")) { return; }
    labFoo = new QLabel(parent);
    labFoo->move(fooX,fooY);
    labFoo->setPixmap(pixmap);
    labErros_.push_back(labFoo);

    if (!pixmap.load(":/mfdu/error_hold_speed_brake")) { return; }
    labFoo = new QLabel(parent);
    labFoo->move(fooX,fooY);
    labFoo->setPixmap(pixmap);
    labErros_.push_back(labFoo);
}

void MfduMainDisp::fooNoneAdd_(QPixmap &pixmap, QLabel *parent, std::vector<QLabel *> &label, int x, int y)
{
//    if (!pixmap.load(":/mfdu/none")) { return; }
//    QLabel* lab = new QLabel(parent);
//    lab->move(x,y);
//    lab->setPixmap(pixmap);
//    label.push_back(lab);
}

void MfduMainDisp::fooNoneAdd2_(QPixmap &pixmap, QLabel *parent, std::vector<QLabel *> &label, int x, int y)
{
//    if (!pixmap.load(":/mfdu/none35")) { return; }
//    QLabel* lab = new QLabel(parent);
//    lab->move(x,y);
//    lab->setPixmap(pixmap);
//    label.push_back(lab);
}



void MfduMainDisp::setNeededIcon_(std::vector<QLabel *> &vec_lab, int val)
{
    for (int i = 0, n = vec_lab.size(); i < n; ++i)
    {
        if (i == val)
        {
            vec_lab[i]->setVisible(true);
        }
        else
        {
            if (vec_lab[i]-isVisible())
                vec_lab[i]->setVisible(false);
        }
    }
}

void MfduMainDisp::setNeededIcon_(std::vector<QLabel *> &vec_lab, int val, int x, int y)
{
    for (int i = 0, n = vec_lab.size(); i < n; ++i)
    {
        if (i == val)
        {
            vec_lab[i]->move(x, y);
            vec_lab[i]->setVisible(true);
        }
        else
        {
            if (vec_lab[i]-isVisible())
                vec_lab[i]->setVisible(false);
        }
    }
}


QString MfduMainDisp::trainUnitNumber (int val, bool two_lines)
{
    if (val == 0)
        return QString("");
    else
    {
        // TODO
        if (two_lines)
            return QString("001/n01");
        else
            return QString("00101");
    }
}




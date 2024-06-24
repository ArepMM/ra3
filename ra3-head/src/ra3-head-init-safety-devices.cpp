#include    "ra3-head.h"

#include    "filesystem.h"

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void RA3HeadMotor::initSafetyDevices(const QString &modules_dir, const QString &custom_cfg_dir)
{
    (void) modules_dir;
    (void) custom_cfg_dir;

    blok = new BLOK();
    blok->setMaxVelocity(120.0);
    blok->setDirection(dir * orient);
    blok->setTrainLength(mpsu->getOutputData().train_length);
    //Конфиг для БЛОК не сделан
    //blok->read_config("blok", custom_cfg_dir);

    FileSystem &fs = FileSystem::getInstance();
    QString route_path = fs.getRouteRootDir().c_str();
    route_path += QDir::separator() + route_dir;

    // Загрузка электронной карты в БЛОК
    QString speeds_name = "speeds";

    if (dir > 0)
        speeds_name += "1";
    else
        speeds_name += "2";

    QString path = route_path + QDir::separator() + speeds_name + ".conf";
    blok->loadSpeedsMap(path);

    // Загрузка станций в БЛОК
    path = route_path + QDir::separator() + "stations.conf";
    blok->loadStationsMap(path);
}

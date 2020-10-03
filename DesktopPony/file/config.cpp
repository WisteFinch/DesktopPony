#include "config.h"

Config::Config()
{

}

bool Config::read(QString path)
{
    QSettings file(path, QSettings::IniFormat);
    file.beginGroup("option");
    this->characterName = file.value("character_name").toString();
    this->characterSize = file.value("character_size").toInt();
    file.endGroup();

    file.beginGroup("constant");
    this->gravitationalAcceleration = file.value("gravitational_acceleration").toDouble();
    this->obstruction = file.value("obstruction").toDouble();
    file.endGroup();

    file.beginGroup("information");
    this->version = file.value("version").toString();
    this->updateUrl = file.value("update_url").toString();
    file.endGroup();

    return true;
}

bool Config::save(QString path)
{
    QSettings file(path, QSettings::IniFormat);
    file.beginGroup("option");
    file.setValue("character_name", this->characterName);
    file.setValue("character_size", this->characterSize);
    file.endGroup();

    file.beginGroup("constant");
    file.setValue("gravitational_acceleration", this->gravitationalAcceleration);
    file.setValue("obstruction", this->obstruction);
    file.endGroup();

    file.beginGroup("information");
    file.setValue("version", this->version);
    file.setValue("update_url", this->updateUrl);
    file.endGroup();

    return true;
}

void Config::restore(bool flag)
{
    if(flag)
    {
        QFile::remove("config.ini");
        QFile::copy(STR_DEFAULT_CONFIG_PATH, "config.ini");
    }else
    {
        read(STR_DEFAULT_CONFIG_PATH);
        save(STR_DEFAULT_CONFIG_PATH);
    }
}

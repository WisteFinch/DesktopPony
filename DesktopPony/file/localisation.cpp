#include "localisation.h"

Localisation::Localisation()
{
    readLocalisation("zh-cn");
}

bool Localisation::readLocalisation(QString lang)
{
    if(lang.isEmpty())
        lang = this->defaultLang;

    bool flag = false;
    //读取文件夹
    QDir rootDir(":\\localisation\\");
    QDir userDir("localisation\\");
    rootDir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    rootDir.setSorting(QDir::Name);
    userDir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    userDir.setSorting(QDir::Name);
    QFileInfoList rootList = rootDir.entryInfoList();
    QFileInfoList userList = userDir.entryInfoList();
    QFileInfoList list;
    for(int i = 0; i < rootList.size(); i++)
    {
        list.push_back(rootList.at(i));
    }
    for(int i = 0; i < userList.size(); i++)
    {
        list.push_back(userList.at(i));
    }
    QFile *loadFile = nullptr;
    //遍历文件
    for (int i = 0; i < list.size(); i++)
    {
        if(list.at(i).isFile())
        {
            //读取文件
            loadFile = new QFile();
            loadFile->setFileName(list.at(i).filePath());
            loadFile->open(QIODevice::ReadOnly);

            QByteArray allData = loadFile->readAll().simplified();
            loadFile->close();
            delete loadFile;
            loadFile = nullptr;

            QJsonParseError json_error;
            QJsonDocument jsonDoc(QJsonDocument::fromJson(allData, &json_error));

            if(json_error.error != QJsonParseError::NoError)
            {
                continue;
            }

            QJsonObject rootObj = jsonDoc.object();
            QString name = rootObj.value("language").toString();
            if(lang != name)
                continue;
            QJsonObject subObj = rootObj.value("localisation").toObject();
            QStringList keys = subObj.keys();

            for(int i = 0; i < keys.size(); i++)
                {
                    this->localisation.insert(keys.at(i), subObj.value(keys.at(i)).toString());
                }
            flag = true;
        }
    }

    return flag;
}

const QString Localisation::get(QString txt)
{
    QString r = this->localisation.value(txt);
    if(r.isNull())
        return txt;
    return r;
}

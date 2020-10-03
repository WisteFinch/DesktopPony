#include "filecharacter.h"

FileCharacter::FileCharacter()
{

}

void FileCharacter::initCharacterIndex(QString index)
{
    if(index.isNull())
        index = STR_CHARACTER_ROOT_PATH;

    QFile file(index+"\\index.json");
    if(file.exists())
        file.remove();
    QJsonObject indexJson;
    QDir dir("character");
    dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    dir.setSorting(QDir::Name);
    QFileInfoList list = dir.entryInfoList();
    QFile *loadFile;
    for (int i = 0; i < list.size(); ++i)
    {
        if(list.at(i).isFile())
        {
            loadFile = new QFile();
            loadFile->setFileName(list.at(i).filePath());
            loadFile->open(QIODevice::ReadOnly);

            QByteArray allData = loadFile->readAll();
            loadFile->close();
            delete loadFile;
            loadFile = nullptr;

            QJsonParseError json_error;
            QJsonDocument jsonDoc(QJsonDocument::fromJson(allData, &json_error));

            if(json_error.error != QJsonParseError::NoError)
            {
                break;
            }

            QJsonObject tempObj = jsonDoc.object();
            QJsonObject obj = tempObj.value("metadata").toObject();

            if(!obj["name"].isNull())
            {
                indexJson.insert(obj["name"].toString(), list.at(i).filePath());
            }
        }

    }
    QJsonDocument indexJsonDoc;
    indexJsonDoc.setObject(indexJson);

    file.open(QFile::ReadWrite);
    file.write(indexJsonDoc.toJson());
    file.close();

}

QString FileCharacter::getCharacterIndex(QString name, QString index) const
{
    if(index.isNull())
        index = STR_CHARACTER_ROOT_PATH;

    QString p;
    QFile *loadFile = new QFile;
    loadFile->setFileName(index + "\\index.json");
    if(!loadFile->open(QIODevice::ReadOnly))
    {
        return nullptr;
    }

    QByteArray allData = loadFile->readAll();
    loadFile->close();
    delete loadFile;
    loadFile = nullptr;

    QJsonParseError json_error;
    QJsonDocument jsonDoc(QJsonDocument::fromJson(allData, &json_error));

    if(json_error.error != QJsonParseError::NoError)
    {
        return nullptr;
    }

    QJsonObject obj = jsonDoc.object();
    QStringList keys = obj.keys();

    for(int i = 0; i < keys.size(); i++)
    {
        if(keys.at(i) == name)
            return obj[keys.at((i))].toString();
    }

    return nullptr;
}

QJsonObject FileCharacter::getCharacterIndexJsonObject(QString index) const
{
    if(index.isNull())
        index = STR_CHARACTER_ROOT_PATH;

    QString p;
    QFile *loadFile = new QFile;
    loadFile->setFileName(index + "\\index.json");
    if(loadFile->open(QIODevice::ReadOnly))
    {
        QByteArray allData = loadFile->readAll();
        loadFile->close();
        delete loadFile;
        loadFile = nullptr;

        QJsonParseError json_error;
        QJsonDocument jsonDoc(QJsonDocument::fromJson(allData, &json_error));

        if(json_error.error == QJsonParseError::NoError)
        {
            return jsonDoc.object();
        }
    }

    QJsonObject a;
    return a;
}

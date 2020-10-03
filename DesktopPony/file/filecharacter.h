#ifndef FILECHARACTER_H
#define FILECHARACTER_H

#include "metadata.h"
#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonArray>
#include <QJsonObject>

class FileCharacter
{
public:
    FileCharacter();

    /**
     * @brief 生成角色索引
     * @param index 索引地址
     */
    void initCharacterIndex(QString index = nullptr);

    /**
     * @brief 读取角色索引
     * @param 名称
     * @param 索引地址
     * @return 文件地址
     */
    QString getCharacterIndex(QString name, QString index = nullptr) const;

    /**
     * @brief 读取角色索引JsonObject
     * @param 索引地址
     * @return JsonObject
     */
    QJsonObject getCharacterIndexJsonObject(QString index = nullptr) const;
};

#endif // FILECHARACTER_H

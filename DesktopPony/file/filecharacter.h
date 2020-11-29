/**
 * @file file/filecharacter.h
 * @brief 文件-角色
 * @author WisteFinch
 * @date 2020.11.29
 *
 * MIT License
 * Copyright (c) 2019-2020 WisteFinch
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

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

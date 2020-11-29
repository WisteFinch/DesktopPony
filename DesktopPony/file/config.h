/**
 * @file file/config.h
 * @brief 文件-配置文件
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

#ifndef CONFIG_H
#define CONFIG_H

#include "metadata.h"
#include <QFile>
#include <QSettings>
#include <QDir>

class Config
{
public:
    Config();

    QString characterName;///<角色名
    int characterSize;///<角色大小

    double gravitationalAcceleration;///<重力加速度
    double obstruction;///<阻力

    QString version;///<版本
    QString updateUrl;///<更新地址

    /**
     * @brief 读取
     * @param 路径
     * @return flag
     */
    bool read(QString path = "config.ini");

    /**
     * @brief 写入
     * @param 路径
     * @return flag
     */
    bool save(QString path = "config.ini");

    /**
     * @brief 重置
     * @param 模式
     */
    void restore(bool flag = false);


};

#endif // CONFIG_H

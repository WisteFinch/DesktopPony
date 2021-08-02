/**
 * @file data/localisation.h
 * @brief 数据-本地化
 * @author WisteFinch
 * @date 2021.7.18
 *
 * MIT License
 * Copyright (c) 2019-2021 WisteFinch
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

#ifndef LOCALISATION_H
#define LOCALISATION_H

#include "plugin/pluginsharedefinition.h"
#include "plugin/element/pluginelementlocalisation.h"
#include <QString>
#include <QMap>

/**
 * @brief 本地化类
 * @details 为程序提供文本本地化服务
 */
class Localisation
{
public:
    Localisation();
    ~Localisation();

    struct Element {
        QString str;  ///< 本地化文本
        QString uuid_plugin;///< 插件uuid
        QString uuid_element;   ///< 插件元素uuid
        uint index; ///< 索引
    };  ///< 结构体：全局元素
    typedef QMap<QString, Element> TABLE;  ///< 类型定义：对照表 <键, 值>

    /**
     * @brief 初始化
     * @param "插件元素组合列表"函数指针
     */
    void init(PTRFUNC_GET_ELEMENT_PAIR_LIST ptrfunc);

    /**
     * @brief 设置语言
     * @param 语言
     */
    void setLanguage(QString lang);

    /**
     * @brief 获取公有文本
     * @param 键
     * @return 值
     */
    QString get(QString key);

    /**
     * @brief 获取公有文本调试信息
     * @param 键
     * @return 调试信息
     */
    Element getDebugData(QString key);

    /**
     * @brief 获取私有文本
     * @param 插件uuid
     * @param 键
     * @return 值
     */
    QString getPriv(QString uuid, QString key);

    /**
     * @brief 获取私有文本调试信息
     * @param 插件uuid
     * @param 键
     * @return 调试信息
     */
    Element getPrivDebugData(QString uuid, QString key);

    /**
     * @brief 创建本地化索引
     */
    void creatIndex();

private:
    /**
     * @brief 清理
     */
    void clear();

    TABLE *m_p_public = nullptr;///< 公有文本
    QMap<QString, TABLE *> *m_p_private = nullptr;   ///< 私有文本 <插件uuid, 局部本地化>
    PTRFUNC_GET_ELEMENT_PAIR_LIST ptrfun_get_element_pair_list = nullptr;   ///< "插件元素组合列表"函数指针

    QString m_s_language = "zh-hans";
};

#endif // LOCALISATION_H
